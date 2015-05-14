// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <time.h>

#include "time_impl.h"

static unsigned int determine_applicable_save(
    const struct lc_timezone_rule *rules, size_t rules_count,
    const struct tm *dst, int gmtoff, bool forcedst) {
  // Obtain the rule that applies to the start of the current year, but
  // also the set of rules that applies to the current.
  struct ruleset ruleset = {};
  const struct lc_timezone_rule nomatch = {};
  const struct lc_timezone_rule *match = &nomatch, *match_dst = &nomatch;
  ruleset_fill_with_year(&ruleset, &rules, &rules_count, dst->tm_year, &match,
                         &match_dst);

  // Determine whether there is a rule for the current year that matches
  // the time more accurately than the one matching the start of the
  // year.
  for (size_t i = 0; i < ruleset.rules_count; ++i) {
    const struct lc_timezone_rule *rule = ruleset.rules[i];
    int save = match->save > rule->save ? match->save : rule->save;
    int offset = 0;
    switch (rule->timebase) {
      case TIMEBASE_STD:
        offset = -save * 600;
        break;
      case TIMEBASE_UTC:
        offset = -gmtoff - save * 600;
        break;
    }
    if (rule_is_greater_than(rule, dst, offset))
      break;
    match = rule;
    if (match->save > 0)
      match_dst = match;
  }

  // When tm_isdst < 0, simply use the last rule that applied to this
  // time of day. When tm_isdst > 0, use the last rule that applied to
  // this time of day, having a non-zero save.
  return forcedst ? match_dst->save : match->save;
}

int mktime_l(const struct tm *restrict tm, struct timespec *restrict result,
             locale_t locale) {
  // Approximate the time by assuming UTC.
  __mktime_utc(tm, result);

  const struct lc_timezone *timezone = locale->timezone;
  if (tm->tm_isdst == 0) {
    // Fast path: no daylight saving time. Simply find the first era
    // that contains this timestamp and subtract its offset.
    const struct lc_timezone_era *era = &timezone->eras[0];
    for (size_t i = 1; i < timezone->eras_count; ++i) {
      if (era->end > result->tv_sec - era->gmtoff)
        break;
      era = &timezone->eras[i];
    }
    result->tv_sec -= era->gmtoff;
  } else {
    // Slow path: time has daylight saving time (tm_isdst > 0) or it
    // needs to be inferred (tm_isdst < 0).
    bool force_dst = tm->tm_isdst > 0;
    const struct lc_timezone_era *era = &timezone->eras[0];
    for (size_t i = 1; i < timezone->eras_count; ++i) {
      if (era->end >
          result->tv_sec - era->gmtoff -
              (force_dst ? era->end_save_dst : era->end_save_actual) * 600)
        break;
      era = &timezone->eras[i];
    }

    // Process daylight saving time rules.
    if (era->rules_count > 0) {
      struct tm dst;
      __localtime_utc(result->tv_sec, &dst);
      result->tv_sec -=
          determine_applicable_save(era->rules, era->rules_count, &dst,
                                    era->gmtoff, force_dst) *
          600;
    }
    result->tv_sec -= era->gmtoff;
  }
  return 0;
}
