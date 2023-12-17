#include "test-lib.h"

/* Custom character classification functions */

static int my_isdigit(int c)
{
    return c >= '0' && c <= '9';
}

static int my_isalpha(int c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

static int my_isspace(int c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

static int my_ispunct(int c)
{
    return !my_isalpha(c) && !my_isspace(c);
}

/* Test functions */

static void t_digit_type(void)
{
    int i;

    for (i = 0; i < 256; i++)
    {
        if (!my_isdigit(i))
            check_int(my_isdigit(i), ==, 0);
        else
            check_int(my_isdigit(i), ==, 1);
    }
}

static void t_alpha_type(void)
{
    int i;

    for (i = 0; i < 256; i++)
    {
        if (!my_isalpha(i))
            check_int(my_isalpha(i), ==, 0);
        else
            check_int(my_isalpha(i), ==, 1);
    }
}

static void t_space_type(void)
{
    int i;

    for (i = 0; i < 256; i++)
    {
        if (!my_isspace(i))
            check_int(my_isspace(i), ==, 0);
        else
            check_int(my_isspace(i), ==, 1);
    }
}

static void t_punct_type(void)
{
    int i;

    for (i = 0; i < 256; i++)
    {
        if (!my_ispunct(i))
            check_int(my_ispunct(i), ==, 0);
        else
            check_int(my_ispunct(i), ==, 1);
    }
}

int cmd_main(int argc, const char **argv)
{
    TEST(t_digit_type(), "Character is a digit");
    TEST(t_alpha_type(), "Character is alphabetic");
    TEST(t_space_type(), "Character is whitespace");
    TEST(t_punct_type(), "Character is punctuation");

    return test_done();
}
