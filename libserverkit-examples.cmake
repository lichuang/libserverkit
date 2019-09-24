add_executable ( echo_server
  example/echo/server.cc
)

add_executable ( rpc_echo_server
  example/rpc_echo/echo.pb.cc
  example/rpc_echo/server.cc
)

add_executable ( rpc_echo_client
  example/rpc_echo/echo.pb.cc
  example/rpc_echo/client.cc
)

target_link_libraries (echo_server PRIVATE protobuf serverkit protobuf pthread)
target_link_libraries (rpc_echo_server PRIVATE serverkit protobuf pthread)
target_link_libraries (rpc_echo_client PRIVATE serverkit protobuf pthread)