/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_CORE_LISTENER_H__
#define __SERVERKIT_CORE_LISTENER_H__

#include <string>
#include "base/endpoint.h"
#include "core/event.h"

using namespace std;

namespace serverkit {

class AcceptorHandler;
class Poller;
class Application;

// connect socket event listener
class Listener : public Event {
public:
  Listener(const Endpoint& endpoint, Application *server,
           AcceptorHandler*);
  ~Listener();

  virtual void In();

  virtual void Out();

  virtual void Timeout();

  inline const string& String() {
    return endpoint_.String();
  }

  AcceptorHandler* GetAcceptorHandler() {
    return handler_;
  }

private:
  // listen endpoint
  Endpoint endpoint_;
  int fd_;
  Application *application_;
  Poller *poller_;
  AcceptorHandler *handler_;
};

};  // namespace serverkit

#endif  // __SERVERKIT_CORE_LISTENER_H__
