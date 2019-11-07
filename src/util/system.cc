/*
 * Copyright (C) codedump
 */

#include <execinfo.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <libgen.h>
#include <sys/utsname.h>
#include "util/string.h"

namespace serverkit {

const static int kMaxCallStack = 50;

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

void
GetCallStack(string *result) {
  void *buffer[kMaxCallStack] = {NULL};

  *result = "";

  int level = backtrace(buffer, sizeof(buffer));
  if (level < 2) {
    return;
  }
  char cmd[1024] = "addr2line -Cif -e ";

  //char* prog = cmd + strlen(cmd);
  int i;

  *result = "\n@========Stack Frame:========@\n";

  //if (readlink("/proc/self/exe", prog, sizeof(cmd) - (prog-cmd)-1) == -1) {
  if (true) {
    char **stack_strings = NULL;
    stack_strings = (char **)backtrace_symbols(buffer, level);
	  if (!stack_strings) {
		  return;
	  }
    for (i = 0; i < level; ++i) {
      Appendf(result, "  @[%03d]  %s\n", level-i, stack_strings[i]);
      //*result += "  @  " + string(stack_strings[i]) + "\n";
    }
    *result += "@========End Of Stack Frame========@\n";
    free(stack_strings);
    return;
  }

  int n = strlen(cmd);
  cmd[n] = ' ';
  
  string tmp;
  for (i = 0; i < level - 2; ++i) {
    snprintf(cmd + n + 1, sizeof(cmd) - n - 1, " %p", buffer[i]);
    if (!PopenToString(cmd, &tmp)) {
      return;
    }

    string::size_type pos = tmp.find_first_of("\n");
    tmp[pos] = ' ';
    *result += "  @  " + tmp;
  }
  *result += "@========End Of Stack Frame========@\n";
  return;
}

};  // namespace serverkit