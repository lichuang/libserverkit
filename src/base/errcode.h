/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_BASE_ERRCODE_H__
#define __SERVERKIT_BASE_ERRCODE_H__

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

};  // namespace serverkit
#endif  // __SERVERKIT_BASE_ERRCODE_H__
