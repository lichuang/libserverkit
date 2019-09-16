/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_BASE_HANDLE_H__
#define __SERVERKIT_BASE_HANDLE_H__

namespace serverkit {

enum HandleType { 
  HANDLE_TYPE_TCP,
};

class Handle {
public:
  Handle(int fd, Poller *poller, HandleType type)
    : fd_(fd), poller_(poller), type_(type) {}
  virtual ~Handle() {}

  HandleType GetType() const {
    return type_;
  }

  Poller * GetPoller() const { 
    return poller_; 
  }

  int GetFd() const {
    return fd_;
  }
  
protected:
  int fd_;
  Poller *poller_;
  HandleType type_;
};

};  // namespace serverkit

#endif  // __SERVERKIT_BASE_HANDLE_H__