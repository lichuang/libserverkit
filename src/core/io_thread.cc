/*
 * Copyright (C) codedump
 */

#include "base/buffer.h"
#include "base/errcode.h"
#include "base/object_pool.h"
#include "core/accept_message.h"
#include "core/epoll.h"
#include "core/io_thread.h"
#include "core/server.h"
#include "core/session.h"
#include "core/socket.h"
#include "rpc/rpc_channel.h"

namespace serverkit {

IOThread::IOThread(const string &name)
  : poller_(new Epoll()) {
  int rc = poller_->Init(1024);
  if (rc != kOK) {
    return;
  }

  // add mailbox signal fd into poller
  fd_t fd = mailbox_.Fd();
  handle_ = poller_->Add(fd, this);

  thread_ = new Thread(name, this);
}

IOThread::~IOThread() {
  delete poller_;
}

void
IOThread::In() {
  Message* msg;
  int rc = mailbox_.Recv(&msg, 0);

  while (rc == 0 || errno == EINTR) {
    if (rc == 0)  {
      msg->Process();
      delete msg;
    }
    rc = mailbox_.Recv(&msg, 0);
  }
}

void
IOThread::Out() {
  // nothing to do
}

void
IOThread::Timeout() {
  // nothing to do
}

void 
IOThread::processAcceptMessage(Message* msg) {
  AcceptMessage* am = static_cast<AcceptMessage*>(msg);
  Session* session = am->GetSession();
  //Infof("process connection from %s", session->String().c_str());  
  session->SetPoller(poller_);
}

void 
IOThread::processRpcChannelMessage(Message* msg) {
  RpcChannelMessage* am = static_cast<RpcChannelMessage*>(msg);
  RpcChannel* channel = new RpcChannel(am->endpoint_, poller_);
  am->done_(channel);
}

void
IOThread::Process(Message *msg) {
  int type = msg->Type();

  Info() << "Process message " << type;
  
  switch (type) {
  case kAcceptMessage:
    processAcceptMessage(msg);
    break;
  case kRpcChannelMessage:
    processRpcChannelMessage(msg);
    break;
  default:
    Error() << "wrong message type " << type;
    break;
  }

}

void
IOThread::Send(Message *msg) {
  mailbox_.Send(msg);
}

void 
IOThread::Start() {
  thread_->Start();
}

void
IOThread::Run() {
  while (thread_->Running()) {
    poller_->Dispatch();
  }
}

};  // namespace serverkit
