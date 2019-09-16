/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_BASE_STREAM_H__
#define __SERVERKIT_BASE_STREAM_H__

#include "base/handle.h"

namespace serverkit {

class Stream : public Handle {
public:
  Stream(int fd, Poller *poller, HandleType type)
    : Handle(fd, poller, type) {}
    
  virtual ~Stream() {}
};

};  // namespace serverkit

#endif // __SERVERKIT_BASE_STREAM_H__