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

class Logger {
 public:
  // compile time calculation of basename of source file
  class SourceFile
  {
   public:
    template<int N>
    SourceFile(const char (&arr)[N])
      : data_(arr),
        size_(N-1)
    {
      const char* slash = strrchr(data_, '/'); // builtin function
      if (slash)
      {
        data_ = slash + 1;
        size_ -= static_cast<int>(data_ - arr);
      }
    }

    explicit SourceFile(const char* filename)
      : data_(filename)
    {
      const char* slash = strrchr(filename, '/');
      if (slash)
      {
        data_ = slash + 1;
      }
      size_ = static_cast<int>(strlen(data_));
    }

    const char* data_;
    int size_;
  };

  Logger(SourceFile file, int line);
  Logger(SourceFile file, int line, LogLevel level);
  Logger(SourceFile file, int line, LogLevel level, const char* func);
  Logger(SourceFile file, int line, bool toAbort);
  ~Logger();

  LogStream& stream() { return impl_.stream_; }

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

    LogStream stream_;
    LogLevel level_;
    int line_;
    SourceFile basename_;
  };

  Impl impl_;
};

extern LogLevel gLogLevel;

inline LogLevel Logger::logLevel()
{
  return gLogLevel;
}

#define LOG_TRACE if (serverkit::Logger::logLevel() <= serverkit::TRACE) \
  serverkit::Logger(__FILE__, __LINE__, serverkit::TRACE, __func__).stream()
#define LOG_DEBUG if (serverkit::Logger::logLevel() <= serverkit::DEBUG) \
  serverkit::Logger(__FILE__, __LINE__, serverkit::DEBUG, __func__).stream()
#define LOG_INFO if (serverkit::Logger::logLevel() <= serverkit::INFO) \
  serverkit::Logger(__FILE__, __LINE__).stream()
#define LOG_WARN serverkit::Logger(__FILE__, __LINE__, serverkit::WARN).stream()
#define LOG_ERROR serverkit::Logger(__FILE__, __LINE__, serverkit::ERROR).stream()
#define LOG_FATAL serverkit::Logger(__FILE__, __LINE__, serverkit::FATAL).stream()
#define LOG_SYSERR serverkit::Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL serverkit::Logger(__FILE__, __LINE__, true).stream()

const char* strerror_tl(int savedErrno);

// Taken from glog/logging.h
//
// Check that the input is non NULL.  This very useful in constructor
// initializer lists.

#define CHECK_NOTNULL(val) \
  ::serverkit::CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

// A small helper for CHECK_NOTNULL().
template <typename T>
T* CheckNotNull(Logger::SourceFile file, int line, const char *names, T* ptr) {
  if (ptr == NULL) {
   Logger(file, line, FATAL).stream() << names;
  }
  return ptr;
}

}  // namespace serverkit

#endif  // __SERVERKIT_BASE_LOG_H__
