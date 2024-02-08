#include "test-lib.h"
#include "hash-ll.h"
#include "hex.h"

static const char out_file[] = "./output.txt";

static void hash_impl(const char **av, int algo) {
	git_hash_ctx ctx;
	unsigned char hash[GIT_MAX_HEXSZ];
	unsigned bufsz = 8192;
	int binary = 0;
	char *buffer;
	const struct git_hash_algo *algop = &hash_algos[algo];
	FILE *file;
	size_t data_length;

	int ac = 0;
	while (av[ac] != NULL) {
		ac++;
	}

	if (ac > 1) {
		if (!strcmp(av[1], "-b")) {
			binary = 1;
			if (ac > 2) {
				bufsz = strtoul(av[2], NULL, 10) * 1024 * 1024;
			}
		} else {
			bufsz = strtoul(av[1], NULL, 10) * 1024 * 1024;
		}
	}

	if (!bufsz)
		bufsz = 8192;

	while ((buffer = malloc(bufsz)) == NULL) {
		fprintf(stderr, "bufsz %u is too big, halving...\n", bufsz);
		bufsz /= 2;
		if (bufsz < 1024)
			die("OOPS");
	}

	algop->init_fn(&ctx);

	data_length = strlen(av[0]);

	algop->update_fn(&ctx, av[0], data_length);

	algop->final_fn(hash, &ctx);

	file = freopen(out_file, "wb", stderr);
	if (!check(!!file)) {
		test_msg("Error opening file %s", out_file);
		return;
	}

	if (binary)
		fwrite(hash, 1, algop->rawsz, file);
	else
		puts(hash_to_hex_algop(hash, algop));

	fclose(file);
	free(buffer);
}

static char *grep(const char *pattern, const char *filename) {
	char *result;
	char line[1000];
	FILE *file = fopen(out_file, "r");
	if (!check(!!file)) {
		test_msg("Error opening file %s", out_file);
		return NULL;
	}

	while (fgets(line, sizeof(line), file)) {
		if (strstr(line, pattern)) {
			fclose(file);

			result = strdup(line);
			if (!result) {
				fprintf(stderr, "Memory allocation error\n");
				return NULL;
			}
			return result;
		}
	}

	fclose(file);
	return NULL;
}

static void check_sha256(const char **input, const char *expected) {
	hash_impl(input, GIT_HASH_SHA256);
	check_str(grep(expected, out_file), expected);
}

int cmd_main(int argc, const char **argv) {
	const char *av[] = {"a", NULL};
	TEST(check_sha256(av, "ca978112ca1bbdcafac231b39a23dc4da786eff8147c4e72b9807785afee48bb"), "SHA256 a works");

	return test_done();
}
