/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_BASE_FILE_H__
#define __SERVERKIT_BASE_FILE_H__

#include <string>
#include "base/slice.h"

using namespace std;

namespace serverkit {
class File {
public:
	File(const Slice&);
	~File();

	bool Append(const Slice&);
	bool Flush();
	bool Close();
	bool Sync();
	
private:
	int fd_;
	Slice filename_;
};

};  // namespace serverkit

#endif // __SERVERKIT_BASE_FILE_H__