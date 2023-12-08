#include "test-lib.h"
#include "date.h"
#include "strbuf.h"

/* Reference time: 2009-08-30 19:20:00. */
#define GIT_TEST_DATE_NOW 1251660000

/* The time 1466000000 +0200 corresponds to Wed, 15 Jun 2016 16:13:20 +0200. */
static const char test_time[] = "1466000000 +0200";

enum prerequisites {
    	TIME_IS_64BIT = 1 << 0,
    	TIME_T_IS_64BIT = 1 << 1,
};

static int check_prereqs(unsigned int prereqs) {
	if (prereqs & TIME_IS_64BIT && sizeof(timestamp_t) != 8) {
        	test_skip("missing prerequisite TIME_IS_64BIT");
        	return 0;
    	}

    	if (prereqs & TIME_T_IS_64BIT && sizeof(time_t) != 8) {
        	test_skip("missing prerequisite TIME_T_IS_64BIT");
        	return 0;
    	}

	return 1;
}

static void set_TZ_env(const char *zone) {
	setenv("TZ", zone, 1);
	tzset();
}

static void check_relative_dates(int time_val, const char *expected_date) {
	struct strbuf buf = STRBUF_INIT;
	timestamp_t diff = GIT_TEST_DATE_NOW - time_val;

	show_date_relative(diff, &buf);
	check_str(buf.buf, expected_date);
	strbuf_release(&buf);
}

static void check_show_date(const char *format, const char *TIME, const char *expected, unsigned int prereqs, const char *zone) {
	struct date_mode mode = DATE_MODE_INIT;
	char *arg;
	timestamp_t t;
	int tz;

	if (!check_prereqs(prereqs))
		return;
	if (strcmp(zone, ""))
		set_TZ_env(zone);

	parse_date_format(format, &mode);
	t = parse_timestamp(TIME, &arg, 10);
	tz = atoi(arg);

	check_str(show_date(t, tz, &mode), expected);
	if (strcmp(zone, ""))
		set_TZ_env("UTC");

	date_mode_release(&mode);
}

static void check_parse_date(const char *given, const char *expected, const char *zone) {
	struct strbuf result = STRBUF_INIT;
	timestamp_t t;
	int tz;

	if (strcmp(zone, ""))
		set_TZ_env(zone);

	parse_date(given, &result);
	if (sscanf(result.buf, "%"PRItime" %d", &t, &tz) == 2)
		check_str(show_date(t, tz, DATE_MODE(ISO8601)), expected);
	else
		check_str("bad", expected);
	if (strcmp(zone, ""))
		set_TZ_env("UTC");

	strbuf_release(&result);
}

static void check_approxidate(const char *given, const char *expected) {
	timestamp_t t = approxidate(given);
	char *expected_with_offset = xstrfmt("%s +0000", expected);

	check_str(show_date(t, 0, DATE_MODE(ISO8601)), expected_with_offset);
	free(expected_with_offset);
}

static void check_date_format_human(int given, const char *expected) {
	timestamp_t diff = GIT_TEST_DATE_NOW - given;
	check_str(show_date(diff, 0, DATE_MODE(HUMAN)), expected);
}

int cmd_main(int argc, const char **argv) {
	set_TZ_env("UTC");
	setenv("GIT_TEST_DATE_NOW", "1251660000", 1);
	setenv("LANG", "C", 1);

	TEST(check_relative_dates(5, "5 seconds ago"),
		"Check relative date - 5 seconds ago works");
	TEST(check_relative_dates(300, "5 minutes ago"),
		"Check relative date - 5 minutes ago works");
	TEST(check_relative_dates(18000, "5 hours ago"),
		"Check relative date - 5 hours ago works");
	TEST(check_relative_dates(432000, "5 days ago"),
		"Check relative date - 5 days ago works");
	TEST(check_relative_dates(1728000, "3 weeks ago"),
		"Check relative date - 3 weeks ago works");
	TEST(check_relative_dates(13000000, "5 months ago"),
		"Check relative date - 5 months ago works");
	TEST(check_relative_dates(37500000, "1 year, 2 months ago"),
		"Check relative date - 1 year, 2 months ago works");
	TEST(check_relative_dates(55188000, "1 year, 9 months ago"),
		"Check relative date - 1 year, 9 months ago works");
	TEST(check_relative_dates(630000000, "20 years ago"),
		"Check relative date - 20 years ago works");
	TEST(check_relative_dates(31449600, "12 months ago"),
		"Check relative date - 12 months ago works");
	TEST(check_relative_dates(62985600, "2 years ago"),
		"Check relative date - 2 years ago works");

	TEST(check_show_date("iso8601", test_time, "2016-06-15 16:13:20 +0200", 0, ""),
		"Check_show_date - iso8601 works");
	TEST(check_show_date("iso8601-strict", test_time, "2016-06-15T16:13:20+02:00", 0, ""),
		"Check_show_date - iso8601-strict works");
	TEST(check_show_date("rfc2822", test_time, "Wed, 15 Jun 2016 16:13:20 +0200", 0, ""),
		"Check_show_date - rfc2822 works");
	TEST(check_show_date("short", test_time, "2016-06-15", 0, ""),
		"Check_show_date - short works");
	TEST(check_show_date("default", test_time, "Wed Jun 15 16:13:20 2016 +0200", 0, ""),
		"Check_show_date - default works");
	TEST(check_show_date("raw", test_time, test_time, 0, ""),
		"Check_show_date - raw works");
	TEST(check_show_date("unix", test_time, "1466000000", 0, ""),
		"Check_show_date - unix works");
	TEST(check_show_date("iso-local", test_time, "2016-06-15 14:13:20 +0000", 0, ""),
		"Check_show_date - iso-local works");
	TEST(check_show_date("raw-local", test_time, "1466000000 +0000", 0, ""),
		"Check_show_date - raw-local works");
	TEST(check_show_date("unix-local", test_time, "1466000000", 0, ""),
		"Check_show_date - unix-local works");

	TEST(check_show_date("format:%z", test_time, "+0200", 0, ""),
		"Check_show_date - format:%%z works");
	TEST(check_show_date("format-local:%z", test_time, "+0000", 0, ""),
		"Check_show_date - format-local:%%z works");
	TEST(check_show_date("format:%Z", test_time, "", 0, ""),
		"Check_show_date - format:%%Z works");
	TEST(check_show_date("format-local:%Z", test_time, "UTC", 0, ""),
		"Check_show_date - format-local:%%Z works");
	TEST(check_show_date("format:%%z", test_time, "%z", 0, ""),
		"Check_show_date - format:%%z works");
	TEST(check_show_date("format-local:%%z", test_time, "%z", 0, ""),
		"Check_show_date - format-local:%%z works");

	TEST(check_show_date("format:%Y-%m-%d %H:%M:%S", test_time, "2016-06-15 16:13:20", 0, ""),
		"Check_show_date - format:%%Y-%%m-%%d %%H:%%M:%%S works");

	TEST(check_show_date("format-local:%Y-%m-%d %H:%M:%S", test_time, "2016-06-15 09:13:20", 0, "EST5"),
		"Check_show_date - format-local:%%Y-%%m-%%d %%H:%%M:%%S works");

	TEST(check_show_date("format:%s", "123456789 +1234", "123456789", 0, ""),
		"Check_show_date - format:%%s positive offset works");
	TEST(check_show_date("format:%s", "123456789 -1234", "123456789", 0, ""),
		"Check_show_date - format:%%s negative offset works");
	TEST(check_show_date("format-local:%s", "123456789 -1234", "123456789", 0, ""),
		"Check_show_date - format-local:%%s negative offset works");

	/* Arbitrary time absurdly far in the future */
	TEST(check_show_date("iso", "5758122296 -0400", "2152-06-19 18:24:56 -0400", TIME_IS_64BIT | TIME_T_IS_64BIT, ""),
		"Check_show_date - iso works");
	TEST(check_show_date("iso-local", "5758122296 -0400", "2152-06-19 22:24:56 +0000", TIME_IS_64BIT | TIME_T_IS_64BIT, ""),
		"Check_show_date - iso-local works");

	TEST(check_parse_date("2000", "bad", ""),
		"check_parse_date - bad works");
	TEST(check_parse_date("2008-02", "bad", ""),
		"check_parse_date - bad works");
	TEST(check_parse_date("2008-02-14", "bad", ""),
		"check_parse_date - bad works");
	TEST(check_parse_date("2008-02-14 20:30:45", "2008-02-14 20:30:45 +0000", ""),
		"check_parse_date - 2008-02-14 20:30:45 +0000 works");
	TEST(check_parse_date("2008-02-14 20:30:45 -0500", "2008-02-14 20:30:45 -0500", ""),
		"check_parse_date - 2008-02-14 20:30:45 -0500 works");
	TEST(check_parse_date("2008.02.14 20:30:45 -0500", "2008-02-14 20:30:45 -0500", ""),
		"check_parse_date - 2008-02-14 20:30:45 -0500 works");
	TEST(check_parse_date("20080214T20:30:45", "2008-02-14 20:30:45 +0000", ""),
		"check_parse_date - 2008-02-14 20:30:45 +0000 works");
	TEST(check_parse_date("20080214T20:30", "2008-02-14 20:30:00 +0000", ""),
		"check_parse_date - 2008-02-14 20:30:00 +0000 works");
	TEST(check_parse_date("20080214T20", "2008-02-14 20:00:00 +0000", ""),
		"check_parse_date - 2008-02-14 20:00:00 +0000 works");
	TEST(check_parse_date("20080214T203045", "2008-02-14 20:30:45 +0000", ""),
		"check_parse_date - 2008-02-14 20:30:45 +0000 works");
	TEST(check_parse_date("20080214T2030", "2008-02-14 20:30:00 +0000", ""),
		"check_parse_date - 2008-02-14 20:30:00 +0000 works");
	TEST(check_parse_date("20080214T000000.20", "2008-02-14 00:00:00 +0000", ""),
		"check_parse_date - 2008-02-14 00:00:00 +0000 works");
	TEST(check_parse_date("20080214T00:00:00.20", "2008-02-14 00:00:00 +0000", ""),
		"check_parse_date - 2008-02-14 00:00:00 +0000 works");
	TEST(check_parse_date("20080214T203045-04:00", "2008-02-14 20:30:45 -0400", ""),
		"check_parse_date - 2008-02-14 20:30:45 -0400 works");

	TEST(check_parse_date("20080214T203045 -04:00", "2008-02-14 20:30:45 -0400", ""),
		"check_parse_date - 2008-02-14 20:30:45 -0400 works");
	TEST(check_parse_date("20080214T203045.019-04:00", "2008-02-14 20:30:45 -0400", ""),
		"check_parse_date - 2008-02-14 20:30:45 -0400 works");
	TEST(check_parse_date("2008-02-14 20:30:45.019-04:00", "2008-02-14 20:30:45 -0400", ""),
		"check_parse_date - 2008-02-14 20:30:45 -0400 works");
	TEST(check_parse_date("2008-02-14 20:30:45 -0015", "2008-02-14 20:30:45 -0015", ""),
		"check_parse_date - 2008-02-14 20:30:45 -0015 works");
	TEST(check_parse_date("2008-02-14 20:30:45 -5", "2008-02-14 20:30:45 +0000", ""),
		"check_parse_date - 2008-02-14 20:30:45 +0000 works");
	TEST(check_parse_date("2008-02-14 20:30:45 -5:", "2008-02-14 20:30:45 +0000", ""),
		"check_parse_date - 2008-02-14 20:30:45 +0000 works");
	TEST(check_parse_date("2008-02-14 20:30:45 -05", "2008-02-14 20:30:45 -0500", ""),
		"check_parse_date - 2008-02-14 20:30:45 -0500 works");
	TEST(check_parse_date("2008-02-14 20:30:45 -:30", "2008-02-14 20:30:45 +0000", ""),
		"check_parse_date - 2008-02-14 20:30:45 +0000 works");
	TEST(check_parse_date("2008-02-14 20:30:45 -05:00", "2008-02-14 20:30:45 -0500", ""),
		"check_parse_date - 2008-02-14 20:30:45 -0500 works");
	TEST(check_parse_date("2008-02-14 20:30:45", "2008-02-14 20:30:45 -0500", "EST5"),
		"check_parse_date - 2008-02-14 20:30:45 -0500 works");

	TEST(check_parse_date("Thu, 7 Apr 2005 15:14:13 -0700", "2005-04-07 15:14:13 -0700", ""),
		"check_parse_date - Thu, 7 Apr 2005 15:14:13 -0700 works");

	TEST(check_approxidate("now", "2009-08-30 19:20:00"),
		"check_approxidate - 2009-08-30 19:20:00 works");
	TEST(check_approxidate("5 seconds ago", "2009-08-30 19:19:55"),
		"check_approxidate - 5 seconds ago works");
	TEST(check_approxidate("10 minutes ago", "2009-08-30 19:10:00"),
		"check_approxidate - 10 minutes ago works");
	TEST(check_approxidate("yesterday", "2009-08-29 19:20:00"),
		"check_approxidate - yesterday works");
	TEST(check_approxidate("3 days ago", "2009-08-27 19:20:00"),
		"check_approxidate - 3 days ago works");
	TEST(check_approxidate("12:34:56.3 days ago", "2009-08-27 12:34:56"),
		"check_approxidate - 12:34:56.3 days ago works");
	TEST(check_approxidate("3 weeks ago", "2009-08-09 19:20:00"),
		"check_approxidate - 3 weeks ago works");
	TEST(check_approxidate("3 months ago", "2009-05-30 19:20:00"),
		"check_approxidate - 3 months ago works");
	TEST(check_approxidate("2 years 3 months ago", "2007-05-30 19:20:00"),
		"check_approxidate - 2 years 3 months ago works");

	TEST(check_approxidate("6am yesterday", "2009-08-29 06:00:00"),
		"check_approxidate - 6am yesterday works");
	TEST(check_approxidate("6pm yesterday", "2009-08-29 18:00:00"),
		"check_approxidate - 6pm yesterday works");
	TEST(check_approxidate("3:00", "2009-08-30 03:00:00"),
		"check_approxidate - 3:00 works");
	TEST(check_approxidate("15:00", "2009-08-30 15:00:00"),
		"check_approxidate - 15:00 works");
	TEST(check_approxidate("noon today", "2009-08-30 12:00:00"),
		"check_approxidate - noon today works");
	TEST(check_approxidate("noon yesterday", "2009-08-29 12:00:00"),
		"check_approxidate - noon yesterday works");
	TEST(check_approxidate("January 5th noon pm", "2009-01-05 12:00:00"),
		"check_approxidate - January 5th noon pm works");
	TEST(check_approxidate("10am noon", "2009-08-29 12:00:00"),
		"check_approxidate - 10am noon works");

	TEST(check_approxidate("last tuesday", "2009-08-25 19:20:00"),
		"check_approxidate - last tuesday works");
	TEST(check_approxidate("July 5th", "2009-07-05 19:20:00"),
		"check_approxidate - July 5th works");
	TEST(check_approxidate("06/05/2009", "2009-06-05 19:20:00"),
		"check_approxidate - 06/05/2009 works");
	TEST(check_approxidate("06.05.2009", "2009-05-06 19:20:00"),
		"check_approxidate - 06.05.2009 works");

	TEST(check_approxidate("Jun 6, 5AM", "2009-06-06 05:00:00"),
		"check_approxidate - Jun 6, 5AM works");
	TEST(check_approxidate("5AM Jun 6", "2009-06-06 05:00:00"),
		"check_approxidate - 5AM Jun 6 works");
	TEST(check_approxidate("6AM, June 7, 2009", "2009-06-07 06:00:00"),
		"check_approxidate - 6AM, June 7, 2009 works");

	TEST(check_approxidate("2008-12-01", "2008-12-01 19:20:00"),
		"check_approxidate - 2008-12-01 works");
	TEST(check_approxidate("2009-12-01", "2009-12-01 19:20:00"),
		"check_approxidate - 2009-12-01 works");

	TEST(check_date_format_human(18000, "5 hours ago"),
		"check_date_format_human - 5 hours ago works");
	TEST(check_date_format_human(432000, "Tue Aug 25 19:20"),
		"check_date_format_human - 5 days ago works");
	TEST(check_date_format_human(1728000, "Mon Aug 10 19:20"),
		"check_date_format_human - 3 weeks ago works");
	TEST(check_date_format_human(13000000, "Thu Apr 2 08:13"),
		"check_date_format_human - 5 months ago works");
	TEST(check_date_format_human(31449600, "Aug 31 2008"),
		"check_date_format_human - 12 months ago works");
	TEST(check_date_format_human(37500000, "Jun 22 2008"),
		"check_date_format_human - 1 year, 2 months ago works");
	TEST(check_date_format_human(55188000, "Dec 1 2007"),
		"check_date_format_human - 1 year, 9 months ago works");
	TEST(check_date_format_human(630000000, "Sep 13 1989"),
		"check_date_format_human - 20 years ago works");

	return test_done();
}
