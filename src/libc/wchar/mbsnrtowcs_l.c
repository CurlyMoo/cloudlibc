// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <stdbool.h>
#include <uchar.h>
#include <wchar.h>

size_t mbsnrtowcs_l(wchar_t *restrict dst, const char **restrict src,
                    size_t nmc, size_t len, mbstate_t *restrict ps,
                    locale_t locale) {
  const struct lc_ctype *ctype = locale->ctype;
  return ctype->mbstoc32s((char32_t *)dst, len, src, nmc, ps, ctype->data,
                          false);
}
