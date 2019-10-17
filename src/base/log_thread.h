/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_BASE_LOG_THREAD__
#define __SERVERKIT_BASE_LOG_THREAD__

#include <list>
#include "serverkit_types.h"
#include "base/mutex.h"
#include "base/singleton.h"
#include "base/thread.h"
#include "core/event.h"

namespace serverkit {

struct LogMessageData;
class File;
class Poller;

typedef std::list<LogMessageData*> LogList;
typedef LogList::iterator LogListIter;

// the log thread MUST be singleton
class LogThread : public Thread, 
                  public Event {
  friend class Singleton<LogThread>;
public:
  virtual ~LogThread();

  void Init(int argc, char *argv[], const ServerkitOption&);

  void Send(LogMessageData *);
  void Flush(bool end);

  // virtual interface of Event
  virtual void In();

  virtual void Out();

  virtual void Timeout();

protected:
  virtual void Run();
  
private:
  LogThread();
  void iterList(LogList*);
  void output(LogMessageData*);
  void flush();
  void doInit();

private:
  // one for write, another for read
  LogList log_list_[2];
  LogList *write_list_;
  LogList *read_list_;
  // write list index
  int write_index_;

  Mutex* mutex_;
  Poller* poller_;

  // last flush time
  uint64_t last_flush_time_;
  
  // log file
  File* file_;
  // application name
  string app_name_;
  // log path
  string log_path_;
};

#define gLogThread serverkit::Singleton<serverkit::LogThread>::Instance()

extern void SendLog(LogMessageData *data);
extern void Flush(bool end);

}; // namespace serverkit

#endif // __SERVERKIT_BASE_LOG_THREAD__