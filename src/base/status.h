/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_BASE_STATUS_H__
#define __SERVERKIT_BASE_STATUS_H__

#include <string>
#include "base/slice.h"

namespace serverkit {
class Status {
 public:
  // Create a success status.
  Status() : state_(NULL) { }
  ~Status() { delete[] state_; }

  // Copy the specified status.
  Status(const Status& s);
  void operator=(const Status& s);

  // Return a success status.
  static Status OK() { return Status(); }

  // Return error status of an appropriate type.
  static Status IOError(const Slice& msg, int err, const Slice& msg2 = Slice()) {
    return Status(kIOError, msg, msg2, err);
  }
  
  static Status SysError(const Slice& msg, int err, const Slice& msg2 = Slice()) {
    return Status(kSysError, msg, msg2, err);
  }

  static Status AgainError(const Slice& msg, int err, const Slice& msg2 = Slice()) {
    return Status(kTryAgain, msg, msg2, err);
  }

  // Returns true iff the status indicates success.
  bool Ok() const { return (state_ == NULL); }

  // Returns true iff the status indicates an IOError.
  bool IsIOError() const { return code() == kIOError; }

  // Returns true iff the status indicates an kSysError.
  bool IsSysError() const { return code() == kSysError; }

  // Return a string representation of this status suitable for printing.
  // Returns the string "OK" for success.
  std::string String() const;

  int ErrorNum() const { 
    return err_num_;
  }

 private:
  int err_num_;

  // OK status has a NULL state_.  Otherwise, state_ is a new[] array
  // of the following form:
  //    state_[0..3] == length of message
  //    state_[4]    == code
  //    state_[5..]  == message
  const char* state_;

  enum Code {
    kOk = 0,
    kIOError = 1,
    kSysError = 2,
    kTryAgain = 3,
  };

  Code code() const {
    return (state_ == NULL) ? kOk : static_cast<Code>(state_[4]);
  }

  Status(Code code, const Slice& msg, const Slice& msg2, int err);
  static const char* CopyState(const char* s);
};

inline Status::Status(const Status& s) {
  state_ = (s.state_ == NULL) ? NULL : CopyState(s.state_);
}

inline void 
Status::operator=(const Status& s) {
  // The following condition catches both aliasing (when this == &s),
  // and the common case where both s and *this are ok.
  if (state_ != s.state_) {
    delete[] state_;
    if (s.state_) {
      state_ = CopyState(s.state_);
      err_num_ = s.err_num_;
    } else {
      state_ = NULL;
      err_num_ = 0;
    }
  }
}

extern Status IOError(const Slice& context, int err_number);
extern Status SysError(const Slice& context, int err_number);
extern Status TryAgain(const Slice& context, int err_number);

};  // namespace serverkit

#endif // __SERVERKIT_BASE_STATUS_H__