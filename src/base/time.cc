/*
 * Copyright (C) codedump
 */
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

};  // namespace serverkit
