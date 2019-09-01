/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_CORE_SESSION_H__
#define __SERVERKIT_CORE_SESSION_H__

#include <string>
#include "core/data_handler.h"
#include "poller.h"
#include "socket.h"

using namespace std;

namespace serverkit {

class Poller;

class Session : public DataHandler {
public:
  Session(int fd, const string& addr)
    : socket_(new Socket(fd, addr, this)),
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
  virtual void OnError(const Status& status) = 0;

  Socket* GetSocket() {
    return socket_;
  }
  const string& String() const {
    return socket_->String();
  }
  const char* CString() const {
    return socket_->String().c_str();
  }

protected:
  Socket *socket_;
  uint64_t last_read_time_;
  uint64_t last_write_time_;
  Poller *poller_;
};

class SessionFactory {
public:
  virtual ~SessionFactory() {
  }

  virtual Session* Create(int fd, const string&) = 0;
};

};  // namespace serverkit
#endif  // __SERVERKIT_CORE_SESSION_H__
