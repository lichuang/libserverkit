/*
 * Copyright (C) codedump
 */

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <sstream>
#include "base/buffer.h"
#include "base/log.h"
#include "base/log_thread.h"
#include "base/thread.h"

namespace serverkit {

const char* kLogLevelName[NUM_LOG_LEVELS] = {
  "[D ",
  "[I ",
  "[W ",
  "[E ",
  "[F ",
};

static LogLevel 
initLogLevel() {
  char *val = ::getenv("SERVERKIT_LOG_LEVEL");
  if (val == NULL) {
    return DEBUG;
  }
  int i;
  for (i = DEBUG; i < NUM_LOG_LEVELS; ++i) {
    if (!strcmp(kLogLevelName[i], val)) {
      return static_cast<LogLevel>(i);
    }
  }
  return DEBUG;
}

LogLevel gLogLevel = initLogLevel();
static string gLogPath;

void 
defaultOutput(const char* msg, int len) {
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
  : data_(new LogMessageData()),
    level_(level) {
  Stream() << kLogLevelName[level] << CurrentThreadName() 
    << " " << CurrentLogTimeString() 
    << " " << basename(const_cast<char*>(file)) 
    << ':' << line << ']';
}

LogMessage::~LogMessage() {
  finish();
  
  SendLog(data_);
  if (level_ == FATAL) {
    Flush(true);
    abort();
  }
}

void
LogMessage::finish() {
  Stream() << "\n";
  data_->text_[data_->stream_.pcount()] = '\0';
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
