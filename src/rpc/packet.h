/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_RPC_PACKET_H__
#define __SERVERKIT_RPC_PACKET_H__

#include <string>
#include "base/string.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/message.h"

using namespace std;
namespace gpb = ::google::protobuf;

namespace serverkit {

// sizeof (magic_num) + sizeof (call_guid) + sizeof (method_id) + sizeof (size) = 21
static const uint32_t kMetaSize = 21;
static const uint8_t kPacketMagicNum = 0x2B;

struct Packet {
public:	
	uint8_t magic;
	uint64_t guid;
	uint64_t method_id;
	uint32_t size;
	string content;

	Packet() 
		: magic(0), guid(0), method_id(0), size(0), content(""){
	}
	
	Packet(uint64_t id, const gpb::MethodDescriptor *method, const gpb::Message *req) 
		: magic(kPacketMagicNum), guid(id), method_id(0) {
			content = req->SerializeAsString();
			if (method) {
				method_id = HashString(method->full_name());
			} 
	}
};
};  // namespace serverkit

#endif  // __SERVERKIT_RPC_PACKET_H__