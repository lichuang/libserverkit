/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_BASE_FILE_H__
#define __SERVERKIT_BASE_FILE_H__

#include <string>
#include "base/slice.h"
#include "base/status.h"

using namespace std;

namespace serverkit {
class File {
public:
	File(const Slice&);
	~File();

	Status Append(const Slice&);
	Status Flush();
	Status Close();
	Status Sync();

	const Status& status() const { return status_; }

private:
	int fd_;
	Slice filename_;
	Status status_;
};

};  // namespace serverkit

#endif // __SERVERKIT_BASE_FILE_H__