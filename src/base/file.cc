/*
 * Copyright (C) codedump
 */

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "base/file.h"
#include "base/status.h"

namespace serverkit {

File::File(const Slice& file)
	: filename_(file),
		status_(Status::OK()) {
	fd_ = open(file.data(), O_CREAT | O_RDWR | O_APPEND, 0644);
	if (fd_ < 0) {
		status_ = IOError(filename_, errno);
	}
}

File::~File() {
	close(fd_);
}

Status 
File::Append(const Slice& data) {
	if (write(fd_, data.data(), data.size()) < 0) {
		status_ = IOError(filename_, errno);
	}

	return status_;
}

Status
File::Flush() {
	return File::Sync();
}

Status
File::Close() {
  if (close(fd_) < 0) {
    status_ = IOError(filename_, errno);
  }
	fd_ = -1;
	return status_;
}

Status
File::Sync() {
	if (fsync(fd_) < 0) {
    status_ = IOError(filename_, errno);
  }

	return status_;
}

};  // namespace serverkit