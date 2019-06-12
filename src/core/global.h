/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_CORE_GLOBAL_H__
#define __SERVERKIT_CORE_GLOBAL_H__

#include <stdint.h>
#include <string>

using namespace std;

namespace serverkit {

extern volatile uint64_t gCurrentMs;
extern string   gCurrentMsString;

};  // namespace serverkit

#endif  // __SERVERKIT_CORE_GLOBAL_H__
