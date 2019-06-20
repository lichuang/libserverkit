/*
 * Copyright (C) codedump
 */
#include "base/time.h"
#include "core/event.h"
#include "core/poller.h"
#include <string>

using namespace std;

namespace serverkit {

thread_local uint64_t gCurrentMs;
thread_local char gCurrentTimeString[sizeof("yy-mm-dd hh-mm-ss.000") - 1];

Poller::Poller()
  : max_timer_id_(0) {
}

Poller::~Poller() {
}

timer_id_t
Poller::AddTimer(int timeout, Event *event) {
  uint64_t expire = NowMs() + timeout;
  ++max_timer_id_;
  timer_id_t id = max_timer_id_;
  TimerEntry *entry = new TimerEntry(expire, event, id);
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

uint64_t
Poller::executeTimers() {
  if (timers_.empty()) {
    return 0;
  }
  uint64_t current = NowMs();
  uint64_t res = 0;
  TimerIdMap::iterator iter = timers_.begin();
  TimerIdMap::iterator begin = timers_.begin();
  TimerIdMap::iterator end = timers_.end();
  for (; iter != end; ++iter) {
    if (iter->first > current) {
      res = iter->first - current;
      break;
    }

    iter->second->event->Timeout();
  }
  timers_.erase(begin, iter);

  return res;
}

void
Poller::updateTime() {
  gCurrentMs = NowMs();

  struct timeval t;
  ::gettimeofday(&t, NULL);
  struct tm tim;
  ::localtime_r(&t.tv_sec, &tim);
  snprintf(gCurrentTimeString, sizeof(gCurrentTimeString), "%04d-%02d-%02d %02d:%02d:%02d.%03d",
    tim.tm_year + 1900, tim.tm_mon + 1, tim.tm_mday,
    tim.tm_hour, tim.tm_min, tim.tm_sec, (int)t.tv_usec / 1000);
}

void
Poller::Loop() {
  int timeout;

  while (true) {
    timeout = executeTimers();
    if (timeout) {
      if (getLoad() == 0) {
        continue;
      }
    }

    updateTime();

    Poll(timeout);
  }
}

};  // namespace serverkit
