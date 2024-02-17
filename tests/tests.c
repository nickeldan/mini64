#include <string.h>

#include <scrutiny/scrutiny.h>

#include <mini64.h>

static const unsigned char DATA[9] = {0x31, 0x6c, 0x7f, 0x19, 0xe3, 0xf0, 0xc0, 0x87, 0x82};

static void
test_encode(void)
{
    char string[20];

    mini64_encode(DATA, 9, string);
    SCR_ASSERT_LT(strnlen(string, sizeof(string)), sizeof(string));
    SCR_ASSERT_STR_EQ(string, "MWx/GePwwIeC");

    mini64_encode(DATA, 8, string);
    SCR_ASSERT_LT(strnlen(string, sizeof(string)), sizeof(string));
    SCR_ASSERT_STR_EQ(string, "MWx/GePwwIc=");

    mini64_encode(DATA, 7, string);
    SCR_ASSERT_LT(strnlen(string, sizeof(string)), sizeof(string));
    SCR_ASSERT_STR_EQ(string, "MWx/GePwwA==");
}

static void
test_decode(void)
{
    size_t len = 0;
    unsigned char data[20];

    memset(data, 0, sizeof(data));
    SCR_ASSERT_EQ(mini64_decode("MWx/GePwwIeC", strlen("MWx/GePwwIeC"), data, &len), 0);
    SCR_ASSERT_EQ(len, 9);
    SCR_ASSERT_MEM_EQ(data, DATA, 9);

    memset(data, 0, sizeof(data));
    SCR_ASSERT_EQ(mini64_decode("MWx/GePwwIc=", strlen("MWx/GePwwIc="), data, &len), 0);
    SCR_ASSERT_EQ(len, 8);
    SCR_ASSERT_MEM_EQ(data, DATA, 8);

    memset(data, 0, sizeof(data));
    SCR_ASSERT_EQ(mini64_decode("MWx/GePwwA==", strlen("MWx/GePwwA=="), data, &len), 0);
    SCR_ASSERT_EQ(len, 7);
    SCR_ASSERT_MEM_EQ(data, DATA, 7);
}

int
main()
{
    scrGroup group;

    group = scrGroupCreate(NULL, NULL);
    scrGroupAddTest(group, "encode", test_encode, NULL);
    scrGroupAddTest(group, "decode", test_decode, NULL);

    return scrRun(NULL, NULL);
}