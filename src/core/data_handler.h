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

  virtual void OnWrite() { 

  }

  virtual void OnRead() { 

  }
  
  virtual void OnConnect(int error) {

  }

  virtual void OnError(int error) {

  }
};

};  // namespace serverkit

#endif  // __SERVERKIT_CORE_DATA_HANDLER_H__
