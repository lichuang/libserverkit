/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_CORE_TIMER_THREAD_H__
#define __SERVERKIT_CORE_TIMER_THREAD_H__

#include <stdint.h>
#include <time.h>
#include "base/thread.h"

namespace serverkit {

struct Time {
    time_t    sec;    // seconds since 1970.01.01 00:00:00
    uint64_t  msec;   // 
    int       gmtoff; // 
};

// number of cached time slots 
static const int kTimeSlots = 64;

// update internal msec
static const int kUpdateIntervalMsec = 50;

class TimerThread : public Thread {
public:
  TimerThread();
  virtual ~TimerThread();

  // msec since 1970.01.01 00:00:00
  uint64_t GetNowMs() const {
    return *cached_msec_;
  }

private:
  void updateTime();

protected:
  virtual void Run();

private:
  Time cached_times_[kTimeSlots];
  uint64_t cached_msecs_[kTimeSlots];

  volatile Time* cached_time_;
  volatile uint64_t* cached_msec_;

  int index_;
};

};  // namespace serverkit

#endif  // __SERVERKIT_CORE_TIMER_THREAD_H__