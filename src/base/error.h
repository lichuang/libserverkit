/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_BASE_ERROR_H__
#define __SERVERKIT_BASE_ERROR_H__

#include <stdio.h>
#include "base/likely.h"

namespace serverkit {

void PrintStack();
void Abort(const char *errmsg);

#define Assert(x)                                                 \
	do {                                                                       \
		if (unlikely (!(x))) {                                                 \
			fprintf (stderr, "Assertion failed: %s (%s:%d)\n", #x, __FILE__,   \
				__LINE__);                                                \
			fflush (stderr);                                                   \
			serverkit::Abort (#x);                                               \
		}                                                                      \
	} while (false)
  
};  // namespace serverkit

#endif  // __SERVERKIT_BASE_ERROR_H__
