#include "test-lib.h"
#include "date.h"
#include "trace.h"

// Set the reference time: 2009-08-30 19:20:00
#define GIT_TEST_DATE_NOW 1251660000

static void check_relative_dates(time_t time_val, const char *expected_date) {
	time_t diff = GIT_TEST_DATE_NOW - time_val;
	char *captured_string;
	struct strbuf buf = STRBUF_INIT;
	show_date_relative(diff, &buf);
	captured_string = buf.buf;
	check_str(captured_string, expected_date);
	strbuf_release(&buf);
}

int cmd_main(int argc, const char **argv) {
	TEST(check_relative_dates(5, "5 seconds ago"), "Check relative date - 5 seconds ago  works");
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


	return test_done();
}
