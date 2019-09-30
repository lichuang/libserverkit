
add_executable ( buffer_test
  tests/buffer_test.cc
)

add_executable ( thread_test
  tests/thread_test.cc
)

target_link_libraries (buffer_test PRIVATE protobuf serverkit protobuf gtest pthread)
target_link_libraries (thread_test PRIVATE protobuf serverkit protobuf gtest pthread)
