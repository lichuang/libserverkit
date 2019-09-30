/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_RPC_CHANNEL_H__
#define __SERVERKIT_RPC_CHANNEL_H__

#include <map>
#include <queue>
#include <google/protobuf/message.h>
#include <google/protobuf/service.h>
#include "base/endpoint.h"
#include "core/data_handler.h"
#include "core/message.h"
#include "core/application.h"

using namespace std;
namespace gpb = ::google::protobuf;
namespace serverkit {

class Socket;
class Packet;
class PacketParser;
class Poller;
class RpcController;
struct RequestContext;

struct RpcChannelOption {

};

class RpcChannel :  public gpb::RpcChannel::RpcChannel,
                    public DataHandler {
public:
	RpcChannel(const Endpoint& endpoint, Poller*);
	virtual ~RpcChannel();

	// gpb::RpcChannel::RpcChannel virtual method
  virtual void CallMethod(
      const gpb::MethodDescriptor *method,
      gpb::RpcController *controller,
      const gpb::Message *request,
      gpb::Message *response,
      gpb::Closure *done);

	virtual void OnWrite();
  virtual void OnRead();
  virtual void OnConnect(int error);
  virtual void OnError(int error);

private:
  void pushRequestToQueue(
      const gpb::MethodDescriptor *method,
      RpcController *controller,
      const gpb::Message *request,
      gpb::Message *response,
      gpb::Closure *done);
  
  uint64_t allocateGuid() {
    return ++allocate_guid_;
  }

  uint64_t GetGuid() const { 
    return guid_; 
  }

private:
	Socket *socket_;
	PacketParser* parser_;
  uint64_t guid_;
  uint64_t allocate_guid_;
	queue<Packet*> packet_queue_;

	typedef map<uint64_t, RequestContext*> RequestContextMap;
	RequestContextMap request_context_;
  Endpoint endpoint_;
  Poller *poller_;			
};

class RpcChannelMessage : public Message {
public:
  RpcChannelMessage(const Endpoint& endpoint, tid_t tid, MessageHandler *h, CreateChannelDone done)
    : Message(kRpcChannelMessage, tid, h),
      endpoint_(endpoint),
      done_(done) {
  }

  virtual ~RpcChannelMessage() {
  }

  Endpoint endpoint_;
  CreateChannelDone done_;
};

};  // namespace serverkit

#endif  // __SERVERKIT_RPC_CHANNEL_H__