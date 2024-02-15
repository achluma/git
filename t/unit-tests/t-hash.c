#include "test-lib.h"
#include "hash-ll.h"
#include "hex.h"
#include "strbuf.h"

static const char *repeat(const char *pattern, size_t repetitions) {
    	struct strbuf buf = STRBUF_INIT;

    	for (size_t i = 0; i < repetitions; i++) {
        	strbuf_addstr(&buf, pattern);
    	}

    	return buf.buf;
	strbuf_release(&buf);
}

static void check_hash(const char *input, const char *expected, int algo) {
	git_hash_ctx ctx;
	unsigned char hash[GIT_MAX_HEXSZ];
	const struct git_hash_algo *algop = &hash_algos[algo];
	size_t data_length;

	if (!input || !*input) {
		test_msg("Error: No input provided");
		return;
	}

	algop->init_fn(&ctx);

	data_length = strlen(input);

	algop->update_fn(&ctx, input, data_length);

	algop->final_fn(hash, &ctx);

	check_str(hash_to_hex_algop(hash, algop), expected);
}

#define TEST_SHA1(input, expected_output) \
    	TEST(check_hash(input, expected_output, GIT_HASH_SHA1 ), \
        	"SHA1 (%s) works", #input)

#define TEST_SHA256(input, expected_output) \
    	TEST(check_hash(input, expected_output, GIT_HASH_SHA256 ), \
        	"SHA256 (%s) works", #input)

int cmd_main(int argc, const char **argv) {
	static const char *aaaaaaaaaa_100000;
	static const char *alphabet_100000;

	setenv("LANG", "C", 1);

	aaaaaaaaaa_100000 = repeat("aaaaaaaaaa", 100000);
	alphabet_100000 = repeat("abcdefghijklmnopqrstuvwxyz", 100000);

	TEST_SHA1(NULL, "da39a3ee5e6b4b0d3255bfef95601890afd80709");
	TEST_SHA1("a", "86f7e437faa5a7fce15d1ddcb9eaeaea377667b8");
	TEST_SHA1("abc", "a9993e364706816aba3e25717850c26c9cd0d89d");
	TEST_SHA1("message digest", "c12252ceda8be8994d5fa0290a47231c1d16aae3");
	TEST_SHA1("abcdefghijklmnopqrstuvwxyz", "32d10c7b8cf96570ca04ce37f2a19d84240d3a89");
	TEST_SHA1(aaaaaaaaaa_100000, "34aa973cd4c4daa4f61eeb2bdbad27316534016f");
	TEST_SHA1("blob 0", "e69de29bb2d1d6434b8b29ae775ad8c2e48c5391");
	TEST_SHA1("blob 3abc", "f2ba8f84ab5c1bce84a7b441cb1959cfc7093b7f");
	TEST_SHA1("tree 0", "4b825dc642cb6eb9a060e54bf8d69288fbee4904");

	TEST_SHA256("", "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
	TEST_SHA256("a", "ca978112ca1bbdcafac231b39a23dc4da786eff8147c4e72b9807785afee48bb");
	TEST_SHA256("abc", "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
	TEST_SHA256("message digest", "f7846f55cf23e14eebeab5b4e1550cad5b509e3348fbc4efa3a1413d393cb650");
	TEST_SHA256("abcdefghijklmnopqrstuvwxyz", "71c480df93d6ae2f1efad1447c66c9525e316218cf51fc8d9ed832f2daf18b73");
	TEST_SHA256(aaaaaaaaaa_100000, "cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0");
	TEST_SHA256(alphabet_100000, "e406ba321ca712ad35a698bf0af8d61fc4dc40eca6bdcea4697962724ccbde35");
	TEST_SHA256("blob 0", "473a0f4c3be8a93681a267e3b1e9a7dcda1185436fe141f7749120a303721813");
	TEST_SHA256("blob 3abc", "c1cf6e465077930e88dc5136641d402f72a229ddd996f627d60e9639eaba35a6");
	TEST_SHA256("tree 0", "6ef19b41225c5369f1c104d45d8d85efa9b057b53b14b4b9b939dd74decc5321");

	return test_done();
}
