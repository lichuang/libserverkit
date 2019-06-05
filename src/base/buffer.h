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

static const int kBufferSize = 1024;

class Buffer {
public:
  Buffer() {
    Reset();
  }

  ~Buffer() {}

  char* Data() {
    return &(data_[0]);
  }

  void Reset() {
    memset(data_, '\0', sizeof(char) * kBufferSize);
  }

private:
  char data_[kBufferSize];

  DISALLOW_COPY_AND_ASSIGN(Buffer);
};

static thread_local ObjectPool<Buffer> gBufferPool;

class BufferList {
public:
  BufferList()
    : read_inx_(0),
      write_inx_(0) {
    buffer_list_.push_back(gBufferPool.Get());
  }

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

  inline size_t TotalSize() const {
    return (buffer_list_.size() - 1) * kBufferSize
      + write_inx_ - read_inx_;
  }

private:
  size_t read_inx_;
  size_t write_inx_;

  std::list<Buffer*> buffer_list_;

  DISALLOW_COPY_AND_ASSIGN(BufferList);
};

};  // namespace serverkit

#endif  // __QNODE_BASE_BUFFER_H__
