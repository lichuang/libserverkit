/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_BASE_OBJECT_POOL_H__
#define __SERVERKIT_BASE_OBJECT_POOL_H__

#include <list>
#include <pthread.h>
#include "base/error.h"
#include "base/macros.h"
#include "base/object_pool.h"

using namespace std;

namespace serverkit {

// number per ObjectList alloc
static const int kAllocObjectNumber = 100;

template <typename T>
class ObjectList {
public:
  ObjectList() {
    allocate();
  }

  ~ObjectList() {
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
  DISALLOW_COPY_AND_ASSIGN(ObjectList);
  list<T*> free_list_;
};

template <typename T>
class ObjectPool {
public:
  ObjectPool() {
  }

  static ObjectPool<T>* Instance() {
    pthread_once(&ponce_, &ObjectPool<T>::init);
    return instance_;
  }

  T* Get() {
    return obj_list_.Get();
  }
  void Free(T *obj) {
    obj_list_.Free(obj);
  }

private:  
  static void init() {
    instance_ = new ObjectPool<T>();
  }

private:
  thread_local static ObjectList<T> obj_list_;
  static ObjectPool<T> *instance_;
  static pthread_once_t ponce_;
};

template <typename T>
thread_local ObjectList<T> ObjectPool<T>::obj_list_;

template <typename T>
ObjectPool<T>* ObjectPool<T>::instance_ = NULL;

template<typename T>
pthread_once_t ObjectPool<T>::ponce_ = PTHREAD_ONCE_INIT;

template <typename T>
T* GetObject() {
  return ObjectPool<T>::Instance()->Get();
}

template <typename T>
void FreeObject(T* obj) {
  ObjectPool<T>::Instance()->Free(obj);
}

};  // namespace serverkit

#endif  // __SERVERKIT_BASE_OBJECT_POOL_H__
