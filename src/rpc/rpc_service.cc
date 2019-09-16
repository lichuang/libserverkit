/*
 * Copyright (C) codedump
 */

#include <google/protobuf/descriptor.h>
#include "base/log.h"
#include "base/string.h"
#include "rpc/rpc_meta.h"
#include "rpc/rpc_service.h"
#include "rpc/rpc_session.h"

namespace serverkit {

RpcService::RpcService()
  : AcceptorHandler(new RpcSessionFactory()) {
}

RpcService::~RpcService() {

}

RpcMeta* 
RpcService::GetService(uint64_t method_id) {
  MethodMetaMap::iterator iter = method_map_.find(method_id);
  if (iter == method_map_.end()) {
    Error() << "not found method for " << method_id;
    return NULL;
  }

	return iter->second;
}

void 
RpcService::Register(gpb::Service* service) {
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

Session* 
RpcService::OnAccept(int fd) {
  Info() << "accept a rpc connection";
  return factory_->NewSession(fd);
}

void 
RpcService::OnError(int error) {

}

};  // namespace serverkit