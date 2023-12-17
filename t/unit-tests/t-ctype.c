#include "test-lib.h"

static int is_in(const char *s, int ch) {
	return !!strchr(s, ch);
}

struct ctype {
	const char *name;
	const char *expect;
	int actual[256];
};

static void test_ctype(const struct ctype *class) {
	for (int i = 0; i < 256; i++) {
		int expect = is_in(class->expect, i);
		int actual = class->actual[i];
		int res = test_assert(TEST_LOCATION(), class->name, actual == expect);
		if (!res)
			test_msg("%s classifies char %d (0x%02x) wrongly", class->name, i, i);
	}
}

#define APPLY16(f, n) \
	f(n + 0x0), f(n + 0x1), f(n + 0x2), f(n + 0x3), \
	f(n + 0x4), f(n + 0x5), f(n + 0x6), f(n + 0x7), \
	f(n + 0x8), f(n + 0x9), f(n + 0xa), f(n + 0xb), \
	f(n + 0xc), f(n + 0xd), f(n + 0xe), f(n + 0xf)
#define APPLY256(f) \
	APPLY16(f, 0x00), APPLY16(f, 0x10), APPLY16(f, 0x20), APPLY16(f, 0x30), \
	APPLY16(f, 0x40), APPLY16(f, 0x50), APPLY16(f, 0x60), APPLY16(f, 0x70), \
	APPLY16(f, 0x80), APPLY16(f, 0x90), APPLY16(f, 0xa0), APPLY16(f, 0xb0), \
	APPLY16(f, 0xc0), APPLY16(f, 0xd0), APPLY16(f, 0xe0), APPLY16(f, 0xf0),

#define CTYPE(name, expect) { #name, expect, { APPLY256(name) } }

int cmd_main(int argc, const char **argv) {
	struct ctype classes[] = {
		CTYPE(isdigit, "0123456789"),
		CTYPE(isspace, " \n\r\t"),
		CTYPE(isalpha, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"),
		CTYPE(isalnum, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"),
		CTYPE(is_glob_special, "*?[\\"),
		CTYPE(is_regex_special, "$()*+.?[\\^{|"),
		CTYPE(is_pathspec_magic, "!\"#%&',-/:;<=>@_`~"),
		CTYPE(isascii, "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
		                "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f"
		                "\x7f"),
		CTYPE(islower, "abcdefghijklmnopqrstuvwxyz"),
		CTYPE(isupper, "ABCDEFGHIJKLMNOPQRSTUVWXYZ"),
		CTYPE(iscntrl, "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x0b\x0c\x0e\x0f"
		               "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f"
		               "\x7f"),
		CTYPE(ispunct, "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"),
		CTYPE(isxdigit, "0123456789abcdefABCDEF"),
		CTYPE(isprint, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
		                "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~ "),
	};

	for (int i = 0; i < sizeof(classes) / sizeof(classes[0]); i++)
		TEST(test_ctype(&classes[i]), "%s works", classes[i].name);

	return test_done();
}
