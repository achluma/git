#include <stdio.h>
#include <string.h>
#include "test-lib.h"
#include "git-compat-util.h"
#include "hash.h"
#include "hex.h"

static void test_get_hash_hex_algop(void)
{
    struct git_hash_algo algo;
    algo.rawsz = GIT_MAX_RAWSZ;
     int result;

    unsigned char hash[GIT_MAX_RAWSZ];
    const char *hex = "0123456789abcdef";

    result = get_hash_hex_algop(hex, hash, &algo);

    check_int(result, ==, 0);
    check_mem(hash, hex, algo.rawsz);
}

int cmd_main(int argc, const char **argv)
{
    TEST(test_get_hash_hex_algop, "Test get_hash_hex_algop");
    return test_done();
}
