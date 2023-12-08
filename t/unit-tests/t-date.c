#include "test-lib.h"
#include "date.h"
#include "trace.h"

// Set the reference time: 2009-08-30 19:20:00
#define GIT_TEST_DATE_NOW 1251660000

static void format_relative_date(int diff, char *expected, size_t size) {
	if (diff < 60) {
		snprintf(expected, size, "%d seconds ago", diff);
	} else if (diff < 3600) {
		snprintf(expected, size, "%d minutes ago", diff / 60);
	} else if (diff < 86400) {
		snprintf(expected, size, "%d hours ago", diff / 3600);
	} else if (diff < 604800) {
		snprintf(expected, size, "%d days ago", diff / 86400);
	} else if (diff < 2592000) {
		snprintf(expected, size, "%d weeks ago", diff / 604800);
	} else if (diff < 31536000) {
		int months = diff / 2592000;
		int days_left = diff % 2592000;
		snprintf(expected, size, "%d months, %d days ago", months, days_left / 86400);
	} else {
		int years = diff / 31536000;
		int months_left = (diff % 31536000) / 2592000;
		snprintf(expected, size, "%d years, %d months ago", years, months_left);
	}
}

static void check_relative_dates(time_t time_val, const char *expected_date) {
	int diff = GIT_TEST_DATE_NOW - time_val;
	struct strbuf buf = STRBUF_INIT;
	char expected[128];
	format_relative_date(diff, expected, sizeof(expected));
	show_date_relative(time_val, &buf);
	check_str(buf.buf, expected);
	strbuf_release(&buf);
}

static void test_check_relative_dates(void) {
	check_relative_dates(5, "5 seconds ago");
	check_relative_dates(300, "5 minutes ago");
	check_relative_dates(18000, "5 hours ago");
	check_relative_dates(432000, "5 days ago");
	check_relative_dates(1728000, "3 weeks ago");
	check_relative_dates(13000000, "5 months ago");
	check_relative_dates(37500000, "1 year, 2 months ago");
	check_relative_dates(55188000, "1 year, 9 months ago");
	check_relative_dates(630000000, "20 years ago");
	check_relative_dates(31449600, "12 months ago");
	check_relative_dates(62985600, "2 years ago");
}

int cmd_main(int argc, const char **argv) {
	TEST(test_check_relative_dates(), "Check relative date works");

	return test_done();
}
