/*
 * Copyright (C) codedump
 */

#include <unistd.h>
#include "base/time.h"

namespace serverkit {

struct timespec *
MakeTimespec(int64_t ms, struct timespec *ts) {
  if (ms < 0) {
    ms = 0;
  }
  struct timeval tv;
  ::gettimeofday(&tv, 0);
  ts->tv_sec = tv.tv_sec + ms / 1000;
  ts->tv_nsec = tv.tv_usec * 1000 + (ms % 1000) * 1000000;
  if (ts->tv_nsec >= 1000000000) {
    ts->tv_nsec -= 1000000000;
    ts->tv_sec++;
  }
  return ts;
}

uint64_t
NowMs() {
  struct timeval tv;
  gettimeofday (&tv, NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void 
TZSet() {
  tzset(); /* Now 'timezome' global is populated. */
}

void
Localtime(const time_t& unix_sec, struct tm* result) {
    static const int kHoursInDay = 24;
    static const int kMinutesInHour = 60;
    static const int kDaysFromUnixTime = 2472632;
    static const int kDaysFromYear = 153;
    static const int kMagicUnkonwnFirst = 146097;
    static const int kMagicUnkonwnSec = 1461;

    result->tm_sec = static_cast<int>(unix_sec % kMinutesInHour);
    int i      = static_cast<int>(unix_sec/kMinutesInHour);
    result->tm_min  = i % kMinutesInHour; //nn
    i /= kMinutesInHour;
    result->tm_hour = static_cast<int>((i + 8) % kHoursInDay); // hh
    result->tm_mday = static_cast<int>((i + 8) / kHoursInDay);
    int a = result->tm_mday + kDaysFromUnixTime;
    int b = (a*4  + 3)/kMagicUnkonwnFirst;
    int c = (-b*kMagicUnkonwnFirst)/4 + a;
    int d =((c*4 + 3) / kMagicUnkonwnSec);
    int e = -d * kMagicUnkonwnSec;
    e = e/4 + c;
    int m = (5*e + 2)/kDaysFromYear;
    result->tm_mday = -(kDaysFromYear * m + 2)/5 + e + 1;
    result->tm_mon = (-m/10)*12 + m + 2;
    result->tm_year = b*100 + d  - 6700 + (m/10);
}

void 
SleepMillisecs(int msec) {
  ::usleep(msec * 1000);
}

};  // namespace serverkit
