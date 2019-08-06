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

LogMessage::OutputFunc gOutputput = defaultOutput;
LogMessage::FlushFunc gFlush = defaultFlush;

LogMessage::LogMessage(const char* file, int line, LogLevel level, const char* func)
  : data_(new LogMessageData()) {
  Stream() << kLogLevelName[level];
  Stream() << CurrentThreadName() << " " << file <<
    ':' << line << " " << CurrentMsString() << ']';
}

LogMessage::~LogMessage() {
  finish();

  /* 
  Buffer* buf = Stream().buffer();
  gOutputput(buf->Data(), buf->Length());
  if (impl_.level_ == FATAL) {
    gFlush();
    abort();
  }
  */
}

void
LogMessage::finish() {
  Stream() << "\n";
}

void LogMessage::setLogLevel(LogLevel level) {
  gLogLevel = level;
}

void LogMessage::setOutput(OutputFunc out) {
  gOutputput = out;
}

void LogMessage::setFlush(FlushFunc flush) {
  gFlush = flush;
}

}  // namespace serverkit
