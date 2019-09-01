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

static int createListenSocket(Status *status);
static Status setNonBlocking(int fd);

static int
createListenSocket(Status *status) {
  int fd, on;

  on = 1;
  if ((fd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
    *status = SysError("create socket", errno);
    return -1;
  }

  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
    *status = SysError("setsockopt", errno);
    return -1;
  }

  return fd;
}

static Status
setNonBlocking(int fd) {
  int flags;

  if ((flags = fcntl(fd, F_GETFL)) == -1) {
    return SysError("fcntl F_GETFL", errno);
  }
  if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
    return SysError("fcntl F_SETFL", errno);
  }

  return Status();
}

int
Listen(const string& addr, int port, int backlog, Status *status) {
  int                 fd;
  struct sockaddr_in  sa;

  fd = createListenSocket(status);
  if (fd < 0) {
    return kError;
  }

  *status = setNonBlocking(fd);
  if (!status->Ok()) {
    goto error;
  }

  memset(&sa,0,sizeof(sa));
  sa.sin_family = AF_INET;
  sa.sin_port = htons(static_cast<uint16_t>(port));

  if (inet_aton(addr.c_str(), &sa.sin_addr) == 0) {
    *status = SysError("inet_aton", errno);
    goto error;
  }

  if (bind(fd, reinterpret_cast<struct sockaddr*>(&sa), sizeof(sa)) < 0) {
    *status = SysError("bind", errno);
    goto error;
  }

  if (listen(fd, backlog) == -1) {
    *status = SysError("listen", errno);
    goto error;
  }

  return fd;

error:
  close(fd);
  return kError;
}

int
Accept(int listen_fd, string *ret, Status *status) {
  struct sockaddr_in addr;
  socklen_t addrlen = sizeof(addr);
  int fd;

  while (true) {
    fd = ::accept(listen_fd, reinterpret_cast<struct sockaddr *>(&addr), &addrlen);
    if (fd == -1) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        *status = TryAgain("accept", errno);
        return kOK;
      }
      if (errno == EINTR) {
        continue;
      }
      *status = SysError("accept", errno);
      return kError;
    }
    
    *status = setNonBlocking(fd);
    if (!status->Ok()) {
      return kError;
    }
    Stringf(ret, "%s:%d", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
    break;
  }
  return fd;
}

int
Recv(int fd, BufferList *buffer, Status *status) {
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
        *status = TryAgain("read", errno);
        break;        
      } else if (errno == EINTR) {
        continue;
      } else {
        // something wrong has occoured
        *status = SysError("read", errno);
        return kError;
      }
    } else {
      // socket has been closed
      *status = SysError("read", errno);
      return kError;
    }
  };

  return ret;
}

int
Send(int fd, BufferList *buffer, Status *status) {
  ssize_t nbytes;
  int ret;

  nbytes = 0;
  ret = 0;
  while (true) {
    if (buffer->Empty())  {
      // there is nothing in user-space stack to send
      break;
    }
    nbytes = ::write(fd, buffer->ReadPoint(), buffer->ReadableSize());

    if (nbytes > 0) {
      buffer->ReadAdvance(nbytes);
      ret += static_cast<int>(nbytes);
    } else if (nbytes < 0) {
      if (errno == EINTR) {
        continue;
      } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
        *status = TryAgain("write", errno);
        break;
      } else {
        *status = SysError("write", errno);
        return kError;
      }
    } else {
      // connection has been closed
      *status = SysError("write", errno);
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
