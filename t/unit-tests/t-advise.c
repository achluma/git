#include "test-lib.h"
#include "advice.h"
#include "config.h"
#include "setup.h"
#include "strbuf.h"

static const char expect_advice_msg[] = "hint: This is a piece of advice\nhint: Disable this message with \"git config advice.nestedTag false\"\n";

static void fxn_advise_if_enabled(const char *argv)
{
	FILE *file;
	setup_git_directory();
	/*
	 * Any advice type can be used for testing, but NESTED_TAG was
	 * selected here and in t-ctype where this command is being
	 * executed.
	 */
	file = freopen("output.txt", "w", stderr);
	if (!check(!!file)) {
		test_msg("Error opening file %s", "output.txt");
		return;
	}
	advise_if_enabled(ADVICE_NESTED_TAG, "%s", argv);
	// Close the file
	fclose(file);
}

static void advice_unset(void) {
	struct strbuf actual = STRBUF_INIT;

	// Run the test command and redirect stderr to actual file
	fxn_advise_if_enabled("This is a piece of advice");

	// Read the contents of the actual file directly
	if (strbuf_read_file(&actual, "output.txt", 0) < 0) {
		test_msg("Error reading file %s", "output.txt");
		strbuf_release(&actual);
		return;
	}

	// Compare the expect and actual
	check_str(actual.buf, expect_advice_msg);
	strbuf_release(&actual);
}

static void advice_set_true(void) {
	struct strbuf actual = STRBUF_INIT;
	git_config_set("advice.nestedTag","true");

	// Run the test command and redirect stderr to actual file
	fxn_advise_if_enabled("This is a piece of advice");

	// Read the contents of the actual file directly
	if (strbuf_read_file(&actual, "output.txt", 0) < 0) {
		test_msg("Error reading file %s", "output.txt");
		strbuf_release(&actual);
		return;
	}

	// Compare the expect and actual
	check_str(actual.buf, expect_advice_msg);

	// Release strbuf memory
	strbuf_release(&actual);
	git_config_clear();
}


static void advice_set_false(void) {
	struct strbuf actual = STRBUF_INIT;
	git_config_set("advice.nestedTag","false");

	// Run the test command and redirect stderr to actual file
	fxn_advise_if_enabled("This is a piece of advice");

	// Read the contents of the actual file directly
	if (strbuf_read_file(&actual, "output.txt", 0) < 0) {
		test_msg("Error reading file %s", "output.txt");
		strbuf_release(&actual);
		return;
	}

	// Compare the expect and actual
	check_str(actual.buf, "");

	// Release strbuf memory
	strbuf_release(&actual);
	git_config_clear();
}

int cmd_main(int argc, const char **argv) {
	setenv("TERM", "dumb", 1);

	TEST(advice_unset(), "advice should be printed when config variable is unset");
	TEST(advice_set_true(), "advice should be printed when config variable is set to true");
	TEST(advice_set_false(), "advice should not be printed when config variable is set to false");

	return test_done();
}
