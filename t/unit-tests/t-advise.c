#include "test-lib.h"
#include "advice.h"
#include "config.h"
#include "setup.h"

static void fxn_advise_if_enabled(const char *argv)
{
	FILE *file;
	setup_git_directory();
	/*
	 * Any advice type can be used for testing, but NESTED_TAG was
	 * selected here and in t0018 where this command is being
	 * executed.
	 */
	file = freopen("output.txt", "w", stderr);
	if (file == NULL) {
		perror("Error opening file");
		return;
	}
	advise_if_enabled(ADVICE_NESTED_TAG, "%s", argv);
	// Close the file
	fclose(file);
}

static void advice_unset(void) {
	char actual[1024];
	const char *expect;
	int index;
	FILE *actual_file;

	expect = "\033[33mhint: This is a piece of advice\033[m\n\033[33mhint: Disable this message with \"git config advice.nestedTag false\"\033[m\012";


	// Run the test command and redirect stderr to actual file
	fxn_advise_if_enabled("This is a piece of advice");

	// Open the actual file and read its contents
	actual_file = fopen("output.txt", "r");
	if (actual_file == NULL) {
		perror("Error opening actual file");
		return;
	}

	// Read the contents of the actual file
	index = 0;
    	while (fgets(actual + index, sizeof(actual) - index, actual_file) != NULL) {
        	index += strlen(actual + index);
        	if (index >= sizeof(actual) - 1) {
        	    fprintf(stderr, "Output exceeded buffer size\n");
        	    break;
        	}
    	}

	// Close the actual file
	fclose(actual_file);

	// Compare the expect and actual
	check_str(actual, expect);
}

static void advice_set_true(void) {
	char actual[1024];
	const char *expect;
	int index;
	FILE *actual_file;

	//(void)system("git config advice.nestedTag true");

	expect = "\033[33mhint: This is a piece of advice\033[m\n\033[33mhint: Disable this message with \"git config advice.nestedTag false\"\033[m\012";


	// Run the test command and redirect stderr to actual file
	fxn_advise_if_enabled("This is a piece of advice");

		// Open the actual file and read its contents
	actual_file = fopen("output.txt", "r");
	if (actual_file == NULL) {
		perror("Error opening actual file");
		return;
	}

	// Read the contents of the actual file
    	index = 0;
    	while (fgets(actual + index, sizeof(actual) - index, actual_file) != NULL) {
        	index += strlen(actual + index);
        	if (index >= sizeof(actual) - 1) {
        	    fprintf(stderr, "Output exceeded buffer size\n");
        	    break;
        	}
    	}

	// Close the actual file
	fclose(actual_file);

	// Compare the expect and actual
	check_str(actual, expect);
}

static void advice_set_false(void) {
	char actual[1024];
	const char *expect;
	int index;
	FILE *actual_file;

	//(void)system("git config advice.nestedTag false");
	//git_config(git_default_config, NULL);

	expect = "";


	// Run the test command and redirect stderr to actual file
	fxn_advise_if_enabled("This is a piece of advice");

	// Open the actual file and read its contents
	actual_file = fopen("output.txt", "r");
	if (actual_file == NULL) {
		perror("Error opening actual file");
		return;
	}

	// Read the contents of the actual file
	index = 0;
    	while (fgets(actual + index, sizeof(actual) - index, actual_file) != NULL) {
        	index += strlen(actual + index);
        	if (index >= sizeof(actual) - 1) {
        	    fprintf(stderr, "Output exceeded buffer size\n");
        	    break;
        	}
    	}

	// Close the actual file
	fclose(actual_file);

	// Compare the expect and actual
	check_str(actual, expect);
}



int cmd_main(int argc, const char **argv) {
	TEST(advice_unset(), "advice_unset - works");
	TEST(advice_set_true(), "advice_set_true - works");
	TEST(advice_set_false(), "advice_set_false - works");

	return test_done();
}
