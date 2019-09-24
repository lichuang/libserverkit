/*
 * Copyright (C) codedump
 */

#include <string.h>
#include "base/buffer.h"

namespace serverkit {

BufferList::~BufferList() {
  while (head_ != NULL) {
    Buffer* buffer = head_;
    head_ = head_->next_;
    FreeObject<Buffer>(buffer);
  }
}

size_t 
BufferList::Read(char* to, size_t n) {
  size_t orig = n;
  size_t read_size, current_size;

  while (n > 0) {
    read_size = ReadableSize();
    if (read_size == 0) {
      break;
    }

    current_size = currentBufferReadSize();

    if (n > current_size) {
      memcpy(to, ReadPoint(), current_size);
      ReadAdvance(current_size);
      to += current_size;
      n  -= current_size;
    } else {
      memcpy(to, ReadPoint(), n);
      ReadAdvance(n);
      n = 0;
      break;
    }
  }

  return orig - n;
}

size_t 
BufferList::Write(const char* from, size_t n) {
  size_t write_size, total = 0;

  while (n > 0) {
    write_size = WriteableSize();

    if (n > write_size) {
      memcpy(WritePoint(), from, write_size);
      WriteAdvance(write_size);
      from += write_size;
      n    -= write_size;
      total += write_size;
    } else {
      memcpy(WritePoint(), from, n);
      WriteAdvance(n);
      total += n;
      break;
    }
  }

  return total;
}

void 
BufferList::ReadAdvance(size_t n) {
  read_inx_ += n;
  read_size_ += n;

  // a buffer has been read out, pop it from list
  if (read_inx_ == kBufferSize) {
    Buffer *buffer = read_;
    read_ = read_->next_;
    head_ = read_;
    read_inx_ = 0;
    FreeObject<Buffer>(buffer);
  }
}

void 
BufferList::WriteAdvance(size_t n) {
  write_inx_ += n;
  write_size_ += n;
  // a buffer has been filled in, create a new buffer into list
  if (write_inx_ == kBufferSize) {
    Buffer* buffer = GetObject<Buffer>();
    write_->next_ = buffer;
    write_ = buffer;    
    write_inx_ = 0;
  }
}

size_t 
BufferList::ReadableSize() const {
  return write_size_ - read_size_;
}

size_t 
BufferList::WriteableSize() const {
  return kBufferSize - write_inx_;
}

size_t 
BufferList::currentBufferReadSize() {
  // is the same buffer?
  if (read_ == write_) {
    return write_inx_ - read_inx_;
  } 

  return kBufferSize - read_inx_;
}
};  // namespace serverkit
