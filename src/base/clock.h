/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_BASE_CLOCK_H__
#define __SERVERKIT_BASE_CLOCK_H__

#include <stdint.h>
#include "base/macros.h"

namespace serverkit {

class Clock {
public:
  Clock();

  uint64_t NowMs() const {
    return last_ms_;
  }

  void Update();

private:
  uint64_t last_ms_;
  DISALLOW_COPY_AND_ASSIGN(Clock);
};

};  // namespace serverkit

#endif  // __SERVERKIT_BASE_CLOCK_H__
