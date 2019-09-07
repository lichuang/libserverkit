/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_PROTOBUF_SERVICE_H__
#define __SERVERKIT_PROTOBUF_SERVICE_H__

#include <map>
#include "base/endpoint.h"
#include "core/acceptor_handler.h"
#include "rpc/method_meta.h"
#include "rpc/packet.h"

using namespace std;

namespace serverkit {

class ProtobufChannel;
class Packet;
class PacketParser;

class ProtobufService : public AcceptorHandler {
public:
	ProtobufService(const Endpoint&);
	virtual ~ProtobufService();

	ProtobufChannel* GetChannelByEndpoint(const Endpoint&);
	ProtobufChannel* GetChannelByGuid(uint64_t);

	void Register(gpb::Service* service);
	void RunService(const Packet& packet, uint64_t guid);

	// AcceptorHandler virtual method
  virtual void OnAccept(Session*);
  virtual void OnError(int error);

private:
	typedef map<Endpoint, ProtobufChannel*> EpChannelMap;
	typedef map<uint64_t, ProtobufChannel*> GUIDChannelMap;

	EpChannelMap channel_map_;
	UUIDChannelMap guid_channel_map_;
};

};  // namespace serverkit

#endif  // __SERVERKIT_PROTOBUF_SERVICE_H__