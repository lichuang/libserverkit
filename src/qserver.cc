/*
 * Copyright (C) codedump
 */
#include <string>
#include "base/log.h"
#include "core/server.h"
using namespace std;

int main() {
  //serverkit::InitLog("./", serverkit::LOG_DEBUG);

  serverkit::Server server(1);
  server.Run();
  Info << "test";

  return 0;
}
