/*
 * Copyright (C) codedump
 */
#ifndef __SERVERKIT_CORE_POLLER_H__
#define __SERVERKIT_CORE_POLLER_H__

#include <map>
#include <stdio.h>
#include "base/atomic.h"
#include "base/time.h"
#include "core/typedef.h"

namespace serverkit {

// epoll
class EpollEntry;
typedef EpollEntry* handle_t;
static const handle_t kInvalidHandle = NULL;

static const int kEventRead  = 1 << 1;
static const int kEventWrite = 1 << 2;

class Event;

class Poller {
public:
  Poller();

  virtual ~Poller();

  virtual int    Init(int size) = 0;
  virtual handle_t Add(fd_t fd, Event *event, int) = 0;
  virtual int    Del(handle_t) = 0;
  virtual int    ResetIn(handle_t) = 0; 
  virtual int    SetIn(handle_t) = 0; 
  virtual int    ResetOut(handle_t) = 0; 
  virtual int    SetOut(handle_t) = 0; 

  timer_id_t AddTimer(int timeout, Event *);
  void CancelTimer(timer_id_t);
  void Loop();

protected:
  virtual int    Poll(int timeout) = 0;

  uint64_t executeTimers();

  // for load
  inline void adjustLoad(int cnt) {
    if (cnt > 0) {
      load_.add(cnt);
    } else if (cnt < 0) {
      load_.sub(-cnt);
    }
  }

  inline int getLoad() const {
    return load_.get();
  }

protected:  
  struct TimerEntry {
    uint64_t expire;
    Event *event;
    timer_id_t id;

    TimerEntry(uint64_t ex, Event *e, timer_id_t i)
      : expire(ex),event(e), id(i) {}
  };
  typedef std::multimap<uint64_t, TimerEntry*> TimerMap;
  TimerMap timers_;
  typedef std::map<timer_id_t, TimerEntry*> TimerIdMap;
  TimerIdMap timer_ids_;
  timer_id_t max_timer_id_;
  atomic_counter_t load_;
};

extern uint64_t CurrentMs();

};  // namespace serverkit

#endif // __SERVERKIT_CORE_POLLER_H__
