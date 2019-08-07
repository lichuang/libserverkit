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
class File;
class Poller;

// the log thread MUST be singleton
class LogThread : public Thread, 
                  public Event {
  friend class Singleton<LogThread>;
public:
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
  LogThread();
  void updateTime();
  void output(LogMessageData*);

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
  int write_index_;

  Mutex* mutex_;
  Poller* poller_;

  // time
  volatile uint64_t now_ms_;
  // now ms string
  volatile char now_str_[kTimeFormatLength];

  // log file
  File* file_;
};

extern void SendLog(LogMessageData *data);
extern uint64_t CurrentMs();
extern const char* CurrentMsString();

}; // namespace serverkit

#endif // __SERVERKIT_BASE_LOG_THREAD__