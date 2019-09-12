/*
 * Copyright (C) codedump
 */
#ifndef __SERVERKIT_CORE_IO_THREAD_H__
#define __SERVERKIT_CORE_IO_THREAD_H__

#include <string>
#include "base/macros.h"
#include "base/thread.h"
#include "core/event.h"
#include "core/mailbox.h"
#include "core/message.h"
#include "core/poller.h"

using namespace std;

namespace serverkit {

class IOThread 
  : public Thread,
    public Event,
    public MessageHandler {
public:
  IOThread(const string& name);
  virtual ~IOThread();

  virtual void In();

  virtual void Out();

  virtual void Timeout();

  virtual void Process(Message*);

  Poller* GetPoller() {
    return poller_;
  }

  void Send(Message *msg);
protected:
  virtual void Run();

  void processAcceptMessage(Message*);
  void processRpcChannelMessage(Message*);
  
protected:
  Poller *poller_;
  Mailbox mailbox_;

  DISALLOW_COPY_AND_ASSIGN(IOThread);
};

};  // namespace serverkit

#endif  // __SERVERKIT_CORE_IO_THREAD_H__
