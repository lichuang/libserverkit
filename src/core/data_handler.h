/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_CORE_DATA_HANDLER_H__
#define __SERVERKIT_CORE_DATA_HANDLER_H__

namespace serverkit {

// virtual interface for socket data handler
class DataHandler {
public:
  virtual ~DataHandler() {
  }

  virtual void OnWrite() = 0;
  virtual void OnRead() = 0;
  virtual void OnError(int err) = 0;
};

};  // namespace serverkit

#endif  // __SERVERKIT_CORE_DATA_HANDLER_H__
