/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_BASE_WAIT_H__
#define __SERVERKIT_BASE_WAIT_H__

#include "base/condition.h"
#include "base/macros.h"

namespace serverkit {

class WaitGroup {
public:
  WaitGroup();
  ~WaitGroup();

  void Add(int);
  void Wait();
  void Done();
  int Count();
  
private:
  Mutex mutex_;
  Condition condition_;
  int counter_;

  DISALLOW_COPY_AND_ASSIGN(WaitGroup);
};

};  // namespace serverkit

#endif  // __SERVERKIT_BASE_WAIT_H__