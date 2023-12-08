#include "test-lib.h"
#include "date.h"

static void test_show_relative_dates(void) {
    const char *dates[] = { "0", "100", NULL };

    for (const char **argv = dates; *argv; argv++) {
        check_int(show_relative_dates((const char *[]){ *argv, NULL }), 0);
    }
}

static void test_show_human_dates(void) {
    const char *dates[] = { "0", "100", NULL };

    for (const char **argv = dates; *argv; argv++) {
        check_int(show_human_dates((const char *[]){ *argv, NULL }), 0);
    }
}

static void test_show_dates(void) {
    const char *dates[] = { "0", "100", NULL };
    const char *format = "iso";

    for (const char **argv = dates; *argv; argv++) {
        check_int(show_dates((const char *[]){ *argv, NULL }, format), 0);
    }
}

static void test_parse_dates(void) {
    const char *dates[] = { "invalid_date", "2023-10-17 10:00:00 +0200", NULL };

    for (const char **argv = dates; *argv; argv++) {
        check_int(parse_dates((const char *[]){ *argv, NULL }), 0);
    }
}

static void test_parse_approxidate(void) {
    const char *dates[] = { "yesterday", "next week", NULL };

    for (const char **argv = dates; *argv; argv++) {
        check_int(parse_approxidate((const char *[]){ *argv, NULL }), 0);
    }
}

static void test_parse_approx_timestamp(void) {
    const char *dates[] = { "yesterday", "next week", NULL };

    for (const char **argv = dates; *argv; argv++) {
        check_int(parse_approx_timestamp((const char *[]){ *argv, NULL }), 0);
    }
}

int main(int argc UNUSED, const char **argv UNUSED) {
    test_start("date unit tests");

    TEST(test_show_relative_dates, "Test show_date_relative");
    TEST(test_show_human_dates, "Test show_date in human-readable format");
    TEST(test_show_dates, "Test show_date with custom format");
    TEST(test_parse_dates, "Test date parsing");
    TEST(test_parse_approxidate, "Test approxidate");
    TEST(test_parse_approx_timestamp, "Test approxidate for timestamps");

    return test_finish();
}
