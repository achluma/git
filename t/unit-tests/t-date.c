#include "test-lib.h"
#include "date.h"
#include "trace.h"

static void test_show_relative_dates(void)
{
    const char *dates[] = {"0", "100", NULL};

    for (const char **argv = dates; *argv; argv++) {
        struct strbuf buf = STRBUF_INIT;
        time_t t = atoi(*argv);
        show_date_relative(t, &buf);
        check_str(*argv, buf.buf);
        strbuf_release(&buf);
    }
}

static void test_show_human_dates(void)
{
    const char *dates[] = {"0", "100", NULL};

    for (const char **argv = dates; *argv; argv++) {
        time_t t = atoi(*argv);
        const char *output = show_date(t, 0, DATE_MODE(HUMAN));
        check_str(*argv, output);
        free((void *)output);
    }
}

static void test_show_dates(void) {
    const char *dates[] = {"0", "100", NULL};
    const char *format = "iso";

    for (const char **argv = dates; *argv; argv++) {
        struct date_mode mode = DATE_MODE_INIT;
        char *arg;
        timestamp_t t;
        int tz;
        const char *output;

        parse_date_format(format, &mode);

        t = parse_timestamp(*argv, &arg, 10);
        while (*arg == ' ')
            arg++;
        tz = atoi(arg);

        output = show_date(t, tz, &mode);
        check_str(*argv, output);
        free((void *)output);

        date_mode_release(&mode);
    }
}

static void test_parse_dates(void) {
    const char *dates[] = {"invalid_date", "2023-10-17 10:00:00 +0200", NULL};

    for (const char **argv = dates; *argv; argv++) {
        struct strbuf result = STRBUF_INIT;
        timestamp_t t;
        int tz;
	const char *output;

        parse_date(*argv, &result);
        if (sscanf(result.buf, "%" PRItime " %d", &t, &tz) == 2) {
            output = show_date(t, tz, DATE_MODE(ISO8601));
            check_str(*argv, output);
            free((void *)output);
        } else {
            check_str(*argv, "bad");
        }

        strbuf_release(&result);
    }
}

static void test_parse_approxidate(void) {
    const char *dates[] = {"yesterday", "next week", NULL};

    for (const char **argv = dates; *argv; argv++) {
        timestamp_t t = approxidate(*argv);
        const char *output = show_date(t, 0, DATE_MODE(ISO8601));
        check_str(*argv, output);
        free((void *)output);
    }
}

int cmd_main(int argc, const char **argv) {

TEST(test_show_relative_dates(), "Test show_date_relative");
TEST(test_show_human_dates(), "Test show_human_dates");
TEST(test_show_dates(), "Test show_dates");
TEST(test_parse_dates(), "Test parse_dates");
TEST(test_parse_approxidate(), "Test parse_approxidate");
    return test_done();;
}
