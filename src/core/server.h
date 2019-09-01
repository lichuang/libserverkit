/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_CORE_SERVER_H__
#define __SERVERKIT_CORE_SERVER_H__

#include <map>
#include <string>
#include <vector>
#include "core/acceptor_handler.h"

using namespace std;

namespace serverkit {

class Listener;
class IOThread;
class Poller;
class Session;
class SessionFactory;

class Server : public AcceptorHandler {
public:
  Server(int worker);
  ~Server();

  virtual void OnAccept(Session*);
  virtual void OnError(const Status& status);

  void Listen(const string& addr, int port, SessionFactory*);
  void Run();

private:
  int index_;
  vector<IOThread*> workers_;
  Poller *poller_;
  map<string, Listener*> listeners_;

  DISALLOW_COPY_AND_ASSIGN(Server);
};

};  // namespace serverkit
#endif  // __SERVERKIT_CORE_SERVER_H__
