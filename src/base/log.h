/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_BASE_LOG_H__
#define __SERVERKIT_BASE_LOG_H__

#include <iostream>
#include "base/likely.h"
#include "base/macros.h"

namespace serverkit {

enum LogLevel {
  DEBUG,
  INFO,
  WARN,
  ERROR,
  FATAL,
  NUM_LOG_LEVELS,
};

// An arbitrary limit on the length of a single log message.  This
// is so that streaming can be done more efficiently.
static const size_t kMaxLogMessageLen = 30000;

class LogStreamBuf : public std::streambuf {
 public:
  // REQUIREMENTS: "len" must be >= 2 to account for the '\n' and '\0'.
  LogStreamBuf(char *buf, int len) {
    setp(buf, buf + len - 2);
  }

  // This effectively ignores overflow.
  virtual int_type overflow(int_type ch) {
    return ch;
  }

  // Legacy public ostrstream method.
  size_t pcount() const { return pptr() - pbase(); }
  char* pbase() const { return std::streambuf::pbase(); }
};

// for log stream
class LogStream : public std::ostream {
  public:
    LogStream(char *buf, int len, int ctr)
        : std::ostream(NULL),
          streambuf_(buf, len),
          ctr_(ctr),
          self_(this) {
      rdbuf(&streambuf_);
    }

    int ctr() const { return ctr_; }
    void set_ctr(int ctr) { ctr_ = ctr; }
    LogStream* self() const { return self_; }

    // Legacy std::streambuf methods.
    size_t pcount() const { return streambuf_.pcount(); }
    char* pbase() const { return streambuf_.pbase(); }
    char* str() const { return pbase(); }

  private:
    DISALLOW_COPY_AND_ASSIGN(LogStream);

    LogStreamBuf streambuf_;
    int ctr_;  // Counter hack (for the LOG_EVERY_X() macro)
    LogStream *self_;  // Consistency check hack
};
    
// where log data contents in
struct LogMessageData {
public:  
  char text_[kMaxLogMessageLen+1];
  LogStream stream_;

  LogMessageData()
    : stream_(text_, kMaxLogMessageLen, 0) {
  }
};

class LogMessage {
  public:
    LogMessage(const char* file, int line, LogLevel level, const char* func);
    ~LogMessage();

    std::ostream& Stream() {
      return data_->stream_;
    }

    static LogLevel logLevel();
    static void setLogLevel(LogLevel level);

    typedef void (*OutputFunc)(const char* msg, int len);
    typedef void (*FlushFunc)();
    static void setOutput(OutputFunc);
    static void setFlush(FlushFunc);

  private:
    void init();
    void finish();

  private:
    static LogLevel logLevel_;
    LogMessageData* data_;
    LogLevel level_;
};

extern LogLevel gLogLevel;

inline LogLevel LogMessage::logLevel() {
  return gLogLevel;
}

#define Debug if (serverkit::LogMessage::logLevel() <= serverkit::DEBUG) \
  serverkit::LogMessage(__FILE__, __LINE__, serverkit::DEBUG, __func__).Stream

#define Info if (serverkit::LogMessage::logLevel() <= serverkit::INFO) \
  serverkit::LogMessage(__FILE__, __LINE__, serverkit::INFO, __func__).Stream

#define Warn if (serverkit::LogMessage::logLevel() <= serverkit::WARN) \
  serverkit::LogMessage(__FILE__, __LINE__, serverkit::WARN, __func__).Stream

#define Error if (serverkit::LogMessage::logLevel() <= serverkit::ERROR) \
  serverkit::LogMessage(__FILE__, __LINE__, serverkit::ERROR, __func__).Stream

#define Fatal if (serverkit::LogMessage::logLevel() <= serverkit::FATAL) \
  serverkit::LogMessage(__FILE__, __LINE__, serverkit::FATAL, __func__).Stream

// This class is used to explicitly ignore values in the conditional
// logging macros.  This avoids compiler warnings like "value computed
// is not used" and "statement has no effect".
class LogMessageVoidify {
 public:
  LogMessageVoidify() { }

	void operator&(std::ostream&) { }
};

#define LOG_IF(condition) \
  !(condition) ? (void) 0 : LogMessageVoidify() \
  & serverkit::LogMessage(__FILE__, __LINE__, serverkit::FATAL, __func__).Stream()

#define Assert(condition) \
	LOG_IF(unlikely(!(condition))) << "Assertion failed: " #condition << " "
  
}  // namespace serverkit

#endif  // __SERVERKIT_BASE_LOG_H__
