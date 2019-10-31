/*
 * Copyright (C) codedump
 */
#ifndef __SERVERKIT_CORE_WORKER_H__
#define __SERVERKIT_CORE_WORKER_H__

#include <string>
#include "base/macros.h"
#include "base/thread.h"
#include "core/event.h"
#include "core/mailbox.h"
#include "core/message.h"
#include "core/poller.h"

using namespace std;

namespace serverkit {

// worker thread
// inside the worker there is a poller,
// other threads can communicate to the thread using message
class Worker 
  : public Thread,
    public Event,
    public MessageHandler {
public:
  Worker(const string& name);
  virtual ~Worker();

  // Event vitual methods
  virtual void In();

  virtual void Out();

  virtual void Timeout();

  // process message handler
  virtual void Process(Message*);

  Poller* GetPoller() {
    return poller_;
  }

  // send message to the worker
  void Send(Message *msg);
  
protected:

  void processAcceptMessage(Message*);
  void processRpcChannelMessage(Message*);
  
  virtual void Run();
protected:
  Poller *poller_;
  Mailbox mailbox_;

  DISALLOW_COPY_AND_ASSIGN(Worker);
};

};  // namespace serverkit

#endif  // __SERVERKIT_CORE_WORKER_H__
