/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_CORE_SERVER_H__
#define __SERVERKIT_CORE_SERVER_H__

#include <map>
#include <string>
#include <vector>
#include <google/protobuf/service.h>
#include "base/endpoint.h"
#include "base/singleton.h"

using namespace std;
namespace gpb = ::google::protobuf;

namespace serverkit {

class AcceptorHandler;
class Listener;
class IOThread;
class Poller;
class Session;
class SessionFactory;
class RpcService;

struct ServerOption {
  ServerOption();

  int worker_num;
};

class Server {
  friend class Singleton<Server>;

public:
  ~Server();

  void AddService(const Endpoint&, AcceptorHandler*);
  void AddRpcService(const Endpoint&, gpb::Service* service);

  void Run(const ServerOption&);

  void AcceptNewSession(Session*);

  Poller* GetPoller() {
    return poller_;
  }
  
private:
  Server();
  void doInit() {
    // nothing to do
  }

private:
  int index_;
  vector<IOThread*> workers_;
  Poller *poller_;
  map<string, Listener*> listeners_;

  DISALLOW_COPY_AND_ASSIGN(Server);
};

#define gServer Singleton<Server>::Instance()

};  // namespace serverkit
#endif  // __SERVERKIT_CORE_SERVER_H__
