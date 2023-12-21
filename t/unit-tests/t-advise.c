#include "test-lib.h"
#include "advice.h"
#include "config.h"
#include "setup.h"

static void fxn_advise_if_enabled(const char *argv)
{
	setup_git_directory();
	git_config(git_default_config, NULL);

	/*
	 * Any advice type can be used for testing, but NESTED_TAG was
	 * selected here and in t0018 where this command is being
	 * executed.
	 */
	advise_if_enabled(ADVICE_NESTED_TAG, "%s", argv);
}

static void advice_unset() {
	const char *actual;
	const char *expect;

	expect = "hint: This is a piece of advice\\nhint: Disable this message with \"git config advice.nestedTag false\"";

	// Run the test command and redirect stderr to actual file
	fxn_advise_if_enabled("This is a piece of advice");

	// Compare the expect and actual
	check_str(actual, expect);
}

static void advice_set_true() {
	const char *actual;
	const char *expect;
	setenv("ADVICE_NESTED_TAG", "true", 1);

	expect = "hint: This is a piece of advice\\nhint: Disable this message with \"git config advice.nestedTag false\"";

	// Run the test command and redirect stderr to actual file
	fxn_advise_if_enabled("This is a piece of advice");

	// Compare the expect and actual
	check_str(actual, expect);
}

static void advice_set_false() {
	const char *actual;
	const char *expect;
	setenv("advice.nestedTag", "false", 1);

	// Run the test command and redirect stderr to actual file
	fxn_advise_if_enabled("This is a piece of advice");

	// Compare the expect and actual
	check_str(actual, expect);
}



int cmd_main(int argc, const char **argv) {
	TEST(advice_unset(), "advice_unset - works as we expect");
	TEST(advice_set_true(), "advice_set_true - works as we expect");
	TEST(advice_set_false(), "advice_set_false - works as we expect");

	return test_done();
}
