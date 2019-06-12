/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_CORE_LISTENER_H__
#define __SERVERKIT_CORE_LISTENER_H__

#include <string>
#include "core/event.h"

using namespace std;

namespace serverkit {

class AcceptorHandler;
class Poller;
class SessionFactory;

class Listener : public Event {
public:
  Listener(const string& addr, int port, Poller *poller,
           AcceptorHandler*, SessionFactory*);
  ~Listener();

  virtual void In();

  virtual void Out();

  virtual void Timeout();

  const char* String() const {
    return string_.c_str();
  }

private:
  string addr_;
  int port_;
  int fd_;
  Poller *poller_;
  AcceptorHandler *handler_;
  SessionFactory *factory_;
  string string_;
};

};  // namespace serverkit

#endif  // __SERVERKIT_CORE_LISTENER_H__
