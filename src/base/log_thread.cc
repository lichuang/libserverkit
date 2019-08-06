/*
 * Copyright (C) codedump
 */

#include "base/errcode.h"
#include "base/log.h"
#include "base/log_thread.h"
#include "base/macros.h"
#include "base/time.h"
#include "core/epoll.h"

namespace serverkit {

// update log thread time internal
static const int kUpdateTimeInterval = 10;

LogThread::LogThread()
	: Thread("log_thread"),
		index_(0),
		mutex_(new Mutex()),
		poller_(new Epoll()),
		now_ms_(NowMs()) {
	write_list_ = &(log_list_[index_]);
	read_list_ = &(log_list_[index_ + 1]);

  int rc = poller_->Init(1024);
  if (rc != kOK) {
    return;
  }

	poller_->AddTimer(kUpdateTimeInterval, this);
}

LogThread::~LogThread() {
	delete poller_;
	delete mutex_;
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
		read_list_ = &(log_list_[index_]);
		index_ = (index_ + 1) % 2;
		write_list_ = &(log_list_[index_]);
	}

	UpdateThreadTime();

	// TODO: optimize write log

	LogListIter iter;
	for (iter = read_list_->begin(); iter != read_list_->end(); ++iter) {
		LogMessageData* data = *iter;
		delete data;
	}
}

void
LogThread::UpdateThreadTime() {
  now_ms_ = NowMs();

  struct timeval t;
  ::gettimeofday(&t, NULL);
  struct tm tim;
  ::localtime_r(&t.tv_sec, &tim);
  int n = kTimeFormatLength;
  n+=1;
  snprintf(const_cast<char *>(now_str_), kTimeFormatLength,
    "%4d/%02d/%02d %02d:%02d:%02d.%03d",
    tim.tm_year + 1900, tim.tm_mon + 1, tim.tm_mday,
    tim.tm_hour, tim.tm_min, tim.tm_sec, static_cast<int>(t.tv_usec / 1000));
}

void
LogThread::Run(void* arg) {
	UNUSED(arg);

	poller_->Loop();
}

};  // namespace serverkit