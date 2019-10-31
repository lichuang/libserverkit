/*
 * Copyright (C) codedump
 */

#include "base/log.h"
#include "util/time.h"
#include "base/string.h"
#include "core/accept_message.h"
#include "core/listener.h"
#include "core/application.h"
#include "core/poller.h"
#include "core/epoll.h"
#include "core/worker.h"
#include "rpc/rpc_channel.h"
#include "rpc/rpc_service.h"

namespace serverkit {

Application::Application()
  : index_(0),
    poller_(new Epoll()) {
  ASSERT(poller_ != NULL);
  poller_->Init(1024);
}

Application::~Application() {
  delete poller_;
  size_t n;
  for (n = 0; n < workers_.size(); ++n) {
    delete workers_[n];
  }
}

void
Application::AddService(const Endpoint& endpoint, AcceptorHandler* handler) {
  string address = endpoint.String();
  ASSERT(listeners_.find(address) == listeners_.end()) << "duplicate listener for address " << address;

  Listener *listener = new Listener(endpoint, this, handler);
  ASSERT(listener != NULL);
  listeners_[address] = listener;
  //Infof("listen at %s", listener->String());
}

void 
Application::AddRpcService(const Endpoint& endpoint, gpb::Service* s) {
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
Application::Init(const ServerkitOption& options) {
  int i;
  // start worker threads
  for (i = 0; i < options.worker_num; ++i) {
    Worker* thread = new Worker(Stringf("worker-%d", i));
    thread->Start();
    workers_.push_back(thread);
  }
}

void
Application::Run() {
  Info() << "server running...";

  while (true) {
    poller_->Dispatch();
  }
}

// return a worker in round robin mode
Worker* 
Application::selectWorker() {
  index_ = (index_ + 1) % static_cast<int>(workers_.size());

  return workers_[index_];
}

void 
Application::AcceptNewSession(Session* session) {
  // select a worker to process the session
  Worker* worker = selectWorker();

  // send it in AcceptMessage to the worker
  AcceptMessage *msg = new AcceptMessage(session, worker->GetTid(), worker);
  worker->Send(msg);
}

void
Application::CreateRpcChannel(const Endpoint& endpoint, CreateChannelDone done) {
  // select a worker to process the sessio
  Worker* worker = selectWorker();

  // send it in RpcChannelMessage to the worker
  RpcChannelMessage *msg = new RpcChannelMessage(endpoint, worker->GetTid(), worker, done);
  worker->Send(msg);
}

};  // namespace serverkit
