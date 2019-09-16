/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_TYPES_H__
#define __SERVERKIT_TYPES_H__

#include <string>

using namespace std;

namespace serverkit {

class RpcChannel;

typedef void (*CreateChannelDone)(RpcChannel*);

struct ServerkitOption {
  ServerkitOption()
    : log_path("/tmp/"), 
      worker_num(1) {
  }
  
  string log_path;
  int worker_num;
};

};  // namespace serverkit

#endif  //  __SERVERKIT_TYPES_H__