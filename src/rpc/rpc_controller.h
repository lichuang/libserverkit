/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_RPC_CONTROLLER_H__
#define __SERVERKIT_RPC_CONTROLLER_H__

#include "google/protobuf/service.h"

namespace serverkit {

namespace gpb = ::google::protobuf;

class RpcController : public gpb::RpcController {
public:

};

};  // namespace serverkit

#endif  // __SERVERKIT_RPC_CONTROLLER_H__