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
class SessionFactory;

class Listener : public Event {
public:
  Listener(const Endpoint& endpoint, Poller *poller,
           AcceptorHandler*, SessionFactory*);
  ~Listener();

  virtual void In();

  virtual void Out();

  virtual void Timeout();

  const string String() {
    return endpoint_.String();
  }

private:
  Endpoint endpoint_;
  int fd_;
  Poller *poller_;
  AcceptorHandler *handler_;
  SessionFactory *factory_;
};

};  // namespace serverkit

#endif  // __SERVERKIT_CORE_LISTENER_H__
