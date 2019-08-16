/*
 * Copyright (C) codedump
 */
#ifndef __SERVERKIT_BASE_THREAD__
#define __SERVERKIT_BASE_THREAD__

#include <string>
#include <pthread.h>

using namespace std;

namespace serverkit {

typedef pthread_t tid_t;

enum ThreadState {
  kThreadNone,
  kThreadRunning,
  kThreadStopped,
};

class Thread {
public:
  Thread(const string& name, void *arg);

  virtual ~Thread();

  void Stop();

  tid_t GetTid() const {
    return tid_;
  }

  const string& Name() const {
    return name_;
  }

  const tid_t& Tid() const {
    return tid_;
  }

  ThreadState State() const { 
    return state_; 
  }

  bool Running() const { 
    return state_ == kThreadRunning;
  }

  int Start();
protected:
  virtual void Run() = 0;

private:
  static void* main(void *arg);

protected:
  tid_t tid_;
  string name_;
  void *arg_;
  ThreadState state_;
};

extern const string& CurrentThreadName();

};  // namespace serverkit

#endif  // __SERVERKIT_BASE_THREAD__
