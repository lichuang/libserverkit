/*
 * Copyright (C) codedump
 */

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include "base/buffer.h"
#include "base/log.h"
#include "base/thread.h"

namespace serverkit {

const char* kLogLevelName[NUM_LOG_LEVELS] = {
  "[T ",
  "[D ",
  "[I ",
  "[W ",
  "[E ",
  "[F ",
};

thread_local LogStream gStream;

static LogLevel initLogLevel() {
  char *val = ::getenv("SERVERKIT_LOG_LEVEL");
  if (val == NULL) {
    return INFO;
  }
  int i;
  for (i = TRACE; i < NUM_LOG_LEVELS; ++i) {
    if (!strcmp(kLogLevelName[i], val)) {
      return static_cast<LogLevel>(i);
    }
  }
  return INFO;
}

LogLevel gLogLevel = initLogLevel();
static string gLogPath;

void defaultOutput(const char* msg, int len) {
  size_t n = fwrite(msg, 1, len, stdout);
  //FIXME check n
  UNUSED(n);
}

void defaultFlush() {
  fflush(stdout);
}

Logger::OutputFunc gOutputput = defaultOutput;
Logger::FlushFunc gFlush = defaultFlush;

Logger::Impl::Impl(LogLevel level, const char* file, int line)
   :level_(level),
    line_(line) {
  gStream << kLogLevelName[level];
  gStream << CurrentThreadName() << " " << file <<
    ':' << line_ << " " << CurrentMsString() << ']';
}

void Logger::Impl::Finish() {
  gStream << "\n";
}

Logger::Logger(const char* file, int line, LogLevel level, const char* func)
  : impl_(level, file, line) {
}

Logger::~Logger() {
  impl_.Finish();
  Buffer* buf = Stream().buffer();
  gOutputput(buf->Data(), buf->Length());
  if (impl_.level_ == FATAL) {
    gFlush();
    abort();
  }
}

void Logger::setLogLevel(LogLevel level) {
  gLogLevel = level;
}

void Logger::setOutput(OutputFunc out) {
  gOutputput = out;
}

void Logger::setFlush(FlushFunc flush) {
  gFlush = flush;
}

}  // namespace serverkit
