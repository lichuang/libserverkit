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

#define UNUSED(object) (void) object

#endif  // __SERVERKIT_BASE_MACROS_H__
