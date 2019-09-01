/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_BASE_NET_H__
#define __SERVERKIT_BASE_NET_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include "base/endpoint.h"
#include "base/errcode.h"
#include "base/status.h"

using namespace std;

namespace serverkit {

class BufferList;

int   Listen(const Endpoint&, int backlog, Status *status);
int   Connect(const Endpoint&, Status *status, int *fd);

int   Accept(int listen_fd, Endpoint* endpoint, Status *status);

int   Recv(int fd, BufferList *buffer, Status *status);
int   Send(int fd, BufferList *buffer, Status *status);
void  Close(int fd);

int   MakeFdPair(int *w, int *r);

};  // namespace serverkit

#endif  // __SERVERKIT_BASE_NET_H__
