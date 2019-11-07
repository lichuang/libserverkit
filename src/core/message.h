/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_CORE_MESSAGE_H__
#define __SERVERKIT_CORE_MESSAGE_H__

#include "base/typedef.h"

namespace serverkit {
class Message;

// message types
enum {
  // accepted session from main thread
  kAcceptMessage = 1,
  // bound a rpc channel to a worker
  kRpcChannelMessage = 2,
};

// virtual interface for Mailbox message handler
class MessageHandler {
public:
  virtual ~MessageHandler() {}

  virtual void Process(Message *) = 0;
};

// virtual interface for Mailbox message
class Message {
public:
  Message(int type, tid_t tid, MessageHandler *handler)
    : type_(type), tid_(tid), handler_(handler) {
  }

  virtual ~Message() {}

  void Process() {
    handler_->Process(this);
  }

  int Type() const {
    return type_;
  }

protected:
  int type_;
  tid_t tid_;
  MessageHandler *handler_;
};

};  // namespace serverkit

#endif // __SERVERKIT_CORE_MESSAGE_H__
