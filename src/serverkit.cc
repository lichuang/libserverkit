/*
 * Copyright (C) codedump
 */

#include "serverkit.h"
#include "base/time.h"

namespace serverkit {

Status 
ServerkitInit(int argc, char *args[]) {
	serverkit::Singleton<serverkit::LogThread>::Instance();
	TZSet();
	
	return Status::OK();
}

};  // namespace serverkit