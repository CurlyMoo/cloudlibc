// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <wchar.h>

TEST(wcsstr, examples) {
  // If ws2 points to a string with zero length, the function shall return ws1.
  const wchar_t *str = (const wchar_t *)0x42;
  ASSERT_EQ(str, wcsstr(str, L""));

  str = L"Hello world";
  ASSERT_EQ(str + 2, wcsstr(str, L"ll"));
  ASSERT_EQ(str + 4, wcsstr(str, L"o worl"));
  ASSERT_EQ(str + 6, wcsstr(str, L"world"));
  ASSERT_EQ(str + 10, wcsstr(str, L"d"));
  ASSERT_EQ(NULL, wcsstr(str, L"word"));
  ASSERT_EQ(NULL, wcsstr(str, L"world!"));
}
