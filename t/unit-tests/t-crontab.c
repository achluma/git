#include "test-lib.h"
#include <stdio.h>
#include <string.h>

/* Helper function to compare file contents */
static int compare_file_contents(const char *file1, const char *file2) {
    FILE *f1 = fopen(file1, "r");
    if (!f1)
        return 0;

    FILE *f2 = fopen(file2, "r");
    if (!f2) {
        fclose(f1);
        return 0;
    }

    int result = 1;
    int c1, c2;

    while ((c1 = fgetc(f1)) != EOF && (c2 = fgetc(f2)) != EOF) {
        if (c1 != c2) {
            result = 0;
            break;
        }
    }

    if (c1 != EOF || c2 != EOF)
        result = 0;

    fclose(f1);
    fclose(f2);

    return result;
}

/* Unit tests for cmd__crontab function */
static void test_cmd_crontab_read(void) {
    const char *filename = "test.txt";
    const char *expectedOutput = "This is a test file.\n";

    FILE *file = fopen(filename, "w");
    if (!file)
        fail("Failed to create test file");

    fputs(expectedOutput, file);

    fclose(file);

    const char *argv[] = {"test-tool", "crontab", filename, "-l"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    capture_output();
    cmd__crontab(argc, argv);
    char *output = get_captured_output();

    check_string(output, expectedOutput);

    free(output);
    remove(filename);
}

static void test_cmd_crontab_write(void) {
    const char *filename = "test.txt";
    const char *input = "This is a test input.";

    const char *argv[] = {"test-tool", "crontab", filename, input};
    int argc = sizeof(argv) / sizeof(argv[0]);

    cmd__crontab(argc, argv);

    check_int(compare_file_contents(filename, "test.txt"), 1);

    remove(filename);
}

int cmd_main(int argc, const char **argv) {
    TEST(test_cmd_crontab_read, "Read from file");
    TEST(test_cmd_crontab_write, "Write to file");

    return test_done();
}
