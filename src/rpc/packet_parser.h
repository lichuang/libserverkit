/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_PACKET_PARSER_H__
#define __SERVERKIT_PACKET_PARSER_H__

#include "base/macros.h"
#include "core/socket.h"
#include "rpc/packet.h"

namespace serverkit {

enum PacketParserState {
	RECV_HEADER_STATE,
	RECV_PACKET_STATE,
	RECV_DONE
};

class DataHandler;

class PacketParser {
public:
	PacketParser(Socket *, DataHandler*);
	~PacketParser();

	bool RecvPacket();
	void SendPacket(Packet* packet);

	const Packet& GetPacket() const { 
		return packet_;
	}

private:
	Socket *socket_;
	PacketParserState state_;
	Packet packet_;
	DataHandler *handler_;

	DISALLOW_COPY_AND_ASSIGN(PacketParser);
};

};  // namespace serverkit

#endif  // __SERVERKIT_PACKET_PARSER_H__