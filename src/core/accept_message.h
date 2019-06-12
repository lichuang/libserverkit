/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_ACCEPT_MESSAGE_H__
#define __SERVERKIT_ACCEPT_MESSAGE_H__

#include "core/message.h"

namespace serverkit {

class Session;

class AcceptMessage : public Message {
public:
  AcceptMessage(Session *s, tid_t tid, MessageHandler *h)
    : Message(kAcceptMessage, tid, h),
      session_(s) {
  }

  virtual ~AcceptMessage() {
  }
  Session* GetSession() {
    return session_;
  }
private:
  Session *session_;
};

};  // namespace serverkit

#endif  // __SERVERKIT_ACCEPT_MESSAGE_H__
