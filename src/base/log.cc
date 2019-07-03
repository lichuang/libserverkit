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
thread_local char t_errnobuf[512];

const char* strerror_tl(int savedErrno) {
  return strerror_r(savedErrno, t_errnobuf, sizeof t_errnobuf);
}

static LogLevel initLogLevel() {
  char *val = ::getenv("SERVERKIT_LOG_LEVEL");
  if (val == NULL) {
    return INFO;
  }
  int i;
  for (i = TRACE; i < NUM_LOG_LEVELS; ++i) {
    if (!strcmp(kLogLevelName[i], val)) {
      return (LogLevel)i;
    }
  }
  return INFO;
}

LogLevel gLogLevel = initLogLevel();
static string gLogPath;

// helper class for known string length at compile time
class T {
 public:
  T(const char* str, unsigned len)
    :str_(str),
     len_(len) {
    assert(strlen(str) == len_);
  }

  const char* str_;
  const unsigned len_;
};

inline LogStream& operator<<(LogStream& s, T v) {
  s.Append(v.str_, v.len_);
  return s;
}

inline LogStream& operator<<(LogStream& s, const Logger::SourceFile& v) {
  s.Append(v.data_, v.size_);
  return s;
}

void defaultOutput(const char* msg, int len) {
  size_t n = fwrite(msg, 1, len, stdout);
  //FIXME check n
  (void)n;
}

void defaultFlush() {
  fflush(stdout);
}

Logger::OutputFunc gOutputput = defaultOutput;
Logger::FlushFunc gFlush = defaultFlush;

Logger::Impl::Impl(LogLevel level, int savedErrno, const SourceFile& file, int line)
   :level_(level),
    line_(line),
    basename_(file) {
  gStream << kLogLevelName[level];
  gStream << CurrentThreadName() << " " << basename_ <<
    ':' << line_ << " " << CurrentMsString() << ']';

  if (savedErrno != 0) {
    gStream << strerror_tl(savedErrno) << " (errno=" << savedErrno << ") ";
  }
}

void Logger::Impl::finish() {
  gStream << "\n";
}

Logger::Logger(SourceFile file, int line, LogLevel level, const char* func)
  : impl_(level, 0, file, line) {
}

Logger::~Logger() {
  impl_.finish();
  Buffer* buf = stream().buffer();
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
