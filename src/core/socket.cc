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
    status_(SOCKET_INIT) {
  //Infof("addr: %s, fd: %d", addr.c_str(), fd);   
  GetEndpointByFd(fd, &endpoint_);
}

Socket::~Socket() {
  CloseSocket();
}

void 
Socket::Connect(const Endpoint& endpoint) {
  status_ = SOCKET_CONNECTING;
  serverkit::Status status;
  serverkit::Connect(endpoint, &status, fd_);

  if (status.IsTryAgain()) {
    poller_->Add(fd_, this, kEventWrite);
  } else if (!status.Ok()) {
    handler_->OnConnect(status.ErrorNum());
    status_ = SOCKET_INIT;
  } else {
    status_ = SOCKET_CONNECTED;
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
  serverkit::Status status;

  while (true) {
    int n = Recv(fd_, &read_list_, &status);
    if (n < 0) {
      if (handler_) {
        handler_->OnError(status.ErrorNum());
      }
      CloseSocket();
      break;
    } else {
      read_list_.WriteAdvance(n);
      if (status.IsTryAgain()) {
        break;
      }
    }
  }

  if (handler_) {
    handler_->OnRead();
  }
}

void
Socket::Out() {
  serverkit::Status status;

  if (status_ == SOCKET_CONNECTING) {
    handler_->OnConnect(Status());
    status_ = SOCKET_CONNECTED;
  }

  if (status_ != SOCKET_CONNECTED) {
    Error() << "socket is closed, cannot read data" << status_;
    return;
  }

  while (true) {
    if (write_list_.Empty()) {
      is_writable_ = true;
      poller_->ResetOut(handle_);
      break;
    }

    int n = Send(fd_, &write_list_, &status);
    if (n < 0) {
      CloseSocket();
      if (handler_) {
        handler_->OnError(status.ErrorNum());
      }
      return;
    } else {
      write_list_.ReadAdvance(n);
      if (status.IsTryAgain()) {
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
