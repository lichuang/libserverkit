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
class PacketParser;
class ConnectionContext;
struct RequestContext;

class RpcSession : public Session, gpb::RpcChannel::RpcChannel {
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
	RpcSession(int fd, const Endpoint& endpoint);

private:
	ConnectionContext* context_;
};

class RpcSessionFactory : SessionFactory {
public:
  virtual ~RpcSessionFactory() {
  }

  virtual Session* Create(int fd, const Endpoint& endpoint) {
		return new RpcSession(fd, endpoint);
	}
};

};  // namespace serverkit
#endif  // __SERVERKIT_RPC_SESSION_H__