/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_PROTOBUF_CHANNEL_H__
#define __SERVERKIT_PROTOBUF_CHANNEL_H__

#include "base/endpoint.h"
#include "rpc/packet.h"
#include "rpc/packet_parser.h"
#include "google/protobuf/message.h"
#include "google/protobuf/service.h"

namespace serverkit {

class ProtobufService;

class ProtobufChannel : public gpb::RpcChannel::RpcChannel {
public:
	ProtobufChannel(const Endpoint& endpoint, Socket* socket, ProtobufService *service);
	virtual ~ProtobufChannel();

	// gpb::RpcChannel::RpcChannel virtual method
  virtual void CallMethod(
      const gpb::MethodDescriptor *method,
      gpb::RpcController *controller,
      const gpb::Message *request,
      gpb::Message *response,
      gpb::Closure *done);	
};

};  // namespace serverkit

#endif  // __SERVERKIT_PROTOBUF_CHANNEL_H__