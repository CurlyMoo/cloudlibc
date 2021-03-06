// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <search.h>

#ifndef tfind
#error "tfind is supposed to be a macro as well"
#endif

posix_tnode *(tfind)(const void *key, posix_tnode *const *rootp,
                     int (*compar)(const void *, const void *)) {
  return tfind(key, rootp, compar);
}
