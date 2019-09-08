/*
 * Copyright (C) codedump
 */

#include "base/log.h"
#include "base/misc.h"
#include "core/socket.h"
#include "rpc/rpc_channel.h"
#include "rpc/rpc_controller.h"
#include "rpc/rpc_service.h"

namespace serverkit {

ProtobufChannel::ProtobufChannel(Socket* socket, ProtobufService *service) 
	: socket(socket), 
    service(service),
    guid_(NewGlobalID()),
    allocate_guid_(0), 
    parser_(new ProtobufParser(socket)) {
}

ProtobufChannel::~ProtobufChannel() {
  guid_ = 0;
  delete parser_;
}

void 
ProtobufChannel::CallMethod(
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
    rpc_controller->Init(guid(), call_guid);

    Packet *packet = new Packet(call_guid, method, request);
    Debug() << "write to socket: " << call_guid << " : " << request->DebugString();

    request_context_[call_guid] = new RequestContext(rpc_controller, response, done);
    packet_parser_->SendPacket(packet);
    return;
  }

  if (status == SOCKET_CONNECTING) {
    pushRequestToQueue(method, rpc_controller, request, response, done);
  }
}

void 
ProtobufChannel::pushRequestToQueue(
  const gpb::MethodDescriptor *method,
  RpcController *controller,
  const gpb::Message *request,
  gpb::Message *response,
  gpb::Closure *done) {
  uint64_t call_guid = allocateGuid();
  controller->Init(guid(), call_guid);
  packet_queue_.push(new Packet(call_guid, method, request));

  request_context_[call_guid] = new RequestContext(controller, response, done));
}

void 
ProtobufChannel::OnConnect(int error) {
  if (status.Ok()) {
    while (!packet_queue_.empty()) {
      packet_parser_->SendPacket(packet_queue_.front());
      packet_queue_.pop();
    }
    return;
  }

  Error() << "connect to " << endpoint_.String() << " failed: " << status.String();
}

void 
ProtobufChannel::OnRead() { 
  while (packet_parser_->RecivePacket()) {
    const Packet& packet = packet_parser_->GetPacket();

    Debug() << "read: " << packet.guid
        << ", method id: " << packet.method_id
        << ", content: " << packet.content;

    if (packet.method_id != 0) {
      service_->RunService(packet, guid());
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
          << string_utility::StringToHex(packet.content)
          << " from " << socket_->GetEndpoint().String();
    }
    request_context_.erase(packet.guid);
    context->Run();
  }
}

void 
ProtobufChannel::OnError(int error) {

}

void 
ProtobufChannel::Run() {
  RequestContextMap::iterator it;
  for (it = request_context_.begin(); it != request_context_.end();) {
    RequestContext* context = it->second;
    request_context_.erase(it++);
    context->Run();
  }
}

void 
ProtobufChannel::Cancel(uint64_t call_guid) {
  RequestContextMap::iterator iter = request_context_.find(call_guid);
  if (iter == request_context_.end()) {
    return;
  }
  delete iter->second;
  request_context_.erase(iter);
}

};  // namespace serverkit