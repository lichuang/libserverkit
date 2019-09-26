/*
 * Copyright (C) codedump
 */

#include <string.h>
#include <gtest/gtest.h>
#include "core/server.h"
#include "core/session.h"
#include "core/data_handler.h"
#include "core/acceptor_handler.h"

using namespace serverkit;

class EchoSession : public Session {
public:
  EchoSession(int fd): Session(fd) {

  }

  virtual ~EchoSession() {

  }

  virtual void OnWrite() {
    
  }

  virtual void OnRead() {
    
  }
  
  virtual void OnError(int error) {

  }
};

class EchoSessionFactory: public SessionFactory {
public:
  EchoSessionFactory(){}
  virtual ~EchoSessionFactory() {
  }

  virtual Session* NewSession(int fd) {
    return new EchoSession(fd);
  }
};

class EchoServiceTest: public testing::Test {
public:

public:
  void SetUp() {

  }
};

class EchoService : public AcceptorHandler {
public:
  EchoService(SessionFactory* factory)
    : AcceptorHandler(factory) {
  }

  virtual ~EchoService() {

  }  

  virtual Session* OnAccept(int fd) {
    Session *session = factory_->NewSession(fd);
    return session;
  }

  virtual void OnError(int error) {

  }
};

TEST_F(EchoServiceTest, Echo) {
  EchoService *service = new EchoService(new EchoSessionFactory());
  gServer->AddService(Endpoint("127.0.0.1", 22222), service);
}