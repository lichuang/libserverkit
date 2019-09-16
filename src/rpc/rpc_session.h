/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_RPC_SESSION_H__
#define __SERVERKIT_RPC_SESSION_H__

#include <google/protobuf/message.h>
#include <google/protobuf/service.h>
#include "core/session.h"

namespace gpb = ::google::protobuf;

namespace serverkit {

class RpcSessionFactory;
class Packet;
class PacketParser;
class RpcService;
struct RequestContext;

class RpcSession :  public Session, 
                    public gpb::RpcChannel::RpcChannel {
	friend class RpcSessionFactory;
public:
	virtual ~RpcSession();

  virtual void OnWrite();

  virtual void OnRead();

  virtual void OnError(int err);
  
	// gpb::RpcChannel::RpcChannel virtual method
  virtual void CallMethod(
      const gpb::MethodDescriptor *method,
      gpb::RpcController *controller,
      const gpb::Message *request,
      gpb::Message *response,
      gpb::Closure *done);
			
private:
	RpcSession(int fd);
  uint64_t GetGuid() const { 
    return guid_; 
  }
  uint64_t allocateGuid() {
    return ++allocate_guid_;
  }
  void RunService(const Packet& packet, uint64_t channel_guid);
private:
  PacketParser* parser_;
  uint64_t guid_;
  uint64_t allocate_guid_;
	typedef map<uint64_t, RequestContext*> RequestContextMap;
	RequestContextMap request_context_;  
  RpcService *service_;
};

class RpcSessionFactory : public SessionFactory {
public:
  RpcSessionFactory()
    : SessionFactory() {}

  virtual ~RpcSessionFactory() {
  }

  virtual Session* NewSession(int fd) {
		return new RpcSession(fd);
	}
};

};  // namespace serverkit
#endif  // __SERVERKIT_RPC_SESSION_H__