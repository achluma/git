#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "test-lib.h"



/* Custom function to check if a character is in a string */
static int is_in(const char *str, int c) {
    return strchr(str, c) != NULL;
}

/* Macro to test a character type */
#define TEST_CHARACTER_TYPE(func, chars) {		\
    const char *test_chars = chars;             	\
    int i;                                      	\
    for (i = 0; i < 256; i++) {                 	\
        int result_stdlib = func(i);            	\
        int result_custom = is_in(test_chars, i); 	\
        check_int(result_stdlib, ==, result_custom); 	\
    }                                           	\
} 

static void run_character_type_tests(void) {
TEST_CHARACTER_TYPE(isdigit, "0123456789");
TEST_CHARACTER_TYPE(isspace, " \n\r\t");
TEST_CHARACTER_TYPE(isalpha, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
TEST_CHARACTER_TYPE(isalnum, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
TEST_CHARACTER_TYPE(is_glob_special, "*?[\\");
TEST_CHARACTER_TYPE(is_regex_special, "$()*+.?[\\^{|");
TEST_CHARACTER_TYPE(is_pathspec_magic, "!\"#%&',-/:;<=>@_`~");
TEST_CHARACTER_TYPE(isascii, "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2A\x2B\x2C\x2D\x2E\x2F\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3A\x3B\x3C\x3D\x3E\x3F\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\x5C\x5D\x5E\x5F\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6A\x6B\x6C\x6D\x6E\x6F\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7A\x7B\x7C\x7D\x7E\x7F");
TEST_CHARACTER_TYPE(islower, "abcdefghijklmnopqrstuvwxyz");
TEST_CHARACTER_TYPE(isupper, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
TEST_CHARACTER_TYPE(iscntrl, "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F\x7F");
TEST_CHARACTER_TYPE(ispunct, "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}");
TEST_CHARACTER_TYPE(isxdigit, "0123456789abcdefABCDEF");
TEST_CHARACTER_TYPE(isprint, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~");
}

int cmd_main(int argc, const char **argv) {
    /* Run all character type tests */
    TEST(run_character_type_tests(), "All character type tests");

    return test_done();
}
