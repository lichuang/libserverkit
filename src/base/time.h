/*
 * Copyright (C) codedump
 */
#ifndef __SERVERKIT_BASE_TIME_H__
#define __SERVERKIT_BASE_TIME_H__

#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <string>

using namespace std;

namespace serverkit {

struct timespec *MakeTimespec(int64_t ms, struct timespec *ts);
uint64_t NowMs();
void NowMsString(string *ret);
void UpdateTime();

};  // namespace serverkit

#endif  // __SERVERKIT_BASE_TIME_H__
