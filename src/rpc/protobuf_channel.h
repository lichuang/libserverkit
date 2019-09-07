/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_PROTOBUF_CHANNEL_H__
#define __SERVERKIT_PROTOBUF_CHANNEL_H__

#include <map>
#include <queue>
#include "base/endpoint.h"
#include "base/macros.h"
#include "core/data_handler.h"
#include "core/socket.h"
#include "rpc/packet.h"
#include "rpc/packet_parser.h"
#include "google/protobuf/message.h"
#include "google/protobuf/service.h"

using namespace std;

namespace serverkit {

namespace gpb = ::google::protobuf;

struct RequestContext;

class ProtobufService;

class ProtobufChannel : public gpb::RpcChannel::RpcChannel, public DataHandler {
public:
	ProtobufChannel(Socket* socket, ProtobufService *service);
	virtual ~ProtobufChannel();

	// gpb::RpcChannel::RpcChannel virtual method
  virtual void CallMethod(
      const gpb::MethodDescriptor *method,
      gpb::RpcController *controller,
      const gpb::Message *request,
      gpb::Message *response,
      gpb::Closure *done);

  virtual void OnConnect(int error);
  virtual void OnRead();
  virtual void OnError(int error);

  void Run();
  void Cancel(uint64_t call_guid);

  uint64_t GetGuid() const { 
    return guid_; 
  }
  
  const Endpoint& GetEndpoint() const { 
    return socket_->GetEndpoint();
  }
private:
  uint64_t allocateGuid();

  void pushRequestToQueue(
      const gpb::MethodDescriptor *method,
      RpcController *controller,
      const gpb::Message *request,
      gpb::Message *response,
      gpb::Closure *done);

private:
  typedef map<uint64_t, RequestContext*> RequestContextMap;
  
  Socket *socket_;  
  ProtobufService *service_;
  uint64_t guid_;

  // Packet buffer queue, when not connected, queue the packet
  queue<Packet*> packet_queue_;
  PacketParser *parser_;

  RequestContextMap request_context_;

  DISALLOW_COPY_AND_ASSIGN(ProtobufChannel);
};

};  // namespace serverkit

#endif  // __SERVERKIT_PROTOBUF_CHANNEL_H__