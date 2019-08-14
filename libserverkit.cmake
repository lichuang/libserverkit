set(libserverkit_files
  src/base/buffer.cc
  src/base/condition.cc
  src/base/error.cc
  src/base/file.cc
  src/base/lock.cc
  src/base/log.cc
  src/base/log_thread.cc
  src/base/net.cc
  src/base/status.cc
  src/base/string.cc
  src/base/thread.cc
  src/base/time.cc
  src/core/epoll.cc
  src/core/io_thread.cc
  src/core/listener.cc
  src/core/mailbox.cc
  src/core/poller.cc
  src/core/server.cc
  src/core/signaler.cc
  src/core/socket.cc
)

add_library(serverkit ${serverkit_SHARED_OR_STATIC}
  ${libserverkit_files})
