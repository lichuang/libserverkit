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

// update internal msec
static const int kUpdateIntervalMsec = 50;

// log time string length
static const int kLogTimeStringLength = sizeof("2019/10/01 00:00:00.000");

class TimerThread : public Thread {
  friend class Singleton<TimerThread>;

public:
  virtual ~TimerThread();

  // msec since 1970.01.01 00:00:00
  inline uint64_t GetNowMs() const {
    return *cached_msec_;
  }

  inline char* GetNowLogtimeString() const {
    return const_cast<char*>(cached_log_time_str_);
  }

private:
  TimerThread();
  void updateTime();
  void doInit();

protected:
  virtual void Run();

private:
  Time cached_times_[kTimeSlots];
  char cached_log_time_strs_[kTimeSlots][kLogTimeStringLength];
  uint64_t cached_msecs_[kTimeSlots];

  volatile Time* cached_time_;
  volatile char* cached_log_time_str_;
  volatile uint64_t* cached_msec_;

  int index_;
};

#define gTimerThread serverkit::Singleton<serverkit::TimerThread>::Instance()

#define CurrentMs gTimerThread->GetNowMs
#define CurrentLogtimeString gTimerThread->GetNowLogtimeString

};  // namespace serverkit

#endif  // __SERVERKIT_CORE_TIMER_THREAD_H__