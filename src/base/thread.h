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

class Thread {
public:
  Thread(const string& name);

  virtual ~Thread();

  int Start(void *arg);

  void Join();

  tid_t GetTid() const {
    return tid_;
  }

  const string& Name() const {
    return name_;
  }

  const tid_t& Tid() const {
    return tid_;
  }

protected:
  virtual void Run(void* arg) = 0;

private:
  static void* main(void *arg);

protected:
  tid_t tid_;
  string name_;
  void *arg_;
};

// max size per log
static const int kLogBufferSize = 1024 * 1024;

extern void UpdateThreadTime();
extern const string& CurrentThreadName();
extern uint64_t CurrentMs();
extern const char* CurrentMsString();

};  // namespace serverkit

#endif  // __SERVERKIT_BASE_THREAD__
