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
  // compile time calculation of basename of source file
  class SourceFile {
   public:
    template<int N>
    SourceFile(const char (&arr)[N])
      : data_(arr),
        size_(N-1) {
      const char* slash = strrchr(data_, '/'); // builtin function
      if (slash) {
        data_ = slash + 1;
        size_ -= static_cast<int>(data_ - arr);
      }
    }

    explicit SourceFile(const char* filename)
      : data_(filename) {
      const char* slash = strrchr(filename, '/');
      if (slash) {
        data_ = slash + 1;
      }
      size_ = static_cast<int>(strlen(data_));
    }

    const char* data_;
    int size_;
  };

  Logger(SourceFile file, int line, LogLevel level, const char* func);
  ~Logger();

  //LogStream& stream() { return impl_.stream_; }
  LogStream& stream() { return gStream; }

  static LogLevel logLevel();
  static void setLogLevel(LogLevel level);

  typedef void (*OutputFunc)(const char* msg, int len);
  typedef void (*FlushFunc)();
  static void setOutput(OutputFunc);
  static void setFlush(FlushFunc);

 private:
  class Impl {
   public:
    Impl(LogLevel level, int old_errno, const SourceFile& file, int line);
    void finish();

    LogLevel level_;
    int line_;
    SourceFile basename_;
  };

  Impl impl_;
};


extern LogLevel gLogLevel;

inline LogLevel Logger::logLevel() {
  return gLogLevel;
}

#define Trace if (serverkit::Logger::logLevel() <= serverkit::TRACE) \
  serverkit::Logger(__FILE__, __LINE__, serverkit::TRACE, __func__).stream()

#define Debug if (serverkit::Logger::logLevel() <= serverkit::DEBUG) \
  serverkit::Logger(__FILE__, __LINE__, serverkit::DEBUG, __func__).stream()

#define Info if (serverkit::Logger::logLevel() <= serverkit::INFO) \
  serverkit::Logger(__FILE__, __LINE__, serverkit::INFO, __func__).stream()

const char* strerror_tl(int savedErrno);

}  // namespace serverkit

#endif  // __SERVERKIT_BASE_LOG_H__
