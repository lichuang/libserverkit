/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_REQUEST_CONTEXT_H__
#define __SERVERKIT_REQUEST_CONTEXT_H__

#include <google/protobuf/message.h>
#include <google/protobuf/service.h>

namespace gpb = ::google::protobuf;

namespace serverkit {

class RpcController;

struct RequestContext {
  RpcController* controller;
  gpb::Message *response;
  gpb::Closure *done;

  RequestContext(
      RpcController* c,
      gpb::Message* resp,
      gpb::Closure* func)
    : controller(c), response(resp), done(func) {
  }

  ~RequestContext() {
    if (done) {
      delete done;
      done = NULL;
    }
  }

  void Run() {
    gpb::Closure* cb = done;
    done = NULL;
    if (cb) {
      cb->Run();
    }
  }
};

};  // namespace serverkit

#endif  // __SERVERKIT_REQUEST_CONTEXT_H__