/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_BASE_NET_H__
#define __SERVERKIT_BASE_NET_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include "base/errcode.h"

using namespace std;

namespace serverkit {

class BufferList;

int   Listen(const string& addr, int port, int backlog, int *error);
int   Connect(const string& addr, int port, int *error, int *fd);

int   Accept(int listen_fd, string *addr, int *error);

int   Recv(int fd, BufferList *buffer, int *error);
int   Send(int fd, BufferList *buffer, int *error);
void  Close(int fd);

int   MakeFdPair(int *w, int *r);

};  // namespace serverkit

#endif  // __SERVERKIT_BASE_NET_H__
