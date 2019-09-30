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
class Socket;

int   Listen(const Endpoint&, int backlog, int *err);
int   ConnectAsync(const Endpoint&, int);

int   Accept(int listen_fd, int*);

// recv data from socket, put it into buffer,
// if some error occurred, save error in err param
int   Recv(Socket *, BufferList *buffer, int *err);

// send data to socket from buffer,
// if some error occurred, save error in err param
int   Send(Socket *, BufferList *buffer, int *err);

void  Close(int fd);
void  GetEndpointByFd(int fd, Endpoint*);
int   MakeFdPair(int *w, int *r);

int   TcpSocket();

};  // namespace serverkit

#endif  // __SERVERKIT_BASE_NET_H__
