/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_CORE_CHANNEL_H__
#define __SERVERKIT_CORE_CHANNEL_H__

#include "base/endpoint.h"
#include "core/data_handler.h"
#include "poller.h"
#include "socket.h"

namespace serverkit {

class Poller;
class Socket;

// for clien-server connection
class Channel : public DataHandler {
public:
	Channel(const Endpoint& endpoint);
	virtual ~Channel();

	virtual void OnWrite() { 

  }

  virtual void OnRead() { 

  }
  
  virtual void OnConnect(int error) {

  }

  virtual void OnError(int error) {

  }

protected:
	Socket *socket_;
	Poller* poller_;
};

};  // namespace serverkit

#endif  // __SERVERKIT_CORE_CHANNEL_H__