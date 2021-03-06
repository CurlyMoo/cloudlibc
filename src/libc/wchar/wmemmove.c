// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <wchar.h>

wchar_t *wmemmove(wchar_t *ws1, const wchar_t *ws2, size_t n) {
  wchar_t *wsb1 = ws1;
  const wchar_t *wsb2 = ws2;
  if (wsb1 < wsb2) {
    while (n-- > 0)
      *wsb1++ = *wsb2++;
  } else if (wsb1 > wsb2) {
    wsb1 += n;
    wsb2 += n;
    while (n-- > 0)
      *--wsb1 = *--wsb2;
  }
  return ws1;
}

__strong_reference(wmemmove, wmemcpy);
