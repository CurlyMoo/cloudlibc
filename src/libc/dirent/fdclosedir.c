// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <dirent.h>
#include <stdlib.h>

#include "dirent_impl.h"

int fdclosedir(DIR *dirp) {
  int fd = dirp->fd;
  free(dirp->buffer);
  free(dirp->dirent);
  free(dirp);
  return fd;
}
