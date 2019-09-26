/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_CORE_EPOLL_H__
#define __SERVERKIT_CORE_EPOLL_H__

#include <list>
#include <vector>
#include <sys/epoll.h>
#include "core/event.h"
#include "core/poller.h"

using namespace std;

namespace serverkit {

struct EpollEntry {
  fd_t fd;
  epoll_event ev;
  Event *event;
  int flags;
};

class Epoll : public Poller {
public:
  Epoll();

  virtual ~Epoll();

  virtual int       Init(int size);
  virtual handle_t  Add(fd_t fd, Event *event);
  virtual int       Del(handle_t handle); 
  virtual void      MarkWriteable(handle_t handle);

protected:
  virtual int    Poll(int timeout);
  
private:  
  void   processRetired();

private:
  int    size_;
  int    epoll_fd_;
  vector<epoll_event> ep_events_;

  typedef list<EpollEntry*> EntryList;
  typedef list<EpollEntry*>::iterator EntryListIter;
  EntryList retired_list_;
  EntryList writeable_list_;
};

};  // namespace serverkit

#endif  // __SERVERKIT_CORE_EPOLL_H__
