/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_PROTOBUF_SERVICE_H__
#define __SERVERKIT_PROTOBUF_SERVICE_H__

#include "rpc/method_meta.h"

namespace serverkit {

class ProtobufChannel;
class Packet;
class PacketParser;

class ProtobufService {
public:
	ProtobufService();
	virtual ~ProtobufService();

	void Register(gpb::Service* service);

private:

};

};  // namespace serverkit

#endif  // __SERVERKIT_PROTOBUF_SERVICE_H__