#include "test-lib.h"
#include "date.h"
#include "strbuf.h"

static void check_relative_dates(int time_val, const char *expected_date) {
	struct timeval now;
	timestamp_t diff;
	struct strbuf buf = STRBUF_INIT;
	const char *x;

	x = getenv("GIT_TEST_DATE_NOW");
	now.tv_sec = atoi(x);
	now.tv_usec = 0;

	diff = now.tv_sec - time_val;
	show_date_relative(diff, &buf);
	check_str(buf.buf, expected_date);
	strbuf_release(&buf);
}

static void check_show_date(const char *format, const char *TIME, const char *expected) {
	struct date_mode mode = DATE_MODE_INIT;
	char *arg;
	timestamp_t t;
	int tz;

	parse_date_format(format, &mode);
	t = parse_timestamp(TIME, &arg, 10);
	tz = atoi(arg);
	check_str(show_date(t, tz, &mode), expected);
	date_mode_release(&mode);
}

int cmd_main(int argc, const char **argv) {
	setenv("GIT_TEST_DATE_NOW", "1251660000", 1);
	TEST(check_relative_dates(5, "5 seconds ago"), "Check relative date - 5 seconds ago works");
	TEST(check_relative_dates(300, "5 minutes ago"), "Check relative date - 5 minutes ago works");
	TEST(check_relative_dates(18000, "5 hours ago"), "Check relative date - 5 hours ago works");
	TEST(check_relative_dates(432000, "5 days ago"), "Check relative date - 5 days ago works");
	TEST(check_relative_dates(1728000, "3 weeks ago"), "Check relative date - 3 weeks ago works");
	TEST(check_relative_dates(13000000, "5 months ago"), "Check relative date - 5 months ago works");
	TEST(check_relative_dates(37500000, "1 year, 2 months ago"), "Check relative date - 1 year, 2 months ago works");
	TEST(check_relative_dates(55188000, "1 year, 9 months ago"), "Check relative date - 1 year, 9 months ago works");
	TEST(check_relative_dates(630000000, "20 years ago"), "Check relative date - 20 years ago works");
	TEST(check_relative_dates(31449600, "12 months ago"), "Check relative date - 12 months ago works");
	TEST(check_relative_dates(62985600, "2 years ago"), "Check relative date - 2 years ago works");

	TEST(check_show_date("iso8601", "1466000000 +0200", "2016-06-15 16:13:20 +0200"), "Check_show_date - iso8601 works");
	TEST(check_show_date("iso8601", "1466000000 +0200", "2016-06-15 16:13:20 +0200"), "Check_show_date - iso8601 works");
	TEST(check_show_date("iso8601-strict", "1466000000 +0200", "2016-06-15T16:13:20+02:00"), "Check_show_date - iso8601-strict works");
	TEST(check_show_date("rfc2822", "1466000000 +0200", "Wed, 15 Jun 2016 16:13:20 +0200"), "Check_show_date - rfc2822 works");
	TEST(check_show_date("short", "1466000000 +0200", "2016-06-15"), "Check_show_date - short works");
	TEST(check_show_date("default", "1466000000 +0200", "Wed Jun 15 16:13:20 2016 +0200"), "Check_show_date - default works");
	TEST(check_show_date("raw", "1466000000 +0200", "1466000000 +0200"), "Check_show_date - raw works");
	TEST(check_show_date("unix", "1466000000 +0200", "1466000000"), "Check_show_date - unix works");
	TEST(check_show_date("iso-local", "1466000000 +0200", "2016-06-15 14:13:20 +0000"), "Check_show_date - iso-local works");
	TEST(check_show_date("raw-local", "1466000000 +0200", "1466000000 +0000"), "Check_show_date - raw-local works");
	TEST(check_show_date("unix-local", "1466000000 +0200", "1466000000"), "Check_show_date - unix-local works");

	TEST(check_show_date("format:%%z", "1466000000 +0200", "+0200"), "Check_show_date - format:%%z works");
	TEST(check_show_date("format-local:%z", "1466000000 +0200", "+0000"), "Check_show_date - format-local:%%z works");
	TEST(check_show_date("format:%Z", "1466000000 +0200", ""), "Check_show_date - format:%%Z works");
	TEST(check_show_date("format-local:%Z", "1466000000 +0200", "UTC"), "Check_show_date - format-local:%%Z works");
	TEST(check_show_date("format:%%z", "1466000000 +0200", "%z"), "Check_show_date - format:%%z works");
	TEST(check_show_date("format-local:%%z", "1466000000 +0200", "%z"), "Check_show_date - format-local:%%z works");

	TEST(check_show_date("format:%Y-%m-%d %H:%M:%S", "1466000000 +0200", "2016-06-15 16:13:20"), "Check_show_date - format:%%Y-%%m-%%d %%H:%%M:%%S works");

	TEST(check_show_date("format:%s", "123456789 +1234", "123456789"), "Check_show_date - format:%%s positive offset");
	TEST(check_show_date("format:%s", "123456789 -1234", "123456789"), "Check_show_date - format:%%s negative offset");
	TEST(check_show_date("format-local:%s", "123456789 -1234", "123456789"), "Check_show_date - format-local:%%s negative offset");

	return test_done();
}
