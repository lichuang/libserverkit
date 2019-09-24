/*
 * Copyright (C) codedump
 */

#ifndef __QNODE_BASE_BUFFER_H__
#define __QNODE_BASE_BUFFER_H__

#include <string.h>
#include "base/macros.h"
#include "base/object_pool.h"

namespace serverkit {

static const int kBufferSize = 4096;
class BufferList;

class Buffer {
  friend class BufferList;
public:
  Buffer()
    : next_(NULL) {
    Reset();
  }

  ~Buffer() {}

  // return start point of the buffer
  char* Data() {
    return &(data_[0]);
  }

  // return current buffer data length
  int Length() const { 
    return static_cast<int>(cur_ - data_);
  }

  // clear the data, set current point to start
  void Reset() {
    memset(data_, '0', sizeof(data_));
    cur_ = data_;
  }

  // return current point of the data
  char* Current() { return cur_; }

  // return available size of the data
  int Available() const { return static_cast<int>(End() - cur_); }

  // append string to the buffer
  void AppendString(const string& str) {
    AppendData(str.c_str(), str.length());
  }

  // append the data to the buffer
  void AppendData(const char* buf, size_t len) {
    if (static_cast<size_t>(Available()) > len) {
      memcpy(cur_, buf, len);
      cur_ += len;
    }
  }

  const char* End() const { return data_ + sizeof(data_); }

private:
  char data_[kBufferSize];
  char *cur_;

  Buffer *next_;

  DISALLOW_COPY_AND_ASSIGN(Buffer);
};

class BufferList {
public:
  BufferList()
    : read_inx_(0),
      write_inx_(0),
      write_size_(0),
      read_size_(0) {
    head_ = GetObject<Buffer>();
    read_ = write_ = head_;
  }

  ~BufferList();

  // read at most n bytes into to, return bytes actual read
  size_t Read(char* to, size_t n);

  // write string into buffer list
  void WriteString(const string& str) {
    Write(str.c_str(), str.length());
  }

  // write n bytes into buffer list
  size_t Write(const char* from, size_t n);

  bool Empty() const {
    return ReadableSize() == 0;
  }

  char* ReadPoint() {
    return read_->Data() + read_inx_;
  }

  char* WritePoint() {
    return write_->Data() + write_inx_;
  }

  size_t ReadableSize() const;
  size_t WriteableSize() const;

  void ReadAdvance(size_t n);

  void WriteAdvance(size_t n);

  // return total size of the buffer list
  inline size_t TotalSize() const {
    return write_size_;
  }

  // return write size of the buffer list
  inline size_t WriteSize() const {
    return write_size_;
  }

  // return read size of the buffer list
  inline size_t ReadSize() const {
    return read_size_;
  }

private:
  size_t currentBufferReadSize();

private:
  // write and read index in current Buffer
  size_t read_inx_;
  size_t write_inx_;

  Buffer *head_;
  Buffer *read_;
  Buffer *write_;

  size_t write_size_, read_size_;

  DISALLOW_COPY_AND_ASSIGN(BufferList);
};

};  // namespace serverkit

#endif  // __QNODE_BASE_BUFFER_H__
