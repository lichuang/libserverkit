/*
 * Copyright (C) codedump
 */

#include <stdlib.h>
#include "base/error.h"
#include "base/macros.h"

namespace serverkit {

void PrintStack() {
}

void Abort(const char *errmsg) {
  UNUSED(errmsg);
  PrintStack();
  abort();
}

}; //  namespace serverkit

