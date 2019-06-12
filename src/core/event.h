/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_CORE_EVENT_H__
#define __SERVERKIT_CORE_EVENT_H__

#include "core/poller.h"

namespace serverkit {
// virtual interface for notify events
class Event {
public:
  Event() 
    : handle_(kInvalidHandle),
      timer_id_(kInvalidTimer) {
  }
  virtual ~Event() {}

  virtual void In() = 0;

  virtual void Out() = 0;

  virtual void Timeout() = 0;

  void SetTimerId(timer_id_t id) {
    timer_id_ = id;
  }

protected:
  handle_t handle_;
  timer_id_t timer_id_;
};

};  // namespace serverkit

#endif  // __SERVERKIT_CORE_EVENT_H__
