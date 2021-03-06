// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/procdesc.h>

#include <program.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <testing.h>
#include <unistd.h>

TEST(program_getuuid, format) {
  // Validate the format of the resulting UUID. As we're generating
  // random UUIDs, the version number should be set to four.
  const char *uuid = program_getuuid();
#define ASSERT_XDIGIT(c) \
  ASSERT_TRUE(((c) >= '0' && (c) <= '9') || ((c) >= 'a' && (c) <= 'f'))
  ASSERT_XDIGIT(uuid[0]);
  ASSERT_XDIGIT(uuid[1]);
  ASSERT_XDIGIT(uuid[2]);
  ASSERT_XDIGIT(uuid[3]);
  ASSERT_XDIGIT(uuid[4]);
  ASSERT_XDIGIT(uuid[5]);
  ASSERT_XDIGIT(uuid[6]);
  ASSERT_XDIGIT(uuid[7]);
  ASSERT_EQ('-', uuid[8]);
  ASSERT_XDIGIT(uuid[9]);
  ASSERT_XDIGIT(uuid[10]);
  ASSERT_XDIGIT(uuid[11]);
  ASSERT_XDIGIT(uuid[12]);
  ASSERT_EQ('-', uuid[13]);
  ASSERT_EQ('4', uuid[14]);
  ASSERT_XDIGIT(uuid[15]);
  ASSERT_XDIGIT(uuid[16]);
  ASSERT_XDIGIT(uuid[17]);
  ASSERT_EQ('-', uuid[18]);
  ASSERT_TRUE(uuid[19] == '8' || uuid[19] == '9' || uuid[19] == 'a' ||
              uuid[19] == 'b');
  ASSERT_XDIGIT(uuid[20]);
  ASSERT_XDIGIT(uuid[21]);
  ASSERT_XDIGIT(uuid[22]);
  ASSERT_EQ('-', uuid[23]);
  ASSERT_XDIGIT(uuid[24]);
  ASSERT_XDIGIT(uuid[25]);
  ASSERT_XDIGIT(uuid[26]);
  ASSERT_XDIGIT(uuid[27]);
  ASSERT_XDIGIT(uuid[28]);
  ASSERT_XDIGIT(uuid[29]);
  ASSERT_XDIGIT(uuid[30]);
  ASSERT_XDIGIT(uuid[31]);
  ASSERT_XDIGIT(uuid[32]);
  ASSERT_XDIGIT(uuid[33]);
  ASSERT_XDIGIT(uuid[34]);
  ASSERT_XDIGIT(uuid[35]);
  ASSERT_EQ('\0', uuid[36]);
#undef ASSERT_XDIGIT
}

TEST(program_getuuid, cached) {
  // Successive invocations should always return the same value.
  ASSERT_EQ(program_getuuid(), program_getuuid());
}

TEST(program_getuuid, fork) {
  char *copy = strdup(program_getuuid());
  int fd;
  int ret = pdfork(&fd);

  // Child process should have a different UUID.
  if (ret == 0) {
    ASSERT_STRNE(copy, program_getuuid());
    _Exit(123);
  }

  // Parent process should remain unaffected.
  ASSERT_STREQ(copy, program_getuuid());
  free(copy);

  ASSERT_LT(0, ret);
  siginfo_t si;
  ASSERT_EQ(0, pdwait(fd, &si, 0));
  ASSERT_EQ(0, close(fd));

  // Validate exit code.
  ASSERT_EQ(SIGCHLD, si.si_signo);
  ASSERT_EQ(CLD_EXITED, si.si_code);
  ASSERT_EQ(123, si.si_status);
}
