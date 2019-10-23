
add_executable ( atomic_test
  tests/atomic_test.cc
)

add_executable ( buffer_test
  tests/buffer_test.cc
)

add_executable ( thread_test
  tests/thread_test.cc
)

add_executable ( global_id_test
  tests/global_id_test.cc
)

add_executable ( ypipe_test
  tests/ypipe_test.cc
)

add_executable ( spin_lock_test
  tests/spin_lock_test.cc
)

target_link_libraries (atomic_test PRIVATE protobuf serverkit protobuf gtest pthread)
target_link_libraries (buffer_test PRIVATE protobuf serverkit protobuf gtest pthread)
target_link_libraries (thread_test PRIVATE protobuf serverkit protobuf gtest pthread)
target_link_libraries (global_id_test PRIVATE protobuf serverkit protobuf gtest pthread)
target_link_libraries (ypipe_test PRIVATE protobuf serverkit protobuf gtest pthread)
target_link_libraries (spin_lock_test PRIVATE protobuf serverkit protobuf gtest pthread)
