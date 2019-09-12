/*
 * Copyright (C) codedump
 */

#include "serverkit.h"
#include "base/time.h"

namespace serverkit {

Status 
ServerkitInit(int argc, char *args[]) {
	serverkit::Singleton<serverkit::LogThread>::Instance();
	TZSet();
	
	return Status::OK();
}

void 
AddService(const Endpoint& endpoint, AcceptorHandler* handler) {
  gServer->AddService(endpoint, handler);
}

void 
AddRpcService(const Endpoint& endpoint, gpb::Service* service) {
  gServer->AddRpcService(endpoint, service);
}

void 
RunServer(const ServerOption& option) {
  gServer->Run(option);
}

RpcChannel* 
CreateRpcChannel(const Endpoint& endpoint) {
	return gServer->CreateRpcChannel(endpoint);
}

};  // namespace serverkit