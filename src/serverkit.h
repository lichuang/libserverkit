/*
 * Copyright (C) codedump
 */

// this header include all needed serverkit header file

#ifndef __SERVERKIT_H__
#define __SERVERKIT_H__

#include "base/endpoint.h"
#include "base/log.h"
#include "base/log_thread.h"
#include "base/status.h"
#include "core/server.h"

namespace serverkit {

extern Status ServerkitInit(int argc, char *args[]);

extern void AddService(const Endpoint&, AcceptorHandler*);
extern void AddRpcService(const Endpoint&, gpb::Service* service);
extern void RunServer(const ServerOption&);

};  // namespace serverkit

#endif	// __SERVERKIT_H__