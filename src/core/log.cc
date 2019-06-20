/*
 * Copyright (C) codedump
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "base/error.h"
#include "base/singleton.h"
#include "base/thread.h"
#include "core/config.h"
#include "core/log.h"

namespace serverkit {

extern thread_local uint64_t gCurrentMs;
extern thread_local char gCurrentTimeString[sizeof("yy-mm-dd hh-mm-ss.000") - 1];

int gLogLevel = LOG_DEBUG;

static const char* kLogLevelString[] = {
  "F",
  "E",
  "W",
  "I",
  "D"
};

// global thread log info
struct globalThreadInfo {
  string path;

  globalThreadInfo() 
    : path("") {
  }
};


void
InitLog(const string& path, int level) {
  Singleton<globalThreadInfo>::Instance()->path = path;
  gLogLevel = level;
}

void
Log(int level, const char* file, int line, const char *fmt, ...) {
  ThreadInfo *info = CurrentThreadInfo();
  if (info->fd == -1) {
    string file = Singleton<globalThreadInfo>::Instance()->path + "/qnode.log";
    info->fd = open(file.c_str(), O_WRONLY | O_APPEND | O_CREAT);
    Assert(info->fd != -1);
  }

  char *p = &(info->buffer[0]);
  char *end = p + kLogBufferSize;
  int n;

  // log header
  n = snprintf(p, end - p, "[%s %s %s %s:%d]", 
    kLogLevelString[level], gCurrentTimeString,
    info->name.c_str(), file, line);
  p += n;

  va_list ap;
  va_start(ap, fmt);
  n = vsnprintf(p, end - p, fmt, ap);
  va_end(ap);

  *(p + n) = '\n';
  *(p + n + 1) = '\0';

  write(info->fd, info->buffer, p - &(info->buffer[0]) + n + 1);
}

};  // namespace serverkit
