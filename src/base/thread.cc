/*
 * Copyright (C) codedump
 */

#include <sys/prctl.h>
#include "base/condition.h"
#include "base/mutex.h"
#include "base/thread.h"
#include "base/time.h"

namespace serverkit {

// per thread info
struct PerThreadInfo {
  Thread *thread;
  string name;

  PerThreadInfo()
    : thread(NULL),
      name("main") {      
  }

  ~PerThreadInfo() {
  }
};

thread_local static PerThreadInfo gPerThreadInfo;

Thread::Thread(const string& name)
  : tid_(0),
    name_(name),
    state_(kThreadNone) {
}

Thread::~Thread() {
}

int
Thread::Start() {
  int ret = pthread_create(&tid_, NULL, Thread::main, this);

  return ret;
}

void
Thread::Join() {
  state_ = kThreadStopped;

  if (tid_ != 0) {
    pthread_join(tid_, NULL);
    tid_ = 0;
  }
}

void*
Thread::main(void* arg) {
  Thread* thread = static_cast<Thread*>(arg);

  ::prctl(PR_SET_NAME, thread->name_.c_str());
  gPerThreadInfo.thread = thread;
  gPerThreadInfo.name = thread->name_;

  thread->state_ = kThreadRunning;
  thread->Run();

  return NULL;
}

const string&
CurrentThreadName() {
  return gPerThreadInfo.name;
}

PerThreadInfo*
CurrentThreadInfo() {
  return &gPerThreadInfo;
}

uint64_t 
CurrentMs() {
  return 0;
}

};  // namespace serverkit
