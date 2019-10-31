/*
 * Copyright (C) codedump
 */

#include <list>
#include "util/time.h"
#include "base/thread.h"
#include "core/event.h"
#include "core/poller.h"
#include <string>

using namespace std;

namespace serverkit {

static const int kDefaultTimeout = 100;

Poller::Poller()
  : max_timer_id_(0) {
}

Poller::~Poller() {
}

timer_id_t
Poller::AddTimer(int timeout, Event *event, TimerType t) {
  uint64_t expire = CurrentMs() + timeout;
  ++max_timer_id_;
  timer_id_t id = max_timer_id_;
  TimerEntry *entry = new TimerEntry(timeout, expire, event, id, t);
  timers_.insert(TimerMap::value_type(expire, entry));
  timer_ids_[id] = entry;
  event->SetTimerId(id);
  return id;
} 

void
Poller::CancelTimer(timer_id_t id) {
  TimerIdMap::iterator iter = timer_ids_.find(id);
  if (iter == timer_ids_.end()) {
    return;
  }
  TimerEntry *entry = iter->second;
  uint64_t expire = entry->expire;
  TimerMap::iterator pos;
  for (pos = timers_.lower_bound(expire);
       pos != timers_.upper_bound(expire); ++pos) {
    if (pos->second->id == id) {
      break;
    }
  }
  timers_.erase(pos);
  timer_ids_.erase(id);
  delete entry;
}

int
Poller::executeTimers() {
  if (timers_.empty()) {
    // if there is no timers, return default timeout
    return kDefaultTimeout;
  }
  
  int res = kDefaultTimeout;
  TimerIdMap::iterator iter = timers_.begin();
  TimerIdMap::iterator end = timers_.end();
  // save out time timers
  list<TimerIdMap::iterator> del_timers;

  for (; iter != end; ++iter) {
    if (iter->first > now_ms_) {
      // save the timeout and return
      res = static_cast<int>(iter->first - now_ms_);
      break;
    }

    // fire the timer callback
    iter->second->event->Timeout();
    del_timers.push_back(iter);
  }

  // delete out time timers
  for (list<TimerIdMap::iterator>::iterator i = del_timers.begin(); i != del_timers.end(); ++i) {
    TimerIdMap::iterator tmp = *i;
    TimerEntry *entry = tmp->second;
    timers_.erase(tmp);

    if (entry->t == kTimerOnce) {
      // fire only once timer
      timer_ids_.erase(entry->id);
      delete entry;
    } else {
      // fire repeated timer
      uint64_t expire = now_ms_ + entry->timeout;
      timers_.insert(TimerMap::value_type(expire, entry));
    }
  }

  return res;
}

void
Poller::Dispatch() {
  int timeout;

  now_ms_ = CurrentMs();
  timeout = executeTimers();

  Poll(timeout);
}

Poller::TimerEntry::~TimerEntry() {
  delete event;
}

};  // namespace serverkit
