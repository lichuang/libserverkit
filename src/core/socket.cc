/*
 * Copyright (C) codedump
 */

#include <error.h>
#include <string.h>
#include "base/net.h"
#include "base/status.h"
#include "core/data_handler.h"
#include "core/socket.h"

namespace serverkit {

Socket::Socket(int fd, DataHandler* h)
  : fd_(fd),
    handler_(h),
    poller_(NULL),
    is_writable_(false),
    status_(SOCKET_CONNECTED) {  
  GetEndpointByFd(fd, &endpoint_);
}

Socket::Socket(Poller* poller, DataHandler* h)
  : fd_(TcpSocket()),
    handler_(h),
    poller_(poller),
    is_writable_(false),
    status_(SOCKET_INIT) {
}

Socket::~Socket() {
  CloseSocket();
}

void 
Socket::Connect(const Endpoint& endpoint) {
  endpoint_ = endpoint;
  Info() << "try connect to " << String();

  if (status_ != SOCKET_INIT) {
    return;
  }

  status_ = SOCKET_CONNECTING;
  int err = serverkit::ConnectAsync(endpoint, fd_);
  
  if (err == kOK) {
    poller_->Add(fd_, this);
  } else {
    handler_->OnConnect(err);
    status_ = SOCKET_CLOSED;
  }
}

void
Socket::SetPoller(Poller *poller) {
  poller_ = poller;
  handle_ = poller->Add(fd_, this);
}

void
Socket::CloseSocket() {
  poller_->Del(handle_);
  Close(fd_);
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
      CloseSocket();
      return;
    } else {
      if (IsIOTryAgain(err)) {
        break;
      }
    }
  }

  if (!read_list_.Empty() && handler_) {
    handler_->OnRead();
  }
}

void
Socket::Out() {
  Info() << "socket " << endpoint_.String() << " out";
  serverkit::Status status;
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
      is_writable_ = false;
      break;
    }

    n = Send(this, &write_list_, &err);
    if (err != kOK && !IsIOTryAgain(err)) {
      if (handler_) {
        handler_->OnError(err);
      }
      CloseSocket();
      return;
    } else {
      if (IsIOTryAgain(err)) {
        break;
      }
    }
  }

  if (n > 0 && handler_) {
    handler_->OnWrite();
  }
}

void
Socket::Timeout() {
}

void
Socket::Write(const char* from, size_t n) {
  write_list_.Write(from, n);
  if (!is_writable_) {
    is_writable_ = true;
    poller_->MarkWriteable(handle_);
  }
}

size_t
Socket::Read(char* to, size_t n) {
  return read_list_.Read(to, n);
}

};  // namespace serverkit
