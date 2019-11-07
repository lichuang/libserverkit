/*
 * Copyright (C) codedump
 */

#include <error.h>
#include <string.h>
#include "base/net.h"
#include "core/data_handler.h"
#include "core/socket.h"

namespace serverkit {

Socket* 
CreateClientSocket(const Endpoint& endpoint, Poller* poller, DataHandler* handler) {
  return new Socket(endpoint, poller, handler);
}

Socket* 
CreateServerSocket(int fd, DataHandler* handler) {
  return new Socket(fd, handler);
}

// create a server accepted socket
Socket::Socket(int fd, DataHandler* h)
  : fd_(fd),
    handler_(h),
    poller_(NULL),
    is_writable_(false),
    status_(SOCKET_CONNECTED),
    type_(SERVER_SOCKET) {  
  GetEndpointByFd(fd, &endpoint_);
}

// create a client socket
Socket::Socket(const Endpoint& endpoint, Poller* poller, DataHandler* h)
  : fd_(TcpSocket()),
    handler_(h),
    poller_(poller),
    is_writable_(false),
    status_(SOCKET_INIT),
    endpoint_(endpoint),
    type_(CLIENT_SOCKET) {
}

Socket::~Socket() {
  poller_->Del(handle_);
  Socket::close();
}

void 
Socket::Connect() {
  ASSERT_EQUAL(type_, CLIENT_SOCKET);

  Info() << "try connect to " << String();

  if (status_ != SOCKET_INIT) {
    return;
  }

  status_ = SOCKET_CONNECTING;
  int err = serverkit::ConnectAsync(endpoint_, fd_);
  
  if (err == kOK) {
    poller_->Add(fd_, this);
  } else {
    handler_->OnConnect(err);
    status_ = SOCKET_CLOSED;
  }
}

void
Socket::SetPoller(Poller *poller) {
  ASSERT_EQUAL(poller_, NULL);
  poller_ = poller;
  handle_ = poller->Add(fd_, this);
}

void
Socket::close() {
  poller_->Del(handle_);
  serverkit::Close(fd_);
}

void
Socket::In() {
  Debug() << "socket " << String() << " in";
  if (status_ != SOCKET_CONNECTED) {
    return;
  }
  int err;

  while (true) {
    Recv(this, &read_list_, &err);
    Info() << "read_list size:" << read_list_.TotalSize()
      << ", readable size:" << read_list_.ReadableSize();
    if (err != kOK && !IsIOTryAgain(err)) {
      if (handler_) {
        handler_->OnError(err);
      }
      Error() << String() <<" recv error: " << strerror(err);
      Socket::close();
      return;
    } else {
      if (IsIOTryAgain(err)) {
        break;
      }
    }
  }

  if (!read_list_.Empty() && handler_) {
    // if read buffer is not empty, call handler->OnRead
    handler_->OnRead();
  }
}

void
Socket::Out() {
  Info() << "socket " << endpoint_.String() << " out";
  int err, n;

  if (status_ == SOCKET_CONNECTING) {
    status_ = SOCKET_CONNECTED;
    handler_->OnConnect(kOK);
    Info() << "connected to " << endpoint_.String() << " success";
  }

  if (status_ != SOCKET_CONNECTED) {
    Error() << "socket is closed, cannot write data" << status_;
    return;
  }

  while (true) {
    if (write_list_.Empty()) {
      // when write buffer empty, clear the writeable flag
      is_writable_ = false;
      break;
    }

    n = Send(this, &write_list_, &err);
    if (err != kOK && !IsIOTryAgain(err)) {
      if (handler_) {
        handler_->OnError(err);
      }
      Socket::close();
      return;
    } else {
      if (IsIOTryAgain(err)) {
        break;
      }
    }
  }

  if (n > 0 && handler_) {
    // if write some data, call handler->OnWrite
    handler_->OnWrite();
  }
}

void
Socket::Timeout() {
}

void
Socket::Write(const char* from, size_t n) {
  // write data to write buffer
  write_list_.Write(from, n);
  if (!is_writable_) {
    // when write buffer is not empty, set the writeable flag
    is_writable_ = true;
    // mark this handler writeable
    poller_->MarkWriteable(handle_);
  }
}

size_t
Socket::Read(char* to, size_t n) {
  // read data info read buffer
  return read_list_.Read(to, n);
}

};  // namespace serverkit
