/*
 * Copyright (C) codedump
 */

#include <stdio.h>
#include "base/clock.h"
#include "base/time.h"

namespace serverkit {
Clock::Clock() {
  Update();
}

void
Clock::Update() {
  last_ms_ = NowMs();
}

};  // namespace serverkit
