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

struct threadStartEntry {
  Condition *cond;
  Thread *thread;
};

Thread::Thread(const string& name, Runnable* runnable)
  : tid_(0),
    name_(name),
    state_(kThreadNone),
    runnable_(runnable) {
}

Thread::~Thread() {
}

int
Thread::Start() {
  Mutex mutex;
  Condition cond;
  threadStartEntry entry = {
    .cond = &cond,
    .thread = this
  };
  int ret = pthread_create(&tid_, NULL, Thread::main, &entry);

  // wait until thread start
  MutexGuard guard(&mutex);
  cond.Wait(&mutex);

  return ret;
}

void
Thread::Stop() {
  state_ = kThreadStopped;

  if (tid_ != 0) {
    pthread_join(tid_, NULL);
    tid_ = 0;
  }
}

void*
Thread::main(void* arg) {
  threadStartEntry* entry = static_cast<threadStartEntry*>(arg);
  Condition *cond = entry->cond;
  Thread *thread = entry->thread;

  ::prctl(PR_SET_NAME, thread->name_.c_str());
  gPerThreadInfo.thread = thread;
  gPerThreadInfo.name = thread->name_;

  cond->Notify();

  thread->state_ = kThreadRunning;
  thread->runnable_->Run();

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
