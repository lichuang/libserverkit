/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_CORE_APPLICATION_H__
#define __SERVERKIT_CORE_APPLICATION_H__

#include <map>
#include <string>
#include <vector>
#include <google/protobuf/service.h>
#include "serverkit_types.h"
#include "base/endpoint.h"
#include "base/singleton.h"

using namespace std;
namespace gpb = ::google::protobuf;

namespace serverkit {

class AcceptorHandler;
class Listener;
class Worker;
class Poller;
class Session;
class SessionFactory;
class RpcChannel;
class RpcService;

class Application {
  friend class Singleton<Application>;

public:
  ~Application();

  // add a service listening to the endpoint
  void AddService(const Endpoint&, AcceptorHandler*);

  // add a rpc service listening to the endpoint
  void AddRpcService(const Endpoint&, gpb::Service* service);

  // create a rpc channel connect to the endpoint,
  // when bond to a worker, CreateChannelDone will be call
  void CreateRpcChannel(const Endpoint&, CreateChannelDone);

  void Init(const ServerkitOption&);
  void Run();

  void AcceptNewSession(Session*);
  
  Poller* GetPoller() {
    return poller_;
  }
  
private:
  Application();
  void doInit() {
    // nothing to do
  }

  Worker* selectWorker();

private:
  int index_;
  vector<Worker*> workers_;
  Poller *poller_;
  map<string, Listener*> listeners_;

  DISALLOW_COPY_AND_ASSIGN(Application);
};

#define gApplication Singleton<Application>::Instance()

};  // namespace serverkit
#endif  // __SERVERKIT_CORE_APPLICATION_H__
