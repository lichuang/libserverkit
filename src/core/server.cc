/*
 * Copyright (C) codedump
 */

#include "base/error.h"
#include "base/log.h"
#include "base/time.h"
#include "base/string.h"
#include "core/accept_message.h"
#include "core/io_thread.h"
#include "core/listener.h"
#include "core/server.h"
#include "core/poller.h"
#include "core/epoll.h"

namespace serverkit {

Server::Server(int worker)
  : index_(0),
    poller_(new Epoll()) {
  Assert(poller_ != NULL);
  Assert(worker > 0);
  poller_->Init(1024);

  int n;
  for (n = 0; n < worker; ++n) {
    /*
    string name;
    Stringf(&name, "lua-worker-%d", n + 1);
    ScriptIOThread *worker = new ScriptIOThread(name);
    
    Assert(worker != NULL);
    workers_.push_back(worker);
    */
  }
}

Server::~Server() {
  delete poller_;
  size_t n;
  for (n = 0; n < workers_.size(); ++n) {
    delete workers_[n];
  }
}

void
Server::OnAccept(Session* s) {
  index_ = (index_ + 1) % static_cast<int>(workers_.size());

  IOThread *worker = workers_[index_];
  AcceptMessage *msg = new AcceptMessage(s, worker->GetTid(), worker);
  worker->Send(msg);
}

void
Server::OnError(int err) {
}

void
Server::Listen(const string& addr, int port, SessionFactory* f) {
  Listener *listener = new Listener(addr, port, poller_, this, f);
  Assert(listener != NULL);
  listeners_[listener->String()] = listener;
  //Infof("listen at %s", listener->String());
}

void
Server::Run() {
  Info() << "server running...";

  while (true) {
    poller_->Dispatch();
  }
}

};  // namespace serverkit
