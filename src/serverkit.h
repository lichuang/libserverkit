/*
 * Copyright (C) codedump
 */

// this header include all needed serverkit header file

#ifndef __SERVERKIT_H__
#define __SERVERKIT_H__

#include "serverkit_types.h"
#include "base/endpoint.h"
#include "base/log.h"
#include "base/log_thread.h"
#include "base/status.h"
#include "core/server.h"
#include "rpc/rpc_channel.h"
#include "rpc/rpc_controller.h"

namespace serverkit {

class Poller;

extern Status ServerkitInit(int argc, char *args[], const ServerkitOption&);

extern void AddService(const Endpoint&, AcceptorHandler*);

// rpc methods
extern void AddRpcService(const Endpoint&, gpb::Service* service);

extern void CreateRpcChannel(const Endpoint& endpoint, CreateChannelDone done);

extern void RunServer();

};  // namespace serverkit

#endif	// __SERVERKIT_H__