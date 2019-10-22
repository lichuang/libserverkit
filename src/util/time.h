/*
 * Copyright (C) codedump
 */
#ifndef __SERVERKIT_UTIL_TIME_H__
#define __SERVERKIT_UTIL_TIME_H__

#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <string>

using namespace std;

namespace serverkit {

struct timespec *MakeTimespec(int64_t ms, struct timespec *ts);

void TZSet();
void Localtime(const time_t& unix_sec, struct tm* result);
void SleepMillisecs(int msec);

extern uint64_t CurrentMs();
};  // namespace serverkit

#endif  // __SERVERKIT_UTIL_TIME_H__
