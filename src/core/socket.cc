/*
 * Copyright (C) codedump
 */

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
    poller_->Add(fd_, this, kEventWrite | kEventRead);
  } else {
    handler_->OnConnect(err);
    status_ = SOCKET_CLOSED;
  }
}

void
Socket::SetPoller(Poller *poller) {
  poller_ = poller;
  handle_ = poller->Add(fd_, this, kEventRead);
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
    int n = Recv(this, &read_list_, &err);
    if (err != kOK && !IsIOTryAgain(err)) {
      if (handler_) {
        handler_->OnError(err);
      }
      CloseSocket();
      return;
    } else {
      read_list_.WriteAdvance(n);
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
  int err;

  if (status_ == SOCKET_CONNECTING) {
    status_ = SOCKET_CONNECTED;
    handler_->OnConnect(kOK);
    Info() << "connected to " << endpoint_.String();
    return;
  }

  if (status_ != SOCKET_CONNECTED) {
    Error() << "socket is closed, cannot write data" << status_;
    return;
  }

  while (true) {
    if (write_list_.Empty()) {
      is_writable_ = true;
      poller_->ResetOut(handle_);
      break;
    }

    int n = Send(fd_, &write_list_, &err);
    if (n < 0) {
      CloseSocket();
      if (handler_) {
        handler_->OnError(err);
      }
      return;
    } else {
      write_list_.ReadAdvance(n);
      if (IsIOTryAgain(err)) {
        is_writable_ = false;
        break;
      }
    }
  }

  if (handler_) {
    handler_->OnWrite();
  }
}

void
Socket::Timeout() {
}

void
Socket::Write(const char* from, size_t n) {
  write_list_.Write(from, n);
  poller_->SetOut(handle_);
}

size_t
Socket::Read(char* to, size_t n) {
  return read_list_.Read(to, n);
}

int
Socket::ResetIn() {
  return poller_->ResetIn(handle_);
}

int
Socket::SetIn() {
  return poller_->SetIn(handle_);
}

int
Socket::ResetOut() {
  return poller_->ResetOut(handle_);
}

int
Socket::SetOut() {
  return poller_->SetOut(handle_);
}

};  // namespace serverkit
