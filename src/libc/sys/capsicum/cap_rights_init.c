// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/capsicum.h>

#include <stdarg.h>

cap_rights_t *__cap_rights_init(cap_rights_t *rights, ...) {
  // Set all of the capability bits provided as arguments.
  va_list ap;
  va_start(ap, rights);
  rights->__value = 0;
  for (;;) {
    __cap_rights_bits_t right = va_arg(ap, __cap_rights_bits_t);
    if (right == _CAP_SENTINEL)
      break;
    rights->__value |= right;
  }
  va_end(ap);
  return rights;
}
