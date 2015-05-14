// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/time.h>

#include <errno.h>
#include <limits.h>
#include <stdint.h>

#include "time_impl.h"

#define ERALENGTH ((time_t)146097 * 86400)

int __localtime_utc(time_t timer, struct tm *result) {
  // The calendar repeats every 146097 days (400 years). Normalize the
  // timer to lie within this range. This ensures that the timestamp is
  // positive, simplifying the arithmetic below.
  time_t era = timer / ERALENGTH;
  timer %= ERALENGTH;
  if (timer < 0) {
    timer += ERALENGTH;
    --era;
  }

  // Compute time within the day.
  uint_fast24_t seconds = (uint_fast40_t)timer % 86400;
  struct tm tm = {};
  tm.tm_sec = seconds % 60;
  uint_fast16_t minutes = seconds / 60;
  tm.tm_min = minutes % 60;
  tm.tm_hour = minutes / 60;

  // Compute weekday. Day 0 is a Thursday.
  uint_fast24_t days = (uint_fast40_t)timer / 86400;
  tm.tm_wday = (days + 4) % 7;

  // Apply leap days, so that every year becomes exactly 366 days long.
  // First add the three leap days at the boundaries of the centuries.
  // After that we can add up to three leap days per 1461 days.
  days += ((int_fast24_t)days - 11323) / 36524;
  days += days / 1461 + (days + 731) / 1461 + (days + 1096) / 1461;
  tm.tm_yday = days % 366;

  // Compute the year. "struct tm" only uses an integer to hold the year
  // number. Bail out if the year number doesn't fit.
  uint_fast16_t local_year = days / 366 + 70;
  time_t year = era * 400 + local_year;

  // Compute month and day within month.
  const short *months = get_months(local_year);
  while (tm.tm_yday >= months[tm.tm_mon + 1])
    ++tm.tm_mon;
  tm.tm_mday = tm.tm_yday - months[tm.tm_mon] + 1;

  // Determine whether the computation overflowed or not. If it does,
  // still return a structure, but set th year to INT_MIN or INT_MAX.
  int error;
  if (year < INT_MIN) {
    tm.tm_year = INT_MIN;
    error = EOVERFLOW;
  } else if (year > INT_MAX) {
    tm.tm_year = INT_MAX;
    error = EOVERFLOW;
  } else {
    tm.tm_year = year;
    error = 0;
  }
  *result = tm;
  return error;
}
