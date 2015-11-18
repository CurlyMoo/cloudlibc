// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <wctype.h>

TEST(towlower, examples) {
  ASSERT_EQ(WEOF, towlower(WEOF));

  ASSERT_EQ(L' ', towlower(L' '));
  ASSERT_EQ(L'€', towlower(L'€'));

  ASSERT_EQ(L'a', towlower(L'A'));
  ASSERT_EQ(L'a', towlower(L'a'));

  ASSERT_EQ(L'ä', towlower(L'Ä'));
  ASSERT_EQ(L'ä', towlower(L'ä'));

  ASSERT_EQ(L'λ', towlower(L'Λ'));
  ASSERT_EQ(L'λ', towlower(L'λ'));

  ASSERT_EQ(L'𐐷', towlower(L'𐐏'));
  ASSERT_EQ(L'𐐷', towlower(L'𐐷'));
}
