/*
 * Copyright (C) codedump
 */

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "base/error.h"
#include "base/errcode.h"
#include "base/net.h"
#include "base/status.h"
#include "base/string.h"
#include "core/acceptor_handler.h"
#include "core/config.h"
#include "core/listener.h"
#include "core/session.h"

namespace serverkit {

Listener::Listener(const string& addr, int port, Poller* poller,
                   AcceptorHandler *h, SessionFactory *f)
  : addr_(addr),
    port_(port),
    poller_(poller),
    handler_(h),
    factory_(f) {
  Status status;      
  fd_ = Listen(addr, port, kBacklog, &status);
  Assert(fd_ > 0);
  handle_ = poller_->Add(fd_, this, kEventRead);
  Stringf(&string_, "%s:%d", addr.c_str(), port);
}

Listener::~Listener() {
  Close(fd_);
}

void
Listener::In() {
  Status status;
  string addr;

  while (true) {
    int fd = Accept(fd_, &addr, &status);
    if (status.Ok()) {
      Session *s = factory_->Create(fd, addr);
      handler_->OnAccept(s);
    } else if (status.IsTryAgain()) {
      break;
    } else {
      handler_->OnError(status);
      //Errorf("accept connection error: %s", strerror(error));
    }
  }
}

void
Listener::Out() {
  Assert(false);
}

void
Listener::Timeout() {
  Assert(false);
}

};  // namespace serverkit
