/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_CORE_SOCKET_H__
#define __SERVERKIT_CORE_SOCKET_H__

#include <string>
#include "base/buffer.h"
#include "base/macros.h"
#include "core/event.h"

using namespace std;

namespace serverkit {

class DataHandler;
class Poller;

class Socket : public Event {
public:
  Socket(int fd, const string& addr, DataHandler*);

  virtual ~Socket();

  void SetPoller(Poller *);

  size_t ReadBufferSize() const {
    return read_list_.TotalSize();
  }

  size_t WriteBufferSize() const {
    return write_list_.TotalSize();
  }

  void Write(const char* from, size_t n);
  size_t Read(char* to, size_t n);

  virtual void In();

  virtual void Out();

  virtual void Timeout();

  int    ResetIn(); 
  int    SetIn(); 
  int    ResetOut(); 
  int    SetOut(); 

  const string& String() const {
    return addr_;
  }
private:
  void CloseSocket();

private:  
  int fd_;
  DataHandler *handler_;
  Poller* poller_;
  bool is_writable_;
  BufferList read_list_;
  BufferList write_list_;
  string addr_;

  DISALLOW_COPY_AND_ASSIGN(Socket);
};

};  // namespace serverkit

#endif  // __SERVERKIT_CORE_SOCKET_H__
