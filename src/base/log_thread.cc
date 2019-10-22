/*
 * Copyright (C) codedump
 */

#include "base/errcode.h"
#include "base/file.h"
#include "base/log.h"
#include "base/log_thread.h"
#include "base/macros.h"
#include "base/string.h"
#include "util/time.h"
#include "core/epoll.h"
#include "core/timer_thread.h"
#include "util/misc.h"

namespace serverkit {

// update log thread time internal
static const int kUpdateTimeInterval = 100;
// flush log time internal
static const int kFlushTimeInterval = 500;

LogThread::LogThread()
	: Thread("logger"),
		write_index_(0),
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

	poller_->AddTimer(kUpdateTimeInterval, this, kTimerPermanent);

	Start();
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

	// TODO: optimize write log
	// is there any log to output?
	iterList(read_list_);

	// if need to flush file?
	if (CurrentMs() - last_flush_time_ >= kFlushTimeInterval) {
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
	last_flush_time_ = CurrentMs();
}

void
LogThread::Run() {
  while (Running()) {
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

void 
Flush(bool end) {
	Singleton<LogThread>::Instance()->Flush(end);
}

};  // namespace serverkit