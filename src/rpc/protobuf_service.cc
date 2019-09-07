/*
 * Copyright (C) codedump
 */

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <google/protobuf/service.h>
#include "base/string.h"
#include "core/listener.h"
#include "core/session.h"
#include "rpc/method_meta.h"
#include "rpc/protobuf_service.h"
#include "rpc/rpc_channel.h"

namespace serverkit {

namespace gpb = ::google::protobuf;

ProtobufService::ProtobufService(const Endpoint&) {

}

ProtobufService::~ProtobufService() {
}

ProtobufChannel* 
ProtobufService::GetChannelByEndpoint(const Endpoint& endpoint) {
  EpChannelMap::iterator iter = channel_map_.find(endpoint);
  if (iter != channel_map_.end()) {
    return iter->second;
  }

  Info() << "create channel for " << endpoint.String();

  Socket *socket = new Socket(NULL);
  ProtobufChannel* channel = new ProtobufChannel(endpoint, socket, this);
  channel_map_[endpoint] = channel;
  guid_channel_map_[channel->guid()] = channel;

  return channel;
}

ProtobufChannel* 
ProtobufService::GetChannelByGuid(uint64_t guid) {
  GUIDChannelMap::iterator iter = guid_channel_map_.find(guid);
  if (iter == guid_channel_map_.end()) {
    Error() << "not found channel for guid " << guid;
    return NULL;
  }

  return iter->second;
}

void 
ProtobufService::Register(gpb::Service* service) {

}

void 
ProtobufService::RunService(const Packet& packet, uint64_t guid) {
  MethodMetaMap::iterator iter = method_map_.find(packet.method_id);
  if (iter == method_map_.end()) {
    Error() << "not found method for " << packet.method_id;
    return;
  }
  
  RpcMeta* meta = iter->second;

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
    NULL);
}

void 
ProtobufService::OnAccept(Session* session) {
  ProtobufChannel* channel = new ProtobufChannel(socket, this);
  const Endpoint& endpoint = channel->GetEndpoint();
  EpChannelMap::iterator iter = channel_map_.find(endpoint);
  if (iter != channel_map_.end()) {
    it->second->HandleError(kRpcExistedSameChannel);
  }

  channel_map_[endpoint] = channel;
  guid_channel_map_[channel->GetGuid] = channel;
  Info() << "Create channel, endpoint: " << endpoint.String() 
    << ", guid:" << channel->GetGuid();
}

void 
ProtobufService::OnError(int error) {

}
};  // namespace serverkit