/*
 * Copyright (C) codedump
 */

#ifndef __kBASE_ERRCODE_H__
#define __kBASE_ERRCODE_H__

#include <errno.h>

namespace serverkit {

// error code < 0
static const int kError   = -1;
static const int kClosed  = -2;
static const int kConnectionReset = -3;
// below is rpc error code
static const int kRpcChannelTimeout = -4;
static const int kRpcExistedSameChannel = -5;

// success code >= 0
static const int kOK    = 0;
static const int kAgain = 1;

#define IsIOTryAgain(err) (((err) == EAGAIN) || ((err) == EWOULDBLOCK))

#define gErrno         errno
#define StrError       strerror

};  // namespace serverkit
#endif  // __kBASE_ERRCODE_H__
