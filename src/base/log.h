/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_BASE_LOG_H__
#define __SERVERKIT_BASE_LOG_H__

#include "base/log_stream.h"

namespace serverkit {

enum LogLevel {
  TRACE,
  DEBUG,
  INFO,
  WARN,
  ERROR,
  FATAL,
  NUM_LOG_LEVELS,
};

extern thread_local LogStream gStream;

class Logger {
 public:
  Logger(const char* file, int line, LogLevel level, const char* func);
  ~Logger();

  LogStream& Stream() { return gStream; }

  static LogLevel logLevel();
  static void setLogLevel(LogLevel level);

  typedef void (*OutputFunc)(const char* msg, int len);
  typedef void (*FlushFunc)();
  static void setOutput(OutputFunc);
  static void setFlush(FlushFunc);

 private:
  class Impl {
   public:
    Impl(LogLevel level, const char* file, int line);
    void Finish();

    LogLevel level_;
    int line_;
  };

  Impl impl_;
};

extern LogLevel gLogLevel;

inline LogLevel Logger::logLevel() {
  return gLogLevel;
}

#define Trace if (serverkit::Logger::logLevel() <= serverkit::TRACE) \
  serverkit::Logger(__FILE__, __LINE__, serverkit::TRACE, __func__).Stream()

#define Debug if (serverkit::Logger::logLevel() <= serverkit::DEBUG) \
  serverkit::Logger(__FILE__, __LINE__, serverkit::DEBUG, __func__).Stream()

#define Info if (serverkit::Logger::logLevel() <= serverkit::INFO) \
  serverkit::Logger(__FILE__, __LINE__, serverkit::INFO, __func__).Stream()

#define Warn if (serverkit::Logger::logLevel() <= serverkit::INFO) \
  serverkit::Logger(__FILE__, __LINE__, serverkit::WARN, __func__).Stream()

#define Error if (serverkit::Logger::logLevel() <= serverkit::INFO) \
  serverkit::Logger(__FILE__, __LINE__, serverkit::ERROR, __func__).Stream()

#define Fatal if (serverkit::Logger::logLevel() <= serverkit::INFO) \
  serverkit::Logger(__FILE__, __LINE__, serverkit::FATAL, __func__).Stream()
}  // namespace serverkit

#endif  // __SERVERKIT_BASE_LOG_H__
