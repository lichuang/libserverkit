/*
 * Copyright (C) codedump
 */

#include "serverkit.h"
#include "base/time.h"

namespace serverkit {

Status 
ServerkitInit(int argc, char *args[], const ServerkitOption& options) {
	gLogThread->Init(argc, args, options);
	gServer->Init(options);

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
RunServer() {
  gServer->Run();
}

void 
CreateRpcChannel(const Endpoint& endpoint, CreateChannelDone done) {
	gServer->CreateRpcChannel(endpoint, done);
}

};  // namespace serverkit