/*
 * Copyright (C) codedump
 */

#include <google/protobuf/descriptor.h>
#include "base/log.h"
#include "util/string.h"
#include "rpc/rpc_meta.h"
#include "rpc/rpc_service.h"
#include "rpc/rpc_session.h"

namespace serverkit {

RpcService::RpcService()
  : AcceptorHandler(new RpcSessionFactory()) {
}

RpcService::~RpcService() {
}

void
RpcService::printMethod() {
  Info() << "printMethod in " << this;

  MethodMetaMap::iterator iter = method_map_.begin();
  for (; iter != method_map_.end(); iter++) {
    Info() << "method_id: " << iter->first
      << ", method name: " << iter->second->GetMethodName();
  }
}

RpcMeta* 
RpcService::GetService(uint64_t method_id) {
  MethodMetaMap::iterator iter = method_map_.find(method_id);
  if (iter == method_map_.end()) {
    Error() << "not found method for " << method_id;
    //printMethod();
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
		
    ASSERT(method_map_.find(method_id) == method_map_.end()) 
			<< "register duplicate method " << methodDescriptor->full_name();
    
    Info() << "register method " << methodDescriptor->full_name()
      << " with id " << method_id;

    RpcMeta* meta = new RpcMeta(service, methodDescriptor);
    method_map_[method_id] = meta;
  }
  printMethod();
}

Session* 
RpcService::OnAccept(int fd) {
  Info() << "accept a rpc connection";
  RpcSession *session = static_cast<RpcSession*>(factory_->NewSession(fd));
  session->SetService(this);
  return session;
}

void 
RpcService::OnError(int error) {

}

};  // namespace serverkit