#include "test-lib.h"
#include "advice.h"
#include "config.h"
#include "setup.h"
#include "strbuf.h"

static const char expect_advice_msg[] = "hint: This is a piece of advice\nhint: Disable this message with \"git config advice.nestedTag false\"\n";

static void fxn_advise_if_enabled(const char *argv) {
	FILE *file;
	struct strbuf actual = STRBUF_INIT;
	setup_git_directory();

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

	check_str(actual.buf, expect_advice_msg);
	strbuf_release(&actual);

	// Delete the output.txt file after the check is done
    	if (!check(remove("output.txt") == 0)) {
    	    test_msg("Error deleting %s", "output.txt");
    	}
}

static void advice_unset(void) {
	fxn_advise_if_enabled("This is a piece of advice");
}

static void advice_set_true(void) {
	git_config_set("advice.nestedTag", "true");
	fxn_advise_if_enabled("This is a piece of advice");
	git_config_clear();
}

static void advice_set_false(void) {
	git_config_set("advice.nestedTag", "false");
	fxn_advise_if_enabled("This is a piece of advice");
	git_config_clear();
}

int cmd_main(int argc, const char **argv) {
	setenv("TERM", "dumb", 1);

	TEST(advice_unset(), "advice should be printed when config variable is unset");
	TEST(advice_set_true(), "advice should be printed when config variable is set to true");
	TEST(advice_set_false(), "advice should not be printed when config variable is set to false");

	return test_done();
}
