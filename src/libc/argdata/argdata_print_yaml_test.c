// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/argdata.h>

#include <argdata.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <testing.h>

#define TEST_OBJECT(obj, out)                                   \
  do {                                                          \
    /* Write YAML to output buffer. */                          \
    char outbuf[sizeof(out) + 5];                               \
    FILE *fp = fmemopen(outbuf, sizeof(outbuf), "w");           \
    ASSERT_NE(NULL, fp);                                        \
    ASSERT_EQ(0, argdata_print_yaml(obj, fp));                  \
    ASSERT_EQ(sizeof(outbuf) - 1, ftello(fp));                  \
    ASSERT_EQ(0, fclose(fp));                                   \
                                                                \
    /* Compare against expected output. */                      \
    ASSERT_ARREQ("---\n" out "\n", outbuf, sizeof(outbuf) - 1); \
  } while (0)

TEST(argdata_print_yaml, buffer) {
#define TEST_BUFFER(in, out)                      \
  do {                                            \
    /* Prepare argument data input. */            \
    argdata_t ad;                                 \
    argdata_init_binary(&ad, in, sizeof(in) - 1); \
    TEST_OBJECT(&ad, out);                        \
  } while (0)
  TEST_BUFFER("", "!!null \"null\"");

  // Binary data.
  TEST_BUFFER("\x01", "!!binary \"\"");
  TEST_BUFFER("\x01H", "!!binary \"SA==\"");
  TEST_BUFFER("\x01He", "!!binary \"SGU=\"");
  TEST_BUFFER("\x01Hel", "!!binary \"SGVs\"");
  TEST_BUFFER("\x01Hell", "!!binary \"SGVsbA==\"");
  TEST_BUFFER("\x01Hello", "!!binary \"SGVsbG8=\"");
  TEST_BUFFER("\x01Hello, world", "!!binary \"SGVsbG8sIHdvcmxk\"");

  // Boolean values. These should have a one-byte if true.
  TEST_BUFFER("\x02", "!!bool \"false\"");
  TEST_BUFFER("\x02Hello, world", "!!null \"null\"");
  TEST_BUFFER("\x02\x00", "!!null \"null\"");
  TEST_BUFFER("\x02\x01", "!!bool \"true\"");
  TEST_BUFFER("\x02\x02", "!!null \"null\"");

  // File descriptors. These are fixed size.
  TEST_BUFFER("\x03", "!!null \"null\"");
  TEST_BUFFER("\x03\x00\x00\x00", "!!null \"null\"");
  TEST_BUFFER("\x03\x00\x00\x00\x00", "!!fd \"0\"");
  TEST_BUFFER("\x03\x12\x34\x56\x78", "!!fd \"305419896\"");
  TEST_BUFFER("\x03\xff\xff\xff\xff", "!!null \"null\"");

  // Integer values.
  TEST_BUFFER("\x05\xff\x7f\xff\xff\xff\xff\xff\xff\xff", "!!null \"null\"");
  TEST_BUFFER("\x05\x80\x00\x00\x00\x00\x00\x00\x00",
              "!!int \"-9223372036854775808\"");
  TEST_BUFFER("\x05\xfe\xff\xff\xff\xff\xff\xff\xff",
              "!!int \"-72057594037927937\"");
  TEST_BUFFER("\x05\xff\x00\x00\x00\x00\x00\x00\x00",
              "!!int \"-72057594037927936\"");
  TEST_BUFFER("\x05\xff\x7f\xff\xff\xff\xff\xff\xff",
              "!!int \"-36028797018963969\"");
  TEST_BUFFER("\x05\x80\x00\x00\x00\x00\x00\x00",
              "!!int \"-36028797018963968\"");
  TEST_BUFFER("\x05\xfe\xff\xff\xff\xff\xff\xff", "!!int \"-281474976710657\"");
  TEST_BUFFER("\x05\xff\x00\x00\x00\x00\x00\x00", "!!int \"-281474976710656\"");
  TEST_BUFFER("\x05\xff\x7f\xff\xff\xff\xff\xff", "!!int \"-140737488355329\"");
  TEST_BUFFER("\x05\x80\x00\x00\x00\x00\x00", "!!int \"-140737488355328\"");
  TEST_BUFFER("\x05\xfe\xff\xff\xff\xff\xff", "!!int \"-1099511627777\"");
  TEST_BUFFER("\x05\xff\x00\x00\x00\x00\x00", "!!int \"-1099511627776\"");
  TEST_BUFFER("\x05\xff\x7f\xff\xff\xff\xff", "!!int \"-549755813889\"");
  TEST_BUFFER("\x05\x80\x00\x00\x00\x00", "!!int \"-549755813888\"");
  TEST_BUFFER("\x05\xfe\xff\xff\xff\xff", "!!int \"-4294967297\"");
  TEST_BUFFER("\x05\xff\x00\x00\x00\x00", "!!int \"-4294967296\"");
  TEST_BUFFER("\x05\xff\x7f\xff\xff\xff", "!!int \"-2147483649\"");
  TEST_BUFFER("\x05\x80\x00\x00\x00", "!!int \"-2147483648\"");
  TEST_BUFFER("\x05\xfe\xff\xff\xff", "!!int \"-16777217\"");
  TEST_BUFFER("\x05\xff\x00\x00\x00", "!!int \"-16777216\"");
  TEST_BUFFER("\x05\xff\x7f\xff\xff", "!!int \"-8388609\"");
  TEST_BUFFER("\x05\x80\x00\x00", "!!int \"-8388608\"");
  TEST_BUFFER("\x05\xfe\xff\xff", "!!int \"-65537\"");
  TEST_BUFFER("\x05\xff\x00\x00", "!!int \"-65536\"");
  TEST_BUFFER("\x05\xff\x7f\xff", "!!int \"-32769\"");
  TEST_BUFFER("\x05\x80\x00", "!!int \"-32768\"");
  TEST_BUFFER("\x05\xfe\xff", "!!int \"-257\"");
  TEST_BUFFER("\x05\xff\x00", "!!int \"-256\"");
  TEST_BUFFER("\x05\xff\x7f", "!!int \"-129\"");
  TEST_BUFFER("\x05\x80", "!!int \"-128\"");
  TEST_BUFFER("\x05\xff", "!!int \"-1\"");
  TEST_BUFFER("\x05", "!!int \"0\"");
  TEST_BUFFER("\x05\x01", "!!int \"1\"");
  TEST_BUFFER("\x05\x7f", "!!int \"127\"");
  TEST_BUFFER("\x05\x00\x80", "!!int \"128\"");
  TEST_BUFFER("\x05\x00\xff", "!!int \"255\"");
  TEST_BUFFER("\x05\x01\x00", "!!int \"256\"");
  TEST_BUFFER("\x05\x7f\xff", "!!int \"32767\"");
  TEST_BUFFER("\x05\x00\x80\x00", "!!int \"32768\"");
  TEST_BUFFER("\x05\x00\xff\xff", "!!int \"65535\"");
  TEST_BUFFER("\x05\x01\x00\x00", "!!int \"65536\"");
  TEST_BUFFER("\x05\x7f\xff\xff", "!!int \"8388607\"");
  TEST_BUFFER("\x05\x00\x80\x00\x00", "!!int \"8388608\"");
  TEST_BUFFER("\x05\x00\xff\xff\xff", "!!int \"16777215\"");
  TEST_BUFFER("\x05\x01\x00\x00\x00", "!!int \"16777216\"");
  TEST_BUFFER("\x05\x7f\xff\xff\xff", "!!int \"2147483647\"");
  TEST_BUFFER("\x05\x00\x80\x00\x00\x00", "!!int \"2147483648\"");
  TEST_BUFFER("\x05\x00\xff\xff\xff\xff", "!!int \"4294967295\"");
  TEST_BUFFER("\x05\x01\x00\x00\x00\x00", "!!int \"4294967296\"");
  TEST_BUFFER("\x05\x7f\xff\xff\xff\xff", "!!int \"549755813887\"");
  TEST_BUFFER("\x05\x00\x80\x00\x00\x00\x00", "!!int \"549755813888\"");
  TEST_BUFFER("\x05\x00\xff\xff\xff\xff\xff", "!!int \"1099511627775\"");
  TEST_BUFFER("\x05\x01\x00\x00\x00\x00\x00", "!!int \"1099511627776\"");
  TEST_BUFFER("\x05\x7f\xff\xff\xff\xff\xff", "!!int \"140737488355327\"");
  TEST_BUFFER("\x05\x00\x80\x00\x00\x00\x00\x00", "!!int \"140737488355328\"");
  TEST_BUFFER("\x05\x00\xff\xff\xff\xff\xff\xff", "!!int \"281474976710655\"");
  TEST_BUFFER("\x05\x01\x00\x00\x00\x00\x00\x00", "!!int \"281474976710656\"");
  TEST_BUFFER("\x05\x7f\xff\xff\xff\xff\xff\xff",
              "!!int \"36028797018963967\"");
  TEST_BUFFER("\x05\x00\x80\x00\x00\x00\x00\x00\x00",
              "!!int \"36028797018963968\"");
  TEST_BUFFER("\x05\x00\xff\xff\xff\xff\xff\xff\xff",
              "!!int \"72057594037927935\"");
  TEST_BUFFER("\x05\x01\x00\x00\x00\x00\x00\x00\x00",
              "!!int \"72057594037927936\"");
  TEST_BUFFER("\x05\x7f\xff\xff\xff\xff\xff\xff\xff",
              "!!int \"9223372036854775807\"");
  TEST_BUFFER("\x05\x00\x80\x00\x00\x00\x00\x00\x00\x00",
              "!!int \"9223372036854775808\"");
  TEST_BUFFER("\x05\x00\xff\xff\xff\xff\xff\xff\xff\xff",
              "!!int \"18446744073709551615\"");
  TEST_BUFFER("\x05\x01\x00\x00\x00\x00\x00\x00\x00\x00", "!!null \"null\"");

  // Strings.
  // TODO(edje): Add tests for special characters and bad encoding.
  TEST_BUFFER("\x08", "!!null \"null\"");
  TEST_BUFFER("\x08Hello", "!!null \"null\"");
  TEST_BUFFER("\x08Hello\x00", "!!str \"Hello\"");

  // Maps.
  // TODO(ed): Add tests for multi-byte lengths.
  TEST_BUFFER("\x06", "!!map {}");
  TEST_BUFFER(
      "\x06"
      "\x87\x08Hello\x00\x87\x08World\x00"
      "\x81\x02\x82\x02\x01"
      "\x85\x05\x01\x80\x29\x15\x84\x05\x58\xe5\xd9"
      "\x80\x83\x06\x80\x80",
      "!!map {\n"
      "  ? !!str \"Hello\"\n"
      "  : !!str \"World\",\n"
      "  ? !!bool \"false\"\n"
      "  : !!bool \"true\",\n"
      "  ? !!int \"25176341\"\n"
      "  : !!int \"5826009\",\n"
      "  ? !!null \"null\"\n"
      "  : !!map {\n"
      "    ? !!null \"null\"\n"
      "    : !!null \"null\",\n"
      "  },\n"
      "}");

  // Sequences.
  TEST_BUFFER("\x07", "!!seq []");
  TEST_BUFFER(
      "\x07"
      "\x81\x02"
      "\x82\x02\x01"
      "\x80"
      "\x87\x08Hello\x00"
      "\x81\x06"
      "\x81\x07",
      "!!seq [\n"
      "  !!bool \"false\",\n"
      "  !!bool \"true\",\n"
      "  !!null \"null\",\n"
      "  !!str \"Hello\",\n"
      "  !!map {},\n"
      "  !!seq [],\n"
      "]");
#undef TEST_BUFFER
}

TEST(argdata_print_yaml, binary) {
  argdata_t *ad = argdata_create_binary(NULL, 0);
  TEST_OBJECT(ad, "!!binary \"\"");
  argdata_free(ad);

  ad = argdata_create_binary("This is an example string", 25);
  TEST_OBJECT(ad, "!!binary \"VGhpcyBpcyBhbiBleGFtcGxlIHN0cmluZw==\"");
  argdata_free(ad);
}

TEST(argdata_print_yaml, bool) {
  TEST_OBJECT(&argdata_false, "!!bool \"false\"");
  TEST_OBJECT(&argdata_true, "!!bool \"true\"");
}

TEST(argdata_print_yaml, fd) {
  ASSERT_EQ(NULL, argdata_create_fd(-1));
  ASSERT_EQ(EBADF, errno);

  argdata_t *ad = argdata_create_fd(12345678);
  TEST_OBJECT(ad, "!!fd \"12345678\"");
  argdata_free(ad);
}

TEST(argdata_print_yaml, int) {
#define TEST_INT(value, out)                   \
  do {                                         \
    argdata_t *ad = argdata_create_int(value); \
    TEST_OBJECT(ad, "!!int \"" out "\"");      \
    argdata_free(ad);                          \
  } while (0)
  // TODO(edje): Add more tests.
  TEST_INT(0, "0");
  TEST_INT(0xdeadc0de, "3735929054");
  TEST_INT(INT64_MIN, "-9223372036854775808");
  TEST_INT(INT64_MAX, "9223372036854775807");
  TEST_INT(UINT64_MAX, "18446744073709551615");
#undef TEST_INT
}

TEST(argdata_print_yaml, map) {
  {
    argdata_t *ad = argdata_create_map(NULL, NULL, 0);
    TEST_OBJECT(ad, "!!map {}");
    argdata_free(ad);
  }

  {
    const argdata_t *keys[] = {&argdata_true, &argdata_false, &argdata_null};
    const argdata_t *values[] = {&argdata_null, &argdata_true, &argdata_false};
    argdata_t *ad = argdata_create_map(keys, values, __arraycount(keys));
    TEST_OBJECT(ad,
                "!!map {\n"
                "  ? !!bool \"true\"\n"
                "  : !!null \"null\",\n"
                "  ? !!bool \"false\"\n"
                "  : !!bool \"true\",\n"
                "  ? !!null \"null\"\n"
                "  : !!bool \"false\",\n"
                "}");
    argdata_free(ad);
  }
}

TEST(argdata_print_yaml, null) {
  TEST_OBJECT(&argdata_null, "!!null \"null\"");
}

TEST(argdata_print_yaml, seq) {
  {
    argdata_t *ad = argdata_create_seq(NULL, 0);
    TEST_OBJECT(ad, "!!seq []");
    argdata_free(ad);
  }

  {
    const argdata_t *entries[] = {&argdata_false, &argdata_null, &argdata_true};
    argdata_t *ad = argdata_create_seq(entries, __arraycount(entries));
    TEST_OBJECT(ad,
                "!!seq [\n"
                "  !!bool \"false\",\n"
                "  !!null \"null\",\n"
                "  !!bool \"true\",\n"
                "]");
    argdata_free(ad);
  }
}

TEST(argdata_print_yaml, str) {
  argdata_t *ad = argdata_create_str_c("Hello, world");
  TEST_OBJECT(ad, "!!str \"Hello, world\"");
  argdata_free(ad);
}