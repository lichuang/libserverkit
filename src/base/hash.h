/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_BASE_HASH_H__
#define __SERVERKIT_BASE_HASH_H__

#include <stdint.h>

namespace serverkit {

extern uint64_t HashBytes(const char*, int);

};  // namespace serverkit

#endif  // __SERVERKIT_BASE_HASH_H__