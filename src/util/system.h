/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_UTIL_SYSTEM_H__
#define __SERVERKIT_UTIL_SYSTEM_H__

#include <string>

using namespace std;

namespace serverkit {

// return callstack in result
void GetCallStack(string *result);

};  // namespace serverkit

#endif  // __SERVERKIT_UTIL_SYSTEM_H__