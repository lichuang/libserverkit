/*
 * Copyright (C) codedump
 */

#include "serverkit.h"
#include "util/time.h"

namespace serverkit {

Status 
ServerkitInit(int argc, char *args[], const ServerkitOption& options) {
	gLogThread->Init(argc, args, options);
	gApplication->Init(options);

	TZSet();
	
	return Status::OK();
}

void 
AddService(const Endpoint& endpoint, AcceptorHandler* handler) {
  gApplication->AddService(endpoint, handler);
}

void 
AddRpcService(const Endpoint& endpoint, gpb::Service* service) {
  gApplication->AddRpcService(endpoint, service);
}

void 
RunServer() {
  gApplication->Run();
}

void 
CreateRpcChannel(const Endpoint& endpoint, CreateChannelDone done) {
	gApplication->CreateRpcChannel(endpoint, done);
}

};  // namespace serverkit