/*
 * Copyright (C) codedump
 */

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "base/file.h"

namespace serverkit {

File::File(const Slice& file)
	: filename_(file) {
	fd_ = open(file.data(), O_CREAT | O_RDWR | O_APPEND, 0644);
	if (fd_ < 0) {
		//status_ = IOError(filename_, errno);
	}
}

File::~File() {
	close(fd_);
}

bool 
File::Append(const Slice& data) {
	if (write(fd_, data.data(), data.size()) < 0) {
		//status_ = IOError(filename_, errno);
		return false;
	}

	return true;
}

bool
File::Flush() {
	return File::Sync();
}

bool
File::Close() {
  if (close(fd_) < 0) {
    //status_ = IOError(filename_, errno);
		return false;
  }
	fd_ = -1;
	return true;
}

bool
File::Sync() {
	if (fsync(fd_) < 0) {
    //status_ = IOError(filename_, errno);
		return false;
  }

	return true;
}

};  // namespace serverkit