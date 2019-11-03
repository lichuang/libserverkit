/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_CORE_ACCEPTOR_HANDLER_H__
#define __SERVERKIT_CORE_ACCEPTOR_HANDLER_H__

#include "core/session.h"

namespace serverkit {

// AcceptorHandler handle client connect socket event
// when a client connected to the server, it create 
// a new session using SessionFactory
class AcceptorHandler {
public:
  AcceptorHandler(SessionFactory* factory)
    : factory_(factory) {
  }
  virtual ~AcceptorHandler() {
    delete factory_;
  }

  // OnAccept create a new session using SessionFactory
  virtual Session* OnAccept(int) = 0;
  virtual void OnError(int error) = 0;

  SessionFactory* GetFactory() {
    return factory_;
  }
protected:
  // SessionFactory
  SessionFactory* factory_;  
};

};  // namespace serverkit

#endif  // __SERVERKIT_CORE_ACCEPTOR_HANDLER_H__
