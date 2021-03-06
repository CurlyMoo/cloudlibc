// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/errno.h>

#include <cloudabi_syscalls.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int symlinkat(const char *path1, int fd, const char *path2) {
  cloudabi_errno_t error =
      cloudabi_sys_file_symlink(path1, strlen(path1), fd, path2, strlen(path2));
  if (error != 0) {
    errno = errno_fixup_directory(fd, error);
    return -1;
  }
  return 0;
}
