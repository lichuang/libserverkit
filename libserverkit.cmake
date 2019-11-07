set(libserverkit_files
  src/serverkit.cc
  src/base/buffer.cc
  src/base/condition.cc  
  src/base/file.cc
  src/base/hash.cc
  src/base/lock.cc
  src/base/log.cc
  src/base/log_thread.cc
  src/base/net.cc
  src/base/string.cc
  src/base/thread.cc
  src/base/spin_lock.cc
  src/core/application.cc
  src/core/epoll.cc
  src/core/listener.cc
  src/core/mailbox.cc
  src/core/poller.cc
  src/core/signaler.cc
  src/core/socket.cc
  src/core/timer_thread.cc
  src/core/worker.cc
  src/rpc/packet_parser.cc  
  src/rpc/rpc_channel.cc
  src/rpc/rpc_session.cc
  src/rpc/rpc_service.cc
  src/util/lookup8.cc
  src/util/global_id.cc
  src/util/misc.cc
  src/util/system.cc
  src/util/time.cc
)

add_library(serverkit 
  ${serverkit_SHARED_OR_STATIC}
  ${libserverkit_files}
)
