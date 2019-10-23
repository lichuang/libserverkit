/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_PACKET_PARSER_H__
#define __SERVERKIT_PACKET_PARSER_H__

#include "base/macros.h"
#include "rpc/packet.h"

namespace serverkit {

class Socket;

// PacketParser for RPC packet
class PacketParser {
public:
	PacketParser(Socket *);
	~PacketParser();

	// recv a packet from socket
	// if true, then can get the recvived packet from GetPacket
	bool RecvPacket();

	// send packet to socket
	void SendPacket(Packet* packet);

	const Packet& GetPacket() const { 
		return packet_;
	}

private:
	// rpc Packet recv statemachine states
	enum State {
		// receive packet header
		RECV_HEADER_STATE,
		// receive packet content
		RECV_PACKET_STATE,
		// receive done
		RECV_DONE
	};

private:
	// corresponding socket
	Socket *socket_;
	// parser state
	State state_;
	// recv Packet buffer
	Packet packet_;

	DISALLOW_COPY_AND_ASSIGN(PacketParser);
};

};  // namespace serverkit

#endif  // __SERVERKIT_PACKET_PARSER_H__