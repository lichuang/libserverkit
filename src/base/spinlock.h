/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_BASE_SPINLOCK_H__
#define __SERVERKIT_BASE_SPINLOCK_H__

#include <atomic>

namespace serverkit {

class SpinLock {
public:
  void Lock() {
    while (flag_.test_and_set()) {
#if defined(__i386__) || defined(__x86_64__)
      __asm__ __volatile__("pause");
#else
      /* nothing */
#endif
    }
  }

  void UnLock() {
    flag_.clear();
  }

private:
  std::atomic_flag flag_ = ATOMIC_FLAG_INIT;
};
};  // namespace serverkit

#endif  // __SERVERKIT_BASE_SPINLOCK_H__
