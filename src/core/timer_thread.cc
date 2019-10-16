/*
 * Copyright (C) codedump
 */

#include <time.h>
#include <sys/time.h>
#include "core/timer_thread.h"

namespace serverkit {

TimerThread::TimerThread()
  : Thread("timer"),
    index_(0) {
  updateTime();
}

TimerThread::~TimerThread() {

}

void 
TimerThread::updateTime() {
  volatile Time *tp;
  volatile uint64_t *tmsec;
  time_t sec;
  uint64_t msec;

  index_ = (index_ + 1) % kTimeSlots;

  tp = &cached_time_[index_];
  tmsec = &cached_msecs_[index_];

  struct timeval tv;
  gettimeofday (&tv, NULL);

  sec = tv.tv_sec;
  msec = tv.tv_usec / 1000;

  *tmsec = sec * 1000 + msec;
  tp->sec = sec;
  tp->msec = msec;
}

void
TimerThread::Run() {
  while (Running()) {
    updateTime();
  }
}

};  // namespace serverkit