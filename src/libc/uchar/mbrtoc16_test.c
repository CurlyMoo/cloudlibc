// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <locale.h>
#include <testing.h>
#include <uchar.h>
#include <wchar.h>

TEST(mbrtoc16, ascii) {
  static const mbstate_t initial_mbstate;
  mbstate_t mbs = initial_mbstate;
  char16_t c16;

  // Valid character.
  ASSERT_EQ(1, mbrtoc16(&c16, "Foo", 3, &mbs));
  ASSERT_EQ(u'F', c16);
  ASSERT_NE(0, mbsinit(&mbs));

  // Null character.
  ASSERT_EQ(0, mbrtoc16(&c16, "", 1, &mbs));
  ASSERT_EQ(u'\0', c16);
  ASSERT_NE(0, mbsinit(&mbs));

  // Buffer too short.
  ASSERT_EQ((size_t)-2, mbrtoc16(&c16, "Some text", 0, &mbs));
  ASSERT_NE(0, mbsinit(&mbs));

  // Invalid character.
  ASSERT_EQ((size_t)-1, mbrtoc16(&c16, "€", 4, &mbs));
  ASSERT_EQ(EILSEQ, errno);
  ASSERT_NE(0, mbsinit(&mbs));
}

TEST(mbrtoc16, unicode) {
  static const mbstate_t initial_mbstate;
  mbstate_t mbs = initial_mbstate;
  char16_t c16;

  // Valid character.
  ASSERT_EQ(1, mbrtoc16_l(&c16, "Foo", 3, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(u'F', c16);
  ASSERT_NE(0, mbsinit(&mbs));

  // Piecewise consume an Unicode character.
  ASSERT_EQ((size_t)-2,
            mbrtoc16_l(&c16, "\xf0\x90", 2, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(0, mbsinit(&mbs));
  ASSERT_EQ((size_t)-2, mbrtoc16_l(&c16, "\x90", 1, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(0, mbsinit(&mbs));
  ASSERT_EQ(1, mbrtoc16_l(&c16, "\xb7", 1, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(0xd801, c16);
  ASSERT_EQ(0, mbsinit(&mbs));
  ASSERT_EQ((size_t)-3, mbrtoc16_l(&c16, "AAA", 3, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(0xdc37, c16);
  ASSERT_NE(0, mbsinit(&mbs));

  // Null character.
  ASSERT_EQ(0, mbrtoc16_l(&c16, "", 1, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(u'\0', c16);
  ASSERT_NE(0, mbsinit(&mbs));

  // Buffer too short.
  ASSERT_EQ((size_t)-2,
            mbrtoc16_l(&c16, "Some text", 0, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_NE(0, mbsinit(&mbs));
}
