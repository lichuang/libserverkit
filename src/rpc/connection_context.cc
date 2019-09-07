/*
 * Copyright (C) codedump
 */

#include "base/endpoint.h"
#include "base/errcode.h"
#include "base/log.h"
#include "base/misc.h"
#include "core/socket.h"
#include "rpc/connection_context.h"
#include "rpc/packet_parser.h"
#include "rpc/request_context.h"
#include "rpc/rpc_controller.h"

namespace serverkit {

ConnectionContext::ConnectionContext(Socket* socket, DataHandler* handler, RpcConnectionType t)
	: socket_(socket),
    guid_(NewGlobalID()),
    allocate_guid_(0),
		type_(t),
	 	parser_(NULL) {
	parser_ = new PacketParser(socket_, handler);	
}

ConnectionContext::~ConnectionContext() {
	delete parser_;
}

void 
ConnectionContext::PushRequestToQueue(
  const gpb::MethodDescriptor *method,
  RpcController *controller,
  const gpb::Message *request,
  gpb::Message *response,
  gpb::Closure *done) {
  uint64_t call_guid = allocateGuid();
  controller->Init(GetGuid(), call_guid);
  packet_queue_.push(new Packet(call_guid, method, request));

  request_context_[call_guid] = new RequestContext(controller, response, done);
}

void 
ConnectionContext::OnWrite() {

}

void
ConnectionContext::OnRead() {
  while (parser_->RecvPacket()) {
    const Packet& packet = parser_->GetPacket();

    Debug() << "read: " << packet.guid
        << ", method id: " << packet.method_id
        << ", content: " << packet.content;

    if (packet.method_id != 0) {
      //service_->RunService(packet, GetGuid());
      continue;
    }

    if (request_context_.find(packet.guid) ==
        request_context_.end()) {
      Error() << "not found request context, request id: "
          << packet.guid
          << "method_id: " << packet.method_id;
      continue;
    }

    RequestContext* context = request_context_[packet.guid];

    bool ret = context->response->ParseFromString(packet.content);
    if (!ret) {
      Error() << "parse response fail: "
          << packet.content
          << " from " << socket_->String();
    }
    request_context_.erase(packet.guid);
    context->Run();
  }
}

void 
ConnectionContext::OnConnect(int error) {
  Assert(type_ == RPC_CHANNEL_TYPE_C2S);

  if (error == kOK) {
    while (!packet_queue_.empty()) {
      parser_->SendPacket(packet_queue_.front());
      packet_queue_.pop();
    }
    return;
  }

  Error() << "connect to " << socket_->String() << " failed: " << error;  
}

void 
ConnectionContext::CallMethod(
  const gpb::MethodDescriptor *method,
  gpb::RpcController *controller,
  const gpb::Message *request,
  gpb::Message *response,
  gpb::Closure *done) {
  SocketStatus status = socket_->Status();

  Debug() << __FUNCTION__ << " status: " 
    << status << request->DebugString();

  if (status == SOCKET_CLOSED) {
    return;
  }

  RpcController *rpc_controller = reinterpret_cast<RpcController*>(controller);

  if (status == SOCKET_CONNECTED) {
    uint64_t call_guid = allocateGuid();
    rpc_controller->Init(GetGuid(), call_guid);

    Packet *packet = new Packet(call_guid, method, request);
    Debug() << "write to socket: " << call_guid << " : " << request->DebugString();

    request_context_[call_guid] = new RequestContext(rpc_controller, response, done);
    parser_->SendPacket(packet);
    return;
  }

  if (status == SOCKET_CONNECTING) {
    PushRequestToQueue(method, rpc_controller, request, response, done);
  }
}

ConnectionContext* 
AcceptRpcConnection(Socket* socket, DataHandler* handler) {
  return new ConnectionContext(socket, handler, RPC_CHANNEL_TYPE_C2S);
}

};  // namespace serverkit