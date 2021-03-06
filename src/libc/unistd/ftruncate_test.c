// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <testing.h>
#include <unistd.h>

TEST(ftruncate, example) {
  int fd = openat(fd_tmp, "example", O_RDWR | O_CREAT | O_EXCL);
  ASSERT_LE(0, fd);

  // Initial length should be zero.
  struct stat sb;
  ASSERT_EQ(0, fstat(fd, &sb));
  ASSERT_EQ(0, sb.st_size);

  // Invalid length.
  ASSERT_EQ(-1, ftruncate(fd, -5));
  ASSERT_EQ(EINVAL, errno);

  // Extend file.
  ASSERT_EQ(0, ftruncate(fd, 100));
  ASSERT_EQ(0, fstat(fd, &sb));
  ASSERT_EQ(100, sb.st_size);

  // Shrink it again.
  ASSERT_EQ(0, ftruncate(fd, 50));
  ASSERT_EQ(0, fstat(fd, &sb));
  ASSERT_EQ(50, sb.st_size);

  ASSERT_EQ(0, close(fd));
}
