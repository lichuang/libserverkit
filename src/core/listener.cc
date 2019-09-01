/*
 * Copyright (C) codedump
 */

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "base/endpoint.h"
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

Listener::Listener(const Endpoint& endpoint, Poller* poller,
                   AcceptorHandler *h, SessionFactory *f)
  : endpoint_(endpoint),
    poller_(poller),
    handler_(h),
    factory_(f) {
  Status status;      
  fd_ = Listen(endpoint, kBacklog, &status);
  Assert(fd_ > 0);
  handle_ = poller_->Add(fd_, this, kEventRead);
}

Listener::~Listener() {
  Close(fd_);
}

void
Listener::In() {
  Status status;
  Endpoint endpoint;

  while (true) {
    int fd = Accept(fd_, &endpoint, &status);
    if (status.Ok()) {
      Session *s = factory_->Create(fd, endpoint);
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
