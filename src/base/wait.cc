/*
 * Copyright (C) codedump
 */

 #include "base/wait.h"

namespace serverkit {

WaitGroup::WaitGroup()
  : counter_(0) {
}

WaitGroup::~WaitGroup() {

}

void 
WaitGroup::Add(int c) {
  MutexGuard guard(&mutex_);
  counter_ += c;
}

void 
WaitGroup::Wait() {
  MutexGuard guard(&mutex_);
  condition_.Wait(&mutex_);
}
  
void 
WaitGroup::Done() {
  MutexGuard guard(&mutex_);
  if (counter_ > 0) {
    counter_ -= 1;
    if (counter_ <= 0) {
      condition_.Notify();
    }
  }
}

int 
WaitGroup::Count() { 
  MutexGuard guard(&mutex_);
  return counter_;
}

};  // namespace serverkit