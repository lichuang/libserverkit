/*
 * Copyright (C) codedump
 */

#include <sys/eventfd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "base/buffer.h"
#include "base/net.h"
#include "base/string.h"

namespace serverkit {

static int createListenSocket();
static int setNonBlocking(int fd);

static int
createListenSocket() {
  int fd, on;

  on = 1;
  if ((fd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
    return kError;
  }

  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
    return kError;
  }

  return fd;
}

static int
setNonBlocking(int fd) {
  int flags;

  if ((flags = fcntl(fd, F_GETFL)) == -1) {
    return kError;
  }
  if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
    return kError;
  }

  return kOK;
}

int
Listen(const string& addr, int port, int backlog, int *error) {
  int                 fd;
  struct sockaddr_in  sa;

  if ((fd = createListenSocket()) < 0) {
    return kError;
  }

  if (setNonBlocking(fd) < 0) {
    close(fd);
    return kError;
  }

  memset(&sa,0,sizeof(sa));
  sa.sin_family = AF_INET;
  sa.sin_port = htons(static_cast<uint16_t>(port));
  *error = 0;
  if (inet_aton(addr.c_str(), &sa.sin_addr) == 0) {
    *error = errno;
    close(fd);
    return kError;
  }

  if (bind(fd, reinterpret_cast<struct sockaddr*>(&sa), sizeof(sa)) < 0) {
    *error =  errno;
    close(fd);
    return kError;
  }

  if (listen(fd, backlog) == -1) {
    *error =  errno;
    close(fd);
    return kError;
  }

  return fd;
}

int
Accept(int listen_fd, string *ret, int *error) {
  struct sockaddr_in addr;
  socklen_t addrlen = sizeof(addr);
  int fd;

  while (true) {
    fd = accept(listen_fd, reinterpret_cast<struct sockaddr *>(&addr), &addrlen);
    if (fd == -1) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        *error = kAgain;
        return kOK;
      }
      if (errno == EINTR) {
        continue;
      }
      *error = errno;
      return kError;
    }
    Stringf(ret, "%s:%d", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
    setNonBlocking(fd);
    break;
  }
  return fd;
}

int
Recv(int fd, BufferList *buffer, int *error) {
  ssize_t nbytes;
  int ret;

	/*
	 * recv data from tcp socket until:
	 *  1) some error occur or
	 *  2) received data less then required size
	 *    (means there is no data in the tcp stack buffer)
	 */
  nbytes = 0;
  ret = 0;
  while(true) {
    nbytes = ::read(fd, buffer->WritePoint(), buffer->WriteableSize());
    if (nbytes > 0) {
      buffer->WriteAdvance(nbytes);
      ret += static_cast<int>(nbytes);
    } else if (nbytes < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        // there is nothing in the tcp stack,return and wait for the next in event
        *error = kAgain;
        break;        
      } else if (errno == EINTR) {
        continue;
      } else {
        // something wrong has occoured
        *error = errno;
        return kError;
      }
    } else {
      // socket has been closed
      *error = kClosed;
      return kError;
    }
  };

  return ret;
}

int
Send(int fd, BufferList *buffer, int *error) {
  ssize_t nbytes;
  int ret;
  int err;

  nbytes = 0;
  ret = 0;
  while (true) {
    if (buffer->Empty())  {
      // there is nothing in user-space stack to send
      break;
    }
    nbytes = ::write(fd, buffer->ReadPoint(), buffer->ReadableSize());
    err = errno;

    if (nbytes > 0) {
      buffer->ReadAdvance(nbytes);
      ret += static_cast<int>(nbytes);
    } else if (nbytes < 0) {
      if (err == EINTR) {
        continue;
      } else if (err == EAGAIN || errno == EWOULDBLOCK) {
        *error = kAgain;
        break;
      } else {
        *error = err;
        return kError;
      }
    } else {
      // connection has been closed
      *error = kClosed;
      return kError;
    }
  }

  return ret;
}

void
Close(int fd) {
  close(fd);
}

int
MakeFdPair(int *w, int *r) {
  int fd = eventfd(0, EFD_NONBLOCK);
  if (fd == -1) {
    *w = *r = -1;
    return kError;
  }

  *w = *r = fd;
  return kOK;
}

};  // namespace serverkit
