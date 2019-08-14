/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_BASE_STRING_H__
#define __SERVERKIT_BASE_STRING_H__

#include <stdarg.h>
#include <string>

using namespace std;

namespace serverkit {

extern void Stringf(string *ret, const char *fmt, ...);

};  // namespace serverkit

#endif  // __SERVERKIT_BASE_STRING_H__
