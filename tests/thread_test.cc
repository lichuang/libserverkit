/*
 * Copyright (C) codedump
 */

#include <string.h>
#include <gtest/gtest.h>
#include "base/thread.h"
#include "serverkit.h"

using namespace serverkit;

class TestThread : public Thread {
public:
  TestThread(int* cnt)
    : Thread("test") {
    cnt_ = cnt;
  }
  virtual ~TestThread() {

  }
  
  void Run() {
    while (*cnt_ > 0) {
      *cnt_ = *cnt_ - 1;
    }
  }

private:
  int *cnt_;
};

TEST(ThreadTest, test1) {
  int cnt = 10;

  TestThread thread(&cnt);

  thread.Start();
  thread.Join();

  ASSERT_EQ(cnt, 0);
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    serverkit::ServerkitInit(argc, argv, ServerkitOption());

    return RUN_ALL_TESTS();
}