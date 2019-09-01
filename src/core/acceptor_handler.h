/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_CORE_ACCEPTOR_HANDLER_H__
#define __SERVERKIT_CORE_ACCEPTOR_HANDLER_H__

#include "base/status.h"

namespace serverkit {

class Session;

class AcceptorHandler {
public:
  virtual ~AcceptorHandler() {
  }

  virtual void OnAccept(Session*) = 0;
  virtual void OnError(const Status& status) = 0;
};

};  // namespace serverkit

#endif  // __SERVERKIT_CORE_ACCEPTOR_HANDLER_H__
