/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_BASE_OBJECT_POOL_H__
#define __SERVERKIT_BASE_OBJECT_POOL_H__

#include <list>
#include "base/error.h"

using namespace std;

// non-thread safe object pool

namespace serverkit {

// number per ObjectPool alloc
static const int kAllocObjectNumber = 100;

template <typename T>
class ObjectPool {
public:
  ObjectPool() {
    allocate();
  }

  ~ObjectPool() {
    typename list<T*>::iterator iter = free_list_.begin();

    while (iter != free_list_.end()) {
      T *obj = *iter;
      ++iter;
      delete obj;
    }
  }

  T* Get() {
    if (free_list_.empty()) {
      allocate();
    }
    T* obj = free_list_.front();
    free_list_.pop_front();
    Assert(obj != NULL);
    return obj;
  }

  void Free(T *obj) {
    Assert(obj != NULL);
    obj->Reset();
    free_list_.push_back(obj);
  }

private:
  void allocate() {
    int i;

    for (i = 0; i < kAllocObjectNumber; ++i) {
      T* obj = new T();
      if (obj != NULL) {
        free_list_.push_back(obj);
      }
    }
  }

private:
  list<T*> free_list_;

  DISALLOW_COPY_AND_ASSIGN(ObjectPool);
};

};  // namespace serverkit

#endif  // __SERVERKIT_BASE_OBJECT_POOL_H__
