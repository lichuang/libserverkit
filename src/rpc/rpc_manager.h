/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_RPC_MANAGER_H__
#define __SERVERKIT_RPC_MANAGER_H__

#include <map>
#include <google/protobuf/service.h>
#include "base/singleton.h"

using namespace std;
namespace gpb = ::google::protobuf;

namespace serverkit {

class RpcMeta;

class RpcManager {
friend class Singleton<RpcManager>;
public:
	~RpcManager();
	
	void Register(gpb::Service* service);
	RpcMeta* GetService(uint64_t method_id);

private:
	RpcManager();
  void doInit() {
    // nothing to do
  }

private:
	typedef map<uint64_t, RpcMeta*> MethodMetaMap;
	MethodMetaMap method_map_;
};

#define gRpcManager Singleton<RpcManager>::Instance()

extern void RegisterRpcService(gpb::Service* service);
extern RpcMeta* GetRpcService(uint64_t);

};  // namespace serverkit

#endif  // __SERVERKIT_RPC_MANAGER_H__