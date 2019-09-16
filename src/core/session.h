/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_CORE_SESSION_H__
#define __SERVERKIT_CORE_SESSION_H__

#include <string>
#include "base/endpoint.h"
#include "core/data_handler.h"
#include "poller.h"
#include "socket.h"

using namespace std;

namespace serverkit {

class Poller;

class Session : public DataHandler {
public:
  Session(int fd)
    : socket_(new Socket(fd, this)),
      last_read_time_(0),
      last_write_time_(0),
      poller_(NULL) {
  }

  void SetPoller(Poller *poller) {
    poller_ = poller;
    socket_->SetPoller(poller);
  }

  virtual ~Session() {
    delete socket_;
  }

  virtual void OnWrite() {
    last_write_time_ = CurrentMs();
  }

  virtual void OnRead() {
    last_read_time_ = CurrentMs();
  }
  
  virtual void OnError(int error) = 0;

  Socket* GetSocket() {
    return socket_;
  }

  const Endpoint& GetEndpoint() const {
    return socket_->GetEndpoint();
  }
  
  const string& String() {
    return socket_->String();
  }

protected:
  Socket *socket_;
  uint64_t last_read_time_;
  uint64_t last_write_time_;
  Poller *poller_;
};

class SessionFactory {
public:
  SessionFactory(){}
  virtual ~SessionFactory() {
  }

  virtual Session* NewSession(int fd) = 0;
};

};  // namespace serverkit
#endif  // __SERVERKIT_CORE_SESSION_H__
