/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_BASE_ATOMIC_H__
#define __SERVERKIT_BASE_ATOMIC_H__

#include <atomic>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include "base/macros.h"

namespace serverkit {

//  This class encapsulates several atomic operations on pointers.
template <typename T>
class atomic_ptr_t {
public:
  //  Initialise atomic pointer
  inline atomic_ptr_t() : ptr_(NULL) { }

  //  Set value of atomic pointer
  //  Use this function only when you are sure that at most one
  //  thread is accessing the pointer at the moment.
  inline void set(T *ptr) { ptr_.store(ptr); }

  //  Perform atomic 'exchange pointers' operation. Pointer is set
  //  to the 'val_' value. Old value is returned.
  inline T *xchg (T *val) {
    //return static_cast<T *>(atomic_xchg_ptr (&_ptr, val_));
    return ptr_.exchange(val, std::memory_order_acq_rel);
  }

  //  Perform atomic 'compare and swap' operation on the pointer.
  //  The pointer is compared to 'cmp' argument and if they are
  //  equal, its value is set to 'val_'. Old value of the pointer
  //  is returned.
  inline T *cas(T *cmp, T *val) {
    ptr_.compare_exchange_strong(cmp, val, std::memory_order_acq_rel);
    return cmp;
  }

private:
  std::atomic<T *> ptr_;

  DISALLOW_COPY_AND_ASSIGN(atomic_ptr_t);
};

struct atomic_value_t {
  atomic_value_t(const int value) : value_(value) {}

  atomic_value_t(const atomic_value_t &src)
    : value_(src.load()) {
  }

  void store(const int value) {
    value_.store(value, std::memory_order_release);
  }

  int load() const {
    return value_.load(std::memory_order_acquire);
  }

private:
  std::atomic<int> value_;

  atomic_value_t &operator= (const atomic_value_t &src);
};

class atomic_counter_t {
public:
  typedef uint32_t integer_t;

  inline atomic_counter_t(integer_t value = 0)
    : value_(value) {
  }

  //  Set counter _value
  inline void set(integer_t value) { value_.store(value); }

  //  Atomic addition. Returns the old _value.
  inline integer_t add(integer_t increment) {
    integer_t old_value;
    old_value = value_.fetch_add(increment, std::memory_order_acq_rel);

    return old_value;
  }

  //  Atomic subtraction. Returns false if the counter drops to zero.
  inline bool sub(integer_t decrement) {
    integer_t old = value_.fetch_sub(decrement, std::memory_order_acq_rel);
    return old - decrement != 0;
  }

  inline integer_t get() const { return value_; }

private:
  std::atomic<integer_t> value_;

  DISALLOW_COPY_AND_ASSIGN(atomic_counter_t);
} __attribute__ ((aligned (sizeof (void *))));

};  // namespace serverkit

#endif // __SERVERKIT_BASE_ATOMIC_H__
