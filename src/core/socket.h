/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_CORE_SOCKET_H__
#define __SERVERKIT_CORE_SOCKET_H__

#include <string>
#include "base/buffer.h"
#include "base/endpoint.h"
#include "base/macros.h"
#include "core/event.h"

using namespace std;

namespace serverkit {

class DataHandler;
class Poller;

enum SocketStatus {
  SOCKET_INIT = 0,
  SOCKET_CONNECTING = 1,
  SOCKET_CONNECTED = 2,
  SOCKET_CLOSED = 3,
};

class Socket : public Event {
public:
  Socket(int fd, DataHandler*);

  virtual ~Socket();

  void Connect(const Endpoint& endpoint);

  void SetPoller(Poller *);

  size_t ReadBufferSize() const {
    return read_list_.TotalSize();
  }

  size_t WriteBufferSize() const {
    return write_list_.TotalSize();
  }

  void Write(const char* from, size_t n);
  size_t Read(char* to, size_t n);

  virtual void In();

  virtual void Out();

  virtual void Timeout();

  int    ResetIn(); 
  int    SetIn(); 
  int    ResetOut(); 
  int    SetOut(); 

  const SocketStatus& Status() const {
    return status_;
  }

  const Endpoint& GetEndpoint() const {
    return endpoint_;
  }

  const string& String() {
    return endpoint_.String();
  }
private:
  void CloseSocket();

private:  
  int fd_;
  DataHandler *handler_;
  Poller* poller_;
  bool is_writable_;
  BufferList read_list_;
  BufferList write_list_;
  SocketStatus status_;
  Endpoint endpoint_;
  DISALLOW_COPY_AND_ASSIGN(Socket);
};

};  // namespace serverkit

#endif  // __SERVERKIT_CORE_SOCKET_H__
