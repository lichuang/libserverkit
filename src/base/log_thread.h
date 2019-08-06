/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_BASE_LOG_THREAD__
#define __SERVERKIT_BASE_LOG_THREAD__

#include <list>
#include "base/mutex.h"
#include "base/singleton.h"
#include "base/thread.h"
#include "core/event.h"

namespace serverkit {

//#define TIME_FORMAT_LENGTH sizeof("2018/01/01 00:00:00.123") - 1
static const int kTimeFormatLength = 84;

struct LogMessageData;
class Poller;

// the log thread MUST be singleton
class LogThread : public Thread, 
                  public Event {
public:
  LogThread();
  virtual ~LogThread();

  void Send(LogMessageData *);

  // virtual interface of Event
  virtual void In();

  virtual void Out();

  virtual void Timeout();

  uint64_t CurrentMs() const {
    return now_ms_;
  }

  const char* CurrentMsString() const { 
    return const_cast<char*>(&(now_str_[0]));
  }

private:
  void UpdateThreadTime();

protected:
  virtual void Run(void* arg);

private:
  typedef std::list<LogMessageData*> LogList;
  typedef LogList::iterator LogListIter;

  // one for write, another for read
  LogList log_list_[2];
  LogList *write_list_;
  LogList *read_list_;
  // write list index
  int index_;

  Mutex* mutex_;
  Poller* poller_;

  // time
  volatile uint64_t now_ms_;
  // now ms string
  volatile char now_str_[kTimeFormatLength];
};

void
SendLog(LogMessageData *data) {
  Singleton<LogThread>::Instance()->Send(data);
}

uint64_t CurrentMs() {
  return Singleton<LogThread>::Instance()->CurrentMs();
}

const char* CurrentMsString() {
  return Singleton<LogThread>::Instance()->CurrentMsString();
}

}; // namespace serverkit

#endif // __SERVERKIT_BASE_LOG_THREAD__