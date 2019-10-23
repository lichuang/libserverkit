/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_BASE_MACROS_H__
#define __SERVERKIT_BASE_MACROS_H__

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

// shutdown compiler complain
#define UNUSED(object) (void) object

#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

#endif  // __SERVERKIT_BASE_MACROS_H__
