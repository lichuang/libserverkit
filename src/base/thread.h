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

class Thread;
class Runnable;

class Thread {
public:
  Thread(const string& name, void *arg);
  Thread(const string& name, Runnable*);

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

private:
  static void* main(void *arg);

private:
  tid_t tid_;
  string name_;
  ThreadState state_;
  Runnable* runnable_;
};

class Runnable {
public:
  Runnable() : thread_(NULL) {}

  // thread main loop
	virtual void Run() = 0;

	virtual ~Runnable() {}

  tid_t GetTid() const {
    return thread_->GetTid();
  }
  
  Thread* GetThread() {
    return thread_;
  }
  
protected:
  Thread *thread_;  
};

extern const string& CurrentThreadName();

};  // namespace serverkit

#endif  // __SERVERKIT_BASE_THREAD__
