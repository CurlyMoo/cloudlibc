// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int fclose(FILE *stream) {
  // Close underlying descriptor.
  bool result = fop_close(stream);

  // Free file object and associated data.
  pthread_mutex_destroy(&stream->lock);
  free(stream);
  return result ? 0 : EOF;
}
