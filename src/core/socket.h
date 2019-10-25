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
  // init state
  SOCKET_INIT = 0,
  // when client connecting the server
  SOCKET_CONNECTING = 1,
  // socket has been connected
  SOCKET_CONNECTED = 2,
  // socket has been closed
  SOCKET_CLOSED = 3,
};

// Socket has user-space write and read stack to buffer the data. 
// When send data to socket, it will buffer the data, and then send if epoll return writeable.
// If send data fail, DataHandler->OnError will be called.
// When read data from socket, it will read data from the buffer.
class Socket : public Event {
  friend Socket* CreateClientSocket(const Endpoint& , Poller*, DataHandler*);
  friend Socket* CreateServerSocket(int, DataHandler*);

public:
  virtual ~Socket();

  // only be called if it is a client socket
  void Connect();

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
    return endpoint_;
  }

  inline const string& String() {
    return endpoint_.String();
  }
  
  int Fd() const { 
    return fd_;
  }
private:
  // socket constructor is private, it can only be creat from
  // CreateServerSocket or CreateClientSocket

  // create a server accepted socket, called from CreateServerSocket
  Socket(int fd, DataHandler*);
  // create a client socket, called from CreateClientSocket
  Socket(const Endpoint&, Poller*, DataHandler*);
  void close();

private:
  enum socketType {
    // client socket
    CLIENT_SOCKET,
    // server accept socket
    SERVER_SOCKET,
  };

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

  socketType type_;

  // disable copy and assign operate
  DISALLOW_COPY_AND_ASSIGN(Socket);
};

// create a client socket
extern Socket* CreateClientSocket(const Endpoint&, Poller*, DataHandler*);

// create a server accepted socket
extern Socket* CreateServerSocket(int, DataHandler*);

};  // namespace serverkit

#endif  // __SERVERKIT_CORE_SOCKET_H__
