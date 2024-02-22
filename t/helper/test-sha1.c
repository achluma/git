#include "test-tool.h"
#include "hash-ll.h"

int cmd__sha1_is_sha1dc(int argc UNUSED, const char **argv UNUSED)
{
#ifdef platform_SHA_IS_SHA1DC
	return 0;
#endif
	return 1;
}
