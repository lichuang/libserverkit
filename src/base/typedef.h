/*
 * Copyright (C) codedump
 */
#ifndef __SERVERKIT_BASE_TYPEDEF_H__
#define __SERVERKIT_BASE_TYPEDEF_H__

#include <pthread.h>

namespace serverkit {

typedef pthread_t tid_t;

typedef int fd_t;

// invalid fd const
static const fd_t kInvalidFd     = -1;

};  // namespace serverkit

#endif  // __SERVERKIT_BASE_TYPEDEF_H__
