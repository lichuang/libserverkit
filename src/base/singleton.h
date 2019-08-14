/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_BASE_SINGLETON__
#define __SERVERKIT_BASE_SINGLETON__

#include <pthread.h>
#include "base/macros.h"

namespace serverkit {

template <typename T>
class Singleton {
public:
  static T* Instance() {
    pthread_once(&ponce_, &Singleton::init);
    return value_;
  }

private:  
  static void init() {
    value_ = new T();
    value_->doInit();
  }

private:
  DISALLOW_COPY_AND_ASSIGN(Singleton<T>);
  static pthread_once_t ponce_;
  static T* value_;
};

template<typename T>
pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;

template<typename T>
T* Singleton<T>::value_ = NULL;

};  // namespace serverkit

#endif  // __SERVERKIT_BASE_SINGLETON__
