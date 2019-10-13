/*
 * Copyright (C) codedump
 */

#include "base/errcode.h"
#include "base/file.h"
#include "base/log.h"
#include "base/log_thread.h"
#include "base/macros.h"
#include "base/string.h"
#include "base/time.h"
#include "core/epoll.h"
#include "util/misc.h"

namespace serverkit {

// update log thread time internal
static const int kUpdateTimeInterval = 100;
// flush log time internal
static const int kFlushTimeInterval = 500;

LogThread::LogThread()
	: write_index_(0),
		mutex_(new Mutex()),
		poller_(new Epoll()),
		last_flush_time_(0),
		file_(NULL) {
	write_list_ = &(log_list_[write_index_]);
	read_list_  = &(log_list_[write_index_ + 1]);

  int rc = poller_->Init(1024);
  if (rc != kOK) {
    return;
  }

	updateTime();
	poller_->AddTimer(kUpdateTimeInterval, this, kTimerPermanent);

	thread_ = new Thread("logger", this);
	thread_->Start();
}

LogThread::~LogThread() {
	// write log to disk
	iterList(read_list_);
	iterList(write_list_);
	flush();

	delete poller_;
	delete mutex_;
}

void
LogThread::doInit() {
	// start thread
	//Start();
}

void 
LogThread::Init(int argc, char *argv[], const ServerkitOption& option) {
	app_name_ = basename(argv[0]);
	log_path_ = option.log_path;
}

void
LogThread::Send(LogMessageData *data) {
	MutexGuard guard(mutex_);
	write_list_->push_back(data);
}

void
LogThread::In() {
	// nothing to do
}

void 
LogThread::Out() {
	// nothing to do
}

void 
LogThread::Timeout() {
	// exchange write and read list
	if (read_list_->empty()) {
		MutexGuard guard(mutex_);
		read_list_ = &(log_list_[write_index_]);
		write_index_ = (write_index_ + 1) % 2;
		write_list_ = &(log_list_[write_index_]);
	}

	updateTime();

	// TODO: optimize write log
	// is there any log to output?
	iterList(read_list_);

	// if need to flush file?
	if (now_ms_ - last_flush_time_ >= kFlushTimeInterval) {
		flush();
	}
}

void 
LogThread::iterList(LogList* lst) {
	if (!lst->empty()) {
		LogListIter iter;
		for (iter = lst->begin(); iter != lst->end(); ) {
			LogMessageData* data = *iter;
			++iter;
			output(data);
			delete data;
		}
		lst->clear();
	}
}

void
LogThread::updateTime() {
  struct timeval t;
  ::gettimeofday(&t, NULL);
	
	now_ms_ = t.tv_sec * 1000 + t.tv_usec / 1000;
  struct tm tim;
	Localtime(t.tv_sec, &tim);
  int n = kTimeFormatLength;
  n+=1;
  snprintf(const_cast<char *>(now_str_), kTimeFormatLength,
    "%4d/%02d/%02d %02d:%02d:%02d.%03d",
    tim.tm_year + 1900, tim.tm_mon + 1, tim.tm_mday,
    tim.tm_hour, tim.tm_min, tim.tm_sec, static_cast<int>(t.tv_usec / 1000));
}

void 
LogThread::output(LogMessageData* data) {
	if (file_ == NULL) {
		string *host_name = GetHostName();
		string file_name = Stringf("%s/%s-%s.log", log_path_.c_str(),
															 app_name_.c_str(), host_name->c_str());
		file_ = new File(file_name);
	}
	file_->Append(Slice(data->text_, data->stream_.pcount()));
}

void 
LogThread::flush() {
	if (file_ != NULL) {
		file_->Flush();
	}
	last_flush_time_ = now_ms_;
}

void
LogThread::Run() {
  while (thread_->Running()) {
    poller_->Dispatch();
  }
}

void
LogThread::Flush(bool end) {
	if (end) {
		iterList(read_list_);
		iterList(write_list_);
	}
	flush();	
}

void
SendLog(LogMessageData *data) {
  Singleton<LogThread>::Instance()->Send(data);
}

uint64_t 
CurrentLogTime() {
  return Singleton<LogThread>::Instance()->CurrentMs();
}

const char* 
CurrentLogTimeString() {
  return Singleton<LogThread>::Instance()->CurrentMsString();
}

void 
Flush(bool end) {
	Singleton<LogThread>::Instance()->Flush(end);
}

};  // namespace serverkit