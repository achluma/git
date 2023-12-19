#include "test-lib.h"

static int is_in(const char *s, int ch)
{
	/*
	 * We can't find NUL using strchr. Accept it as the first
	 * character in the spec -- there are no empty classes.
	 */
	if (ch == '\0')
		return ch == *s;
	if (*s == '\0')
		s++;
	return !!strchr(s, ch);
}

/* Macro to test a character type */
#define TEST_CTYPE_FUNC(func, string)			\
static void test_ctype_##func(void)				\
{								\
	int i;                                     	 	\
	for (i = 0; i < 256; i++)                 		\
		check_int(func(i), ==, is_in(string, i)); 	\
}

#define DIGIT "0123456789"
#define LOWER "abcdefghijklmnopqrstuvwxyz"
#define UPPER "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define PUNCT "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"
#define ASCII \
	"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f" \
	"\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f" \
	"\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f" \
	"\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f" \
	"\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f" \
	"\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f" \
	"\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f" \
	"\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b\x7c\x7d\x7e\x7f"
#define CNTRL \
	"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f" \
	"\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f" \
	"\x7f"

TEST_CTYPE_FUNC(isdigit, DIGIT)
TEST_CTYPE_FUNC(isspace, " \n\r\t")
TEST_CTYPE_FUNC(isalpha, LOWER UPPER)
TEST_CTYPE_FUNC(isalnum, LOWER UPPER DIGIT)
TEST_CTYPE_FUNC(is_glob_special, "*?[\\")
TEST_CTYPE_FUNC(is_regex_special, "$()*+.?[\\^{|")
TEST_CTYPE_FUNC(is_pathspec_magic, "!\"#%&',-/:;<=>@_`~")
TEST_CTYPE_FUNC(isascii, ASCII)
TEST_CTYPE_FUNC(islower, LOWER)
TEST_CTYPE_FUNC(isupper, UPPER)
TEST_CTYPE_FUNC(iscntrl, CNTRL)
TEST_CTYPE_FUNC(ispunct, PUNCT)
TEST_CTYPE_FUNC(isxdigit, DIGIT "abcdefABCDEF")
TEST_CTYPE_FUNC(isprint, LOWER UPPER DIGIT PUNCT " ")

int cmd_main(int argc, const char **argv) {
	/* Run all character type tests */
	TEST(test_ctype_isspace(), "isspace() works as we expect");
	TEST(test_ctype_isdigit(), "isdigit() works as we expect");
	TEST(test_ctype_isalpha(), "isalpha() works as we expect");
	TEST(test_ctype_isalnum(), "isalnum() works as we expect");
	TEST(test_ctype_is_glob_special(), "is_glob_special() works as we expect");
	TEST(test_ctype_is_regex_special(), "is_regex_special() works as we expect");
	TEST(test_ctype_is_pathspec_magic(), "is_pathspec_magic() works as we expect");
	TEST(test_ctype_isascii(), "isascii() works as we expect");
	TEST(test_ctype_islower(), "islower() works as we expect");
	TEST(test_ctype_isupper(), "isupper() works as we expect");
	TEST(test_ctype_iscntrl(), "iscntrl() works as we expect");
	TEST(test_ctype_ispunct(), "ispunct() works as we expect");
	TEST(test_ctype_isxdigit(), "isxdigit() works as we expect");
	TEST(test_ctype_isprint(), "isprint() works as we expect");

	return test_done();
}
