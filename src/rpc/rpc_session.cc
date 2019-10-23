/*
 * Copyright (C) codedump
 */

#include "rpc/request_context.h"
#include "rpc/rpc_controller.h"
#include "rpc/rpc_meta.h"
#include "rpc/rpc_service.h"
#include "rpc/rpc_session.h"
#include "rpc/packet_parser.h"

namespace serverkit {

struct responseContext {
  uint64_t channel_guid;
  uint64_t guid;
  const gpb::Message *request;
  gpb::Message *response;

  responseContext(uint64_t cguid, uint64_t g,
    const gpb::Message *req, gpb::Message *resp)
      : channel_guid(cguid), guid(g),
        request(req), response(resp) { 
  }

  ~responseContext() {
    delete request;
    delete response;
  }
};

RpcSession::RpcSession(int fd)
	: Session(fd),
		parser_(NULL),
    guid_(0),
    allocate_guid_(0) {
  parser_ = new PacketParser(socket_);		
}

RpcSession::~RpcSession() {
	delete parser_;
}

void 
RpcSession::OnWrite() {
	Session::OnWrite();
}

void 
RpcSession::OnRead() {
	Session::OnRead();

  while (parser_->RecvPacket()) {
    const Packet& packet = parser_->GetPacket();

    Debug() << "read: " << packet.guid
        << ", method id: " << packet.method_id
        << ", content: " << packet.content;

    if (packet.method_id != 0) {
      RunService(packet, GetGuid());
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
RpcSession::OnError(int err) {

}

void 
RpcSession::RunService(const Packet& packet, uint64_t channel_guid) {  
  RpcMeta* meta = service_->GetService(packet.method_id);
  if (meta == NULL) {
    Error() << "not found method for " << packet.method_id;
    return;
  }

  gpb::Message* request = meta->GetRequestPrototype().New();
  if (!request->ParseFromString(packet.content)) {
    Error() << "request ParseFromString fail: " << packet.guid;
    delete request;
    return;
  }
  gpb::Message* response = meta->GetResponsePrototype().New();

  meta->GetService()->CallMethod(
    &meta->GetMethodDescriptor(), 
    reinterpret_cast<gpb::RpcController*>(NULL),
    request, response,
    gpb::NewCallback(this, &RpcSession::onResponse,
                     new responseContext(channel_guid, packet.guid,
                     request, response)));
}

void 
RpcSession::CallMethod(
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

  uint64_t call_guid = allocateGuid();
  rpc_controller->Init(GetGuid(), call_guid);

  Packet *packet = new Packet(call_guid, method, request);
  Debug() << "write to socket: " << call_guid << " : " << request->DebugString();

  request_context_[call_guid] = new RequestContext(rpc_controller, response, done);
  parser_->SendPacket(packet);
}

void 
RpcSession::onResponse(responseContext* context) {
  gpb::Message* response = context->response;
  Packet *packet = new Packet(context->guid, NULL, response);
  parser_->SendPacket(packet);

  delete context;
}

};  // namespace serverkit