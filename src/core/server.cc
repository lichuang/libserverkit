/*
 * Copyright (C) codedump
 */

#include "base/log.h"
#include "base/time.h"
#include "base/string.h"
#include "core/accept_message.h"
#include "core/io_thread.h"
#include "core/listener.h"
#include "core/server.h"
#include "core/poller.h"
#include "core/epoll.h"
#include "rpc/rpc_service.h"

namespace serverkit {

ServerOption::ServerOption() {

}

Server::Server()
  : index_(0),
    poller_(new Epoll()) {
  Assert(poller_ != NULL);
  poller_->Init(1024);
}

Server::~Server() {
  delete poller_;
  size_t n;
  for (n = 0; n < workers_.size(); ++n) {
    delete workers_[n];
  }
}

void
Server::AddService(const Endpoint& endpoint, AcceptorHandler* handler) {
  string address = endpoint.String();
  Assert(listeners_.find(address) == listeners_.end()) << "duplicate listener for address " << address;

  Listener *listener = new Listener(endpoint, this, handler);
  Assert(listener != NULL);
  listeners_[address] = listener;
  //Infof("listen at %s", listener->String());
}

void 
Server::AddRpcService(const Endpoint& endpoint, gpb::Service* s) {
  const string &address = endpoint.String();
  RpcService *service;
  Listener *listener;

  if (listeners_.find(address) == listeners_.end()) {
    service = new RpcService();
    listener = new Listener(endpoint, this, service);
    listeners_[address] = listener;
  } else {
    listener = listeners_[address];
  }

  service = static_cast<RpcService*>(listener->GetAcceptorHandler());
  service->Register(s);
}

void
Server::Run(const ServerOption& ) {
  Info() << "server running...";

  while (true) {
    poller_->Dispatch();
  }
}

void 
Server::AcceptNewSession(Session* session) {
  index_ = (index_ + 1) % static_cast<int>(workers_.size());

  IOThread *worker = workers_[index_];
  AcceptMessage *msg = new AcceptMessage(session, worker->GetTid(), worker);
  worker->Send(msg);
}

void 
AddService(const Endpoint& endpoint, AcceptorHandler* handler) {
  gServer->AddService(endpoint, handler);
}

void 
AddRpcService(const Endpoint& endpoint, gpb::Service* service) {
  gServer->AddRpcService(endpoint, service);
}

void 
RunServer(const ServerOption& option) {
  gServer->Run(option);
}

};  // namespace serverkit
