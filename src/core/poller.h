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

enum TimerType {
  // trigger one time
  kTimerOnce,
  // trigger permanent
  kTimerPermanent
};

class Event;

class Poller {
public:
  Poller();

  virtual ~Poller();

  virtual int       Init(int size) = 0;
  virtual handle_t  Add(fd_t fd, Event *event) = 0;
  virtual int       Del(handle_t handle) = 0;
  virtual void      MarkWriteable(handle_t handle) = 0;

  timer_id_t AddTimer(int timeout, Event *, TimerType);
  void CancelTimer(timer_id_t);
  void Dispatch();

protected:
  virtual int    Poll(int timeout) = 0;

  int executeTimers();

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
    // timeout(in ms)
    uint64_t timeout;

    // expire time(in ms)
    uint64_t expire;

    // related event
    Event *event;

    // time id
    timer_id_t id;

    // timer type
    TimerType t;

    TimerEntry(uint64_t out,uint64_t ex, Event *e, timer_id_t i, TimerType typ)
      : timeout(out), expire(ex),event(e), id(i), t(typ) {}
    ~TimerEntry();     
  };

  typedef std::multimap<uint64_t, TimerEntry*> TimerMap;
  typedef std::map<timer_id_t, TimerEntry*> TimerIdMap;
  
  // map expire time to TimerEntry
  TimerMap timers_;

  // map timer ID to TimerEntry
  TimerIdMap timer_ids_;

  timer_id_t max_timer_id_;
  atomic_counter_t load_;

  uint64_t now_ms_;
};

extern uint64_t CurrentMs();

};  // namespace serverkit

#endif // __SERVERKIT_CORE_POLLER_H__
