/*
 * Copyright (C) codedump
 */

// this header include all needed serverkit header file

#ifndef __SERVERKIT_H__
#define __SERVERKIT_H__

#include "base/log.h"
#include "base/log_thread.h"
#include "base/status.h"
#include "core/server.h"

namespace serverkit {

extern Status ServerkitInit(int argc, char *args[]);

};  // namespace serverkit

#endif	// __SERVERKIT_H__