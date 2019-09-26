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
#include "base/status.h"
#include "base/string.h"
#include "core/acceptor_handler.h"
#include "core/config.h"
#include "core/listener.h"
#include "core/session.h"
#include "core/server.h"

namespace serverkit {

Listener::Listener(const Endpoint& endpoint, Server* server,
                   AcceptorHandler *h)
  : endpoint_(endpoint),
    server_(server),
    poller_(server->GetPoller()),
    handler_(h) {
  Status status;      
  fd_ = Listen(endpoint, kBacklog, &status);
  Assert(fd_ > 0) << "status:" << status.String();
  handle_ = poller_->Add(fd_, this);
}

Listener::~Listener() {
  Close(fd_);
}

void
Listener::In() {
  Status status;

  while (true) {
    int fd = Accept(fd_, &status);
    if (status.Ok()) {
      Session* session = handler_->OnAccept(fd);
      server_->AcceptNewSession(session);
    //} else if (status.IsTryAgain()) {
    //  break;
    } else {
      handler_->OnError(status.ErrorNum());
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
