/*
 * Copyright (C) codedump
 */

#include <sys/prctl.h>
#include "base/error.h"
#include "base/condition.h"
#include "base/mutex.h"
#include "base/thread.h"
#include "base/time.h"

namespace serverkit {

//#define TIME_FORMAT_LENGTH sizeof("2018/01/01 00:00:00.123") - 1
#define TIME_FORMAT_LENGTH 84

// per thread info
struct ThreadInfo {
  // log file fd
  int fd;
  // log buffer
  char* buffer;
  // thread name
  string name;
  Thread *thread;

  ThreadInfo()
    : fd(-1),
      buffer(new char[kLogBufferSize]),
      name("main"),  // default is main thread
      thread(NULL) {      
  }

  ~ThreadInfo() {
    delete [] buffer;
  }
};
thread_local static ThreadInfo gPerThreadInfo;

struct threadStartEntry {
  Condition *cond;
  Thread *thread;
};

Thread::Thread(const string& name)
  : tid_(0),
    name_(name) {
}

Thread::~Thread() {
}

int
Thread::Start(void* arg) {
  arg_ = arg;
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
Thread::Join() {
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
  gPerThreadInfo.name = thread->name_;
  gPerThreadInfo.thread = thread;

  cond->Notify();

  thread->Run(thread->arg_);

  return NULL;
}

const string&
CurrentThreadName() {
  return gPerThreadInfo.name;
}

ThreadInfo*
CurrentThreadInfo() {
  return &gPerThreadInfo;
}

};  // namespace serverkit
