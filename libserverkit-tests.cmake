set(libserverkit_test_files
  tests/buffer_test.cc
  tests/main.cc
)

add_executable ( serverkit_test
  ${libserverkit_test_files}
)

target_link_libraries (serverkit_test PRIVATE protobuf serverkit protobuf gtest pthread)