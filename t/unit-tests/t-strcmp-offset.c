#include "test-lib.h"
#include "read-cache-ll.h"

static void check_strcmp_offset(const char *string1, const char *string2, int expect_result,  uintmax_t expect_offset)
{
	int result;
	size_t offset;

	result = strcmp_offset(string1, string2, &offset);

	/* Because different CRTs behave differently, only rely on signs of the result values. */
	result = (result < 0 ? -1 :
			  result > 0 ? 1 :
			  0);

	check_int(result, ==, expect_result);
	check_uint((uintmax_t)offset, ==, expect_offset);
}

int cmd_main(int argc, const char **argv) {
	TEST(check_strcmp_offset("abc", "abc", 0, 3), "strcmp_offset works");
	TEST(check_strcmp_offset("abc", "def", -1, 0), "strcmp_offset works");
	TEST(check_strcmp_offset("abc", "abz", -1, 2), "strcmp_offset works");
	TEST(check_strcmp_offset("abc", "abcdef", -1, 3), "strcmp_offset works");

	return test_done();
}
