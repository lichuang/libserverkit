/*
 * Copyright (C) codedump
 */

#include "base/errcode.h"
#include "base/misc.h"
#include "core/poller.h"
#include "core/socket.h"
#include "rpc/packet_parser.h"
#include "rpc/request_context.h"
#include "rpc/rpc_channel.h"
#include "rpc/rpc_controller.h"

namespace serverkit {

RpcChannel::RpcChannel(const Endpoint& endpoint, Poller *poller)
	: socket_(new Socket(poller, this)),
    parser_(NULL),
    guid_(NewGlobalID()),
    allocate_guid_(0),
    endpoint_(endpoint),
    poller_(poller) {
	parser_ = new PacketParser(socket_, this);	
}

RpcChannel::~RpcChannel() {
	delete parser_;
	delete socket_;
}

void 
RpcChannel::pushRequestToQueue(
  const gpb::MethodDescriptor *method,
  RpcController *controller,
  const gpb::Message *request,
  gpb::Message *response,
  gpb::Closure *done) {
  Info() << "pushRequestToQueue";

  uint64_t call_guid = allocateGuid();
  controller->Init(GetGuid(), call_guid);
  packet_queue_.push(new Packet(call_guid, method, request));

  request_context_[call_guid] = new RequestContext(controller, response, done);
}

void 
RpcChannel::OnWrite() { 

}

void 
RpcChannel::OnRead() {
  Info() << "RpcChannel::OnRead";
  while (parser_->RecvPacket()) {
    const Packet& packet = parser_->GetPacket();

    Debug() << "read: " << packet.guid
        << ", method id: " << packet.method_id
        << ", content: " << packet.content;

    if (packet.method_id != 0) {
      Error() << "receive request packet " << packet.method_id;
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
    if (!context) {
      return;
    }
    bool ret = context->response->ParseFromString(packet.content);

    request_context_.erase(packet.guid);
    if (!ret) {
      Error() << "parse response "
          << StringToHex(packet.content)
          << " from " << socket_->String() << " failed";
    } else {
      context->Run();
    }
    delete context;  
  }
}
  
void 
RpcChannel::OnConnect(int error) {
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
RpcChannel::OnError(int error) {
}

void 
RpcChannel::CallMethod(
  const gpb::MethodDescriptor *method,
  gpb::RpcController *controller,
  const gpb::Message *request,
  gpb::Message *response,
  gpb::Closure *done) {
  RpcController *rpc_controller = reinterpret_cast<RpcController*>(controller);

  SocketStatus status = socket_->Status();

  Debug() << __FUNCTION__ << " status: " 
    << status << request->DebugString();

  if (status == SOCKET_CLOSED) {
    return;
  }

  if (status == SOCKET_CONNECTED) {
    uint64_t call_guid = allocateGuid();
    rpc_controller->Init(GetGuid(), call_guid);

    Packet *packet = new Packet(call_guid, method, request);
    Debug() << "write to socket: " << call_guid << " : " << request->DebugString();

    request_context_[call_guid] = new RequestContext(rpc_controller, response, done);
    parser_->SendPacket(packet);
    return;
  }

  if (status == SOCKET_INIT) {
    Info() << "rpc channel start connect, guid:" << GetGuid()
      << ", endpoint: " << endpoint_.String();

    pushRequestToQueue(method, rpc_controller, request, response, done);
    socket_->Connect(endpoint_);
  }

  if (status == SOCKET_CONNECTING) {
    Debug() << "connecting";
    pushRequestToQueue(method, rpc_controller, request, response, done);
  }
}

};  // namespace serverkit