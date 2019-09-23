/*
 * Copyright (C) codedump
 */

#include "base/log.h"
#include "rpc/packet_parser.h"

namespace serverkit {

PacketParser::PacketParser(Socket *socket, DataHandler* handler)
	: socket_(socket), 
		state_(RECV_HEADER_STATE),
		handler_(handler) {
}

PacketParser::~PacketParser() {

}

bool 
PacketParser::RecvPacket() {
	size_t read_size = 0;
	bool done = false;

	while (!done) {
		switch (state_) {
			case RECV_HEADER_STATE:
				read_size = socket_->ReadBufferSize();
				if (read_size < kMetaSize) {
					return false;
				}

				Info() << "socket read buffer size: " << socket_->ReadBufferSize()
					<< ", kMetaSize: " << kMetaSize;

				socket_->Read(reinterpret_cast<char*>(&packet_.magic), sizeof(packet_.magic));
				socket_->Read(reinterpret_cast<char*>(&packet_.guid), sizeof(packet_.guid));
				socket_->Read(reinterpret_cast<char*>(&packet_.method_id), sizeof(packet_.method_id));
				socket_->Read(reinterpret_cast<char*>(&packet_.size), sizeof(packet_.size));

				if (packet_.magic != kPacketMagicNum) {
          Error() << "client package error, magic_num: "
              << packet_.magic
              << " call_guid: " << packet_.guid
              << " method_id: " << packet_.method_id
              << " size: " << packet_.size
							<< " address: " << socket_->String();			
					return false;
				}

				state_ = RECV_PACKET_STATE;
				break;
			case RECV_PACKET_STATE:
				read_size = packet_.size;
				Debug() << "packet content size: " << read_size
					<< "current socket read buffer size: " << socket_->ReadBufferSize();
				if (read_size > socket_->ReadBufferSize()) {
					return false;
				}

				packet_.content.resize(packet_.size);
				socket_->Read(reinterpret_cast<char*>(&packet_.content[0]), read_size);
				state_ = RECV_DONE;
				break;

			case RECV_DONE:
				state_ = RECV_HEADER_STATE;
				done = true;
				break;						
		}
	}

	Info() << "out socket read buffer size: " << socket_->ReadBufferSize();
	return true;
}

void 
PacketParser::SendPacket(Packet* packet) {
  socket_->Write(reinterpret_cast<const char*>(&packet->magic), sizeof(packet->magic));
  socket_->Write(reinterpret_cast<const char*>(&packet->guid), sizeof(packet->guid));
  socket_->Write(reinterpret_cast<const char*>(&packet->method_id),sizeof(packet->method_id));
  socket_->Write(reinterpret_cast<const char*>(&packet->size),sizeof(packet->size));
  socket_->Write(reinterpret_cast<const char*>(&packet->content[0]),packet->content.size());

  Debug() << "SendPacket, guid: " << packet->guid
      << ", method id: " << packet->method_id
			<< ", size: " << packet->size
      << ", content: " << packet->content
			<< ", write buffer size: " << socket_->WriteBufferSize();	
}
};  // namespace serverkit