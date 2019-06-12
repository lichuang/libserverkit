/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_CORE_SIGNALER_H__
#define __SERVERKIT_CORE_SIGNALER_H__

#include "base/macros.h"
#include "core/typedef.h"

namespace serverkit {

class Signaler {
public:
  Signaler();
  ~Signaler();

  fd_t Fd() const {
    return rfd_;
  }
  void Send();
  int Wait(int timeout);
  void Recv();

  int RecvFailable();
private:
  fd_t wfd_;
  fd_t rfd_;

  DISALLOW_COPY_AND_ASSIGN(Signaler);
};

};  // namespace serverkit

#endif // __SERVERKIT_CORE_SIGNALER_H__
