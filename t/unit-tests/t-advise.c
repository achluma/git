#include "test-lib.h"
#include "advice.h"
#include "config.h"
#include "setup.h"
#include "strbuf.h"

static const char expect_advice_msg[] = "hint: This is a piece of advice\nhint: Disable this message with \"git config advice.nestedTag false\"\n";

static void check_advise_if_enabled(const char *argv, const char *conf_val, const char *expect) {
	FILE *file;
	struct strbuf actual = STRBUF_INIT;
	setup_git_directory();

	if (conf_val != NULL) {
		git_config_set("advice.nestedTag", conf_val);
	}

	file = freopen("output.txt", "w", stderr);
	if (!check(!!file)) {
		test_msg("Error opening file %s", "output.txt");
		return;
	}

	advise_if_enabled(ADVICE_NESTED_TAG, "%s", argv);
	fclose(file);

	if (!check(strbuf_read_file(&actual, "output.txt", 0) >= 0)) {
		test_msg("Error reading file %s", "output.txt");
		strbuf_release(&actual);
		return;
	}

	check_str(actual.buf, expect);
	strbuf_release(&actual);

	// Delete the output.txt file after the check is done
	if (!check(remove("output.txt") == 0)) {
		test_msg("Error deleting %s", "output.txt");
	}
}

int cmd_main(int argc, const char **argv) {
	setenv("TERM", "dumb", 1);

	TEST(check_advise_if_enabled("This is a piece of advice", NULL, expect_advice_msg), "advice should be printed when config variable is unset");
	TEST(check_advise_if_enabled("This is a piece of advice", "true", expect_advice_msg), "advice should be printed when config variable is set to true");
	TEST(check_advise_if_enabled("This is a piece of advice", "false", ""), "advice should not be printed when config variable is set to false");

	return test_done();
}
