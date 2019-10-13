/*
 * Copyright (C) codedump
 */

#include <pthread.h>
#include <libgen.h>
#include <sys/utsname.h>
#include "util/misc.h"

namespace serverkit {

static string gHostName;
static pthread_once_t gMiscOnce = PTHREAD_ONCE_INIT;

static void
once_misc_run() {
  struct utsname buf;
  if (uname(&buf) == 0) {
    gHostName = buf.nodename;
  } else {
    gHostName = "(unknown host)";
  }
}

string* 
GetHostName() {
  pthread_once(&gMiscOnce,once_misc_run);

  return &gHostName;
}

}; // namespace serverkit