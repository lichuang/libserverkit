/*
 * Copyright (C) codedump
 */
#ifndef __SERVERKIT_CORE_LOG_H__
#define __SERVERKIT_CORE_LOG_H__

#include <stdarg.h>
#include <stdio.h>

namespace serverkit {

// log level
enum {
  LOG_FATAL = 0,
  LOG_ERR   = 1,
  LOG_WARN  = 2,
  LOG_INFO  = 3,
  LOG_DEBUG = 4
};

extern int gLogLevel;

extern void Log(int level, const char* file, int line, const char *format, ...);

extern void InitLog();
extern void SetLogLevel(int level);

#define Fatalf(args...) if (gLogLevel >= LOG_FATAL)   Log(LOG_ERR, __FILE__, __LINE__, args)
#define Errorf(args...) if (gLogLevel >= LOG_ERR)   Log(LOG_ERR, __FILE__, __LINE__, args)
#define Warnf(args...)  if (gLogLevel >= LOG_WARN)  Log(LOG_WARN, __FILE__, __LINE__, args)
#define Infof(args...)  if (gLogLevel >= LOG_INFO)  Log(LOG_INFO, __FILE__, __LINE__, args)
#define Debug(args...)  if (gLogLevel >= LOG_DEBUG) Log(LOG_DEBUG, __FILE__, __LINE__, args)

};  // namespace serverkit

#endif  // __SERVERKIT_CORE_LOG_H__
