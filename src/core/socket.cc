/*
 * Copyright (C) codedump
 */

#include "base/net.h"
#include "base/status.h"
#include "core/data_handler.h"
#include "core/socket.h"

namespace serverkit {

Socket::Socket(int fd, const string& addr, DataHandler* h)
  : fd_(fd),
    handler_(h),
    poller_(NULL),
    is_writable_(false),
    addr_(addr) {
  //Infof("addr: %s, fd: %d", addr.c_str(), fd);      
}

Socket::~Socket() {
  CloseSocket();
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
  Status status;

  while (true) {
    int n = Recv(fd_, &read_list_, &status);
    if (n < 0) {
      if (handler_) {
        handler_->OnError(status);
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
  Status status;

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
        handler_->OnError(status);
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
