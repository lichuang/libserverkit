/*
 * Copyright (C) codedump
 */

#include <google/protobuf/descriptor.h>
#include "base/log.h"
#include "base/string.h"
#include "rpc/rpc_manager.h"
#include "rpc/rpc_meta.h"

namespace serverkit {

RpcManager::RpcManager() {

}

RpcManager::~RpcManager() {

}

RpcMeta* 
RpcManager::GetService(uint64_t method_id) {
  MethodMetaMap::iterator iter = method_map_.find(method_id);
  if (iter == method_map_.end()) {
    Error() << "not found method for " << method_id;
    return NULL;
  }

	return iter->second;
}

void 
RpcManager::Register(gpb::Service* service) {
  const gpb::ServiceDescriptor* serviceDescriptor =
      service->GetDescriptor();

  for (int i = 0; i < serviceDescriptor->method_count(); ++i) {
    const gpb::MethodDescriptor* methodDescriptor =
        serviceDescriptor->method(i);    
		uint64_t method_id = HashString(methodDescriptor->full_name());
		Assert(method_map_.find(method_id) == method_map_.end()) 
			<< "register duplicate method " << methodDescriptor->full_name();
    
    RpcMeta* meta = new RpcMeta(service, methodDescriptor);
    method_map_[method_id] = meta;
  }
}

void 
RegisterRpcService(gpb::Service* service) {
	gRpcManager->Register(service);
}

RpcMeta* 
GetRpcService(uint64_t method_id) {
	return gRpcManager->GetService(method_id);
}
};  // namespace serverkit