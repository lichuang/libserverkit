/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_RPC_SERVICE_H__
#define __SERVERKIT_RPC_SERVICE_H__

#include <map>
#include <google/protobuf/service.h>
#include "core/acceptor_handler.h"

using namespace std;
namespace gpb = ::google::protobuf;

namespace serverkit {

class RpcMeta;

class RpcService : public AcceptorHandler {
public:
	RpcService();
	virtual ~RpcService();
	
	void Register(gpb::Service* service);
	RpcMeta* GetService(uint64_t method_id);

  virtual Session* OnAccept(int);
  virtual void OnError(int error);

private:
	typedef map<uint64_t, RpcMeta*> MethodMetaMap;
	MethodMetaMap method_map_;
};

};  // namespace serverkit

#endif  // __SERVERKIT_RPC_SERVICE_H__