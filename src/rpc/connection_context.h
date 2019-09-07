/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_RPC_CONNECTION_CONTEXT_H__
#define __SERVERKIT_RPC_CONNECTION_CONTEXT_H__

#include <map>
#include <queue>
#include "google/protobuf/message.h"
#include "google/protobuf/service.h"

using namespace std;
namespace gpb = ::google::protobuf;

namespace serverkit {

enum RpcConnectionType { 
	RPC_CHANNEL_TYPE_C2S,
	RPC_CHANNEL_TYPE_S2C
};

class DataHandler;
class Socket;
class Packet;
class PacketParser;
class RpcController;
struct RequestContext;

class ConnectionContext {
public:
	ConnectionContext(Socket*, DataHandler* handler, RpcConnectionType);
	~ConnectionContext();

  void OnWrite();

  void OnRead();

	void OnConnect(int error);

  void CallMethod(
      const gpb::MethodDescriptor *method,
      gpb::RpcController *controller,
      const gpb::Message *request,
      gpb::Message *response,
      gpb::Closure *done);

  void PushRequestToQueue(
      const gpb::MethodDescriptor *method,
      RpcController *controller,
      const gpb::Message *request,
      gpb::Message *response,
      gpb::Closure *done);

  uint64_t GetGuid() const { 
    return guid_; 
  }
  
private:
  uint64_t allocateGuid() {
    return ++allocate_guid_;
  }
private:
  Socket *socket_;
	uint64_t guid_;
	uint64_t allocate_guid_;
	RpcConnectionType type_;
	PacketParser* parser_;
	queue<Packet*> packet_queue_;

	typedef map<uint64_t, RequestContext*> RequestContextMap;
	RequestContextMap request_context_;
};

extern ConnectionContext* AcceptRpcConnection(Socket* socket, DataHandler*);

};  // namespace serverkit

#endif  // __SERVERKIT_RPC_CONNECTION_CONTEXT_H__