/*
 * Copyright (C) codedump
 */

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "base/endpoint.h"
#include "base/errcode.h"
#include "base/log.h"
#include "base/net.h"
#include "base/string.h"
#include "core/acceptor_handler.h"
#include "core/config.h"
#include "core/listener.h"
#include "core/session.h"
#include "core/application.h"

namespace serverkit {

Listener::Listener(const Endpoint& endpoint, Application* application,
                   AcceptorHandler *h)
  : endpoint_(endpoint),
    application_(application),
    poller_(application->GetPoller()),
    handler_(h) {
  int ret;
  fd_ = Listen(endpoint, kBacklog, &ret);
  Assert(ret == kOK);
  handle_ = poller_->Add(fd_, this);
}

Listener::~Listener() {
  Close(fd_);
}

void
Listener::In() {
  int err;

  while (true) {
    int fd = Accept(fd_, &err);
    if (err == kOK) {
      Session* session = handler_->OnAccept(fd);
      application_->AcceptNewSession(session);
    //} else if (status.IsTryAgain()) {
    //  break;
    } else {
      handler_->OnError(err);
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
