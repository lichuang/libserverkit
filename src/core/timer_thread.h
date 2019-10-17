/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_CORE_TIMER_THREAD_H__
#define __SERVERKIT_CORE_TIMER_THREAD_H__

#include <stdint.h>
#include <time.h>
#include "base/singleton.h"
#include "base/thread.h"

namespace serverkit {

struct Time {
    time_t    sec;    // seconds since 1970.01.01 00:00:00
    uint64_t  msec;   // 
    int       gmtoff; // 
};

// number of cached time slots 
static const int kTimeSlots = 64;

// update time internal(in msec)
static const int kUpdateIntervalMsec = 50;

// log time string length
static const int kLogTimeStringLength = sizeof("2019/01/01 00:00:00.000");

// timer thread generate system-wide time per kUpdateIntervalMsec
// timer thread use a slot to cache time,string etc.
class TimerThread : public Thread {
  friend class Singleton<TimerThread>;

public:
  virtual ~TimerThread();

  // msec since 1970.01.01 00:00:00
  inline uint64_t GetNowMs() const {
    return *cached_msec_;
  }

  // now time log string
  inline const char* GetNowLogtimeString() const {
    return const_cast<const char*>(cached_log_time_str_);
  }

private:
  TimerThread();
  void updateTime();
  void doInit();

protected:
  virtual void Run();

private:
  // cache slots
  Time cached_times_[kTimeSlots];
  char cached_log_time_strs_[kTimeSlots][kLogTimeStringLength];
  uint64_t cached_msecs_[kTimeSlots];

  // current volatile variables
  volatile Time* cached_time_;
  volatile char* cached_log_time_str_;
  volatile uint64_t* cached_msec_;

  // current slots index
  int index_;
};

#define gTimerThread serverkit::Singleton<serverkit::TimerThread>::Instance()

// global time API

// return msec since 1970.01.01 00:00:00
#define CurrentMs gTimerThread->GetNowMs

// return current log time string
#define CurrentLogtimeString gTimerThread->GetNowLogtimeString

};  // namespace serverkit

#endif  // __SERVERKIT_CORE_TIMER_THREAD_H__