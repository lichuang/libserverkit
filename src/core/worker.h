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

class Worker 
  : public Runnable,
    public Event,
    public MessageHandler {
public:
  Worker(const string& name);
  virtual ~Worker();

  virtual void In();

  virtual void Out();

  virtual void Timeout();

  virtual void Process(Message*);

  virtual void Run();

  Poller* GetPoller() {
    return poller_;
  }

  void Send(Message *msg);

  void Start();
  
protected:

  void processAcceptMessage(Message*);
  void processRpcChannelMessage(Message*);
  
protected:
  Poller *poller_;
  Mailbox mailbox_;
  Thread *thread_;

  DISALLOW_COPY_AND_ASSIGN(Worker);
};

};  // namespace serverkit

#endif  // __SERVERKIT_CORE_WORKER_H__
