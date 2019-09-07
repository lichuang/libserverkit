/*
 * Copyright (C) codedump
 */

#include "rpc/connection_context.h"
#include "rpc/rpc_session.h"
#include "rpc/packet_parser.h"

namespace serverkit {

RpcSession::RpcSession(int fd, const Endpoint& endpoint)
	: Session(fd, endpoint),
		context_(NULL) {
  context_ = AcceptRpcConnection(socket_, this);		
}

RpcSession::~RpcSession() {
	delete context_;
}

void 
RpcSession::OnWrite() {
	Session::OnWrite();
  context_->OnWrite();
}

void 
RpcSession::OnRead() {
	Session::OnRead();
  context_->OnRead();
}

void 
RpcSession::OnError(int err) {

}

void 
RpcSession::CallMethod(
  const gpb::MethodDescriptor *method,
  gpb::RpcController *controller,
  const gpb::Message *request,
  gpb::Message *response,
  gpb::Closure *done) {
  context_->CallMethod(method, controller, request, response, done);
}

};  // namespace serverkit