/*
 * Copyright (C) codedump
 */

#include <time.h>
#include <sys/time.h>
#include "util/time.h"
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
TimerThread::doInit() {

}

void 
TimerThread::updateTime() {
  Time *tp;
  uint64_t *tmsec;
  char* logStr;
  time_t sec;
  uint64_t msec;

  index_ = (index_ + 1) % kTimeSlots;

  tp = &cached_times_[index_];
  tmsec = &cached_msecs_[index_];
  logStr = &(cached_log_time_strs_[index_][0]);

  struct timeval tv;
  gettimeofday(&tv, NULL);

  sec = tv.tv_sec;
  msec = tv.tv_usec / 1000;

  *tmsec = sec * 1000 + msec;
  tp->sec = sec;
  tp->msec = msec;

  struct tm tim;
	Localtime(tv.tv_sec, &tim);
  
  snprintf(logStr, kLogTimeStringLength,
    "%4d/%02d/%02d %02d:%02d:%02d.%03d",
    tim.tm_year + 1900, tim.tm_mon + 1, tim.tm_mday,
    tim.tm_hour, tim.tm_min, tim.tm_sec, static_cast<int>(msec)); 

  cached_time_ = tp;
  cached_msec_ = tmsec;
  cached_log_time_str_ = logStr;
}

void
TimerThread::Run() {
  while (Running()) {
    updateTime();
    SleepMillisecs(kUpdateIntervalMsec);
  }
}

};  // namespace serverkit