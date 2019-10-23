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

const Endpoint gUnconnectedEndpoint = Endpoint("unconnected address", -1);

// Socket has user-space write and read stack to buffer the data. 
// When send data to socket, it will buffer the data, and then send if epoll return writeable.
// If send data fail, DataHandler->OnError will be called.
// When read data from socket, it will read data from the buffer.
class Socket : public Event {
public:
  Socket(int fd, DataHandler*);
  Socket(Poller*, DataHandler*);

  virtual ~Socket();

  void Connect(const Endpoint& endpoint);

  void SetPoller(Poller *);

  size_t ReadBufferSize() const {
    return read_list_.ReadableSize();
  }

  size_t WriteBufferSize() const {
    return write_list_.ReadableSize();
  }

  void Write(const char* from, size_t n);
  size_t Read(char* to, size_t n);

  virtual void In();

  virtual void Out();

  virtual void Timeout();

  const SocketStatus& Status() const {
    return status_;
  }

  const Endpoint& GetEndpoint() const {
    if (status_ == SOCKET_CONNECTED) {
      return endpoint_;
    }
    return gUnconnectedEndpoint;
  }

  inline const string& String() {
    return endpoint_.String();
  }
  
  int Fd() const { 
    return fd_;
  }
private:
  void CloseSocket();

private:
  // socket fd
  int fd_;

  // data handler
  DataHandler *handler_;

  // poller
  Poller* poller_;

  // true if write buffer has data to send
  bool is_writable_;

  // recv buffer list
  BufferList read_list_;

  // write buffer list
  BufferList write_list_;

  SocketStatus status_;
  
  // corresponding endpoint 
  Endpoint endpoint_;

  // disable copy and assign operate
  DISALLOW_COPY_AND_ASSIGN(Socket);
};

};  // namespace serverkit

#endif  // __SERVERKIT_CORE_SOCKET_H__
