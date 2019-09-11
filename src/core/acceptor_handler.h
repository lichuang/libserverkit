/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_CORE_ACCEPTOR_HANDLER_H__
#define __SERVERKIT_CORE_ACCEPTOR_HANDLER_H__

#include "core/session.h"

namespace serverkit {

class AcceptorHandler {
public:
  AcceptorHandler(SessionFactory* factory)
    : factory_(factory) {
  }
  virtual ~AcceptorHandler() {
    delete factory_;
  }

  virtual Session* OnAccept(int) = 0;
  virtual void OnError(int error) = 0;

  SessionFactory* GetFactory() {
    return factory_;
  }
protected:
  SessionFactory* factory_;  
};

};  // namespace serverkit

#endif  // __SERVERKIT_CORE_ACCEPTOR_HANDLER_H__
