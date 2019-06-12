/*
 * Copyright (C) codedump
 */
#ifndef __QNODE_CORE_TYPEDEF_H__
#define __QNODE_CORE_TYPEDEF_H__

#include <stdint.h>
#include "base/typedef.h"

namespace serverkit {

typedef uint64_t qid_t;
typedef uint64_t timer_id_t;  

static const timer_id_t kInvalidTimer = 0;

};  // namespace serverkit

#endif // __QNODE_CORE_TYPEDEF_H__
