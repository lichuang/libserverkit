/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_BASE_LOG_STREAM_H__
#define __SERVERKIT_BASE_LOG_STREAM_H__

#include <string.h>
#include <string>
#include "base/buffer.h"

using namespace std;

namespace serverkit {

class LogStream {
 public:
  LogStream& operator<<(bool v) {
    buffer_.Append(v ? "1" : "0", 1);
    return *this;
  }

  LogStream& operator<<(short);
  LogStream& operator<<(unsigned short);
  LogStream& operator<<(int);
  LogStream& operator<<(unsigned int);
  LogStream& operator<<(long);
  LogStream& operator<<(unsigned long);
  LogStream& operator<<(long long);
  LogStream& operator<<(unsigned long long);

  LogStream& operator<<(const void*);

  LogStream& operator<<(float v) {
    *this << static_cast<double>(v);
    return *this;
  }
  LogStream& operator<<(double);

  LogStream& operator<<(char v) {
    buffer_.Append(&v, 1);
    return *this;
  }

  LogStream& operator<<(const char* str) {
    if (str) {
      buffer_.Append(str, strlen(str));
    } else {
      buffer_.Append("(null)", 6 /* sizeof("(null)") */ );
    }
    return *this;
  }

  LogStream& operator<<(const unsigned char* str) {
    return operator<<(reinterpret_cast<const char*>(str));
  }

  LogStream& operator<<(const string& v) {
    buffer_.Append(v.c_str(), v.size());
    return *this;
  }

  void Append(const char* data, int len) { 
    buffer_.Append(data, len);
  }

  Buffer* buffer() { return &buffer_; }
  void ResetBuffer() { buffer_.Reset(); }

 private:
  template<typename T>
  void formatInteger(T);

  Buffer buffer_;

  static const int kMaxNumericSize = 32;
};

};  // namespace serverkit

#endif  // __SERVERKIT_BASE_LOG_STREAM_H__
