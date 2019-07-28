/*
 * Copyright (C) codedump
 */

#ifndef __QNODE_BASE_BUFFER_H__
#define __QNODE_BASE_BUFFER_H__

#include <string.h>
#include <list>
#include "base/macros.h"
#include "base/object_pool.h"

namespace serverkit {

static const int kBufferSize = 4096;

class Buffer {
public:
  Buffer() {
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
  int Avail() const { return static_cast<int>(End() - cur_); }

  // advance the current point
  void Add(size_t len) { cur_ += len; }

  // append the data to the buffer
  void Append(const char* buf, size_t len) {
    if (static_cast<size_t>(Avail()) > len) {
      memcpy(cur_, buf, len);
      cur_ += len;
    }
  }

  // zero the buffer
  void BZero() { memset(data_, '0', sizeof(data_)); }

  const char* End() const { return data_ + sizeof(data_); }

private:
  char data_[kBufferSize];
  char *cur_;

  DISALLOW_COPY_AND_ASSIGN(Buffer);
};

class BufferList {
public:
  BufferList()
    : read_inx_(0),
      write_inx_(0) {
    buffer_list_.push_back(GetObject<Buffer>());
  }

  void Append(const char* buf, size_t len);

  // read at most n bytes into to, return bytes actual read
  size_t Read(char* to, size_t n);

  // write n bytes into buffer list
  void Write(const char* from, size_t n);

  bool Empty() const {
    return TotalSize() == 0;
  }

  char* ReadPoint() {
    return buffer_list_.front()->Data() + read_inx_;
  }

  char* WritePoint() {
    return buffer_list_.back()->Data() + write_inx_;
  }

  size_t ReadableSize() const;
  size_t WriteableSize() const;

  void ReadAdvance(size_t n);

  void WriteAdvance(size_t n);

  Buffer* CurrentWrite() {
    return buffer_list_.back();
  }

  // return total size of the buffer list
  inline size_t TotalSize() const {
    return (buffer_list_.size() - 1) * kBufferSize
      + write_inx_ - read_inx_;
  }

private:
  // write and read index in current Buffer
  size_t read_inx_;
  size_t write_inx_;

  std::list<Buffer*> buffer_list_;

  DISALLOW_COPY_AND_ASSIGN(BufferList);
};

};  // namespace serverkit

#endif  // __QNODE_BASE_BUFFER_H__
