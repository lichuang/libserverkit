/*
 * Copyright (C) codedump
 */

#include <string.h>
#include <gtest/gtest.h>
#include "base/spin_lock.h"
#include "base/thread.h"

using namespace serverkit;

static const size_t kBufferSize = 16;

static SpinLock g_lock;

static void FillBuffer(volatile char* buffer, char fill_pattern) {
  for (size_t i = 0; i < kBufferSize; ++i)
    buffer[i] = fill_pattern;
}

static void ChangeAndCheckBuffer(volatile char* buffer) {
  FillBuffer(buffer, '\0');
  int total = 0;
  for (size_t i = 0; i < kBufferSize; ++i)
    total += buffer[i];

  EXPECT_EQ(0, total);

  // This will mess with the other thread's calculation if we accidentally get
  // concurrency.
  FillBuffer(buffer, '!');
}

static void testMain(volatile char* buffer) {
  for (int i = 0; i < 500000; ++i) {
    SpinLockGuard guard(g_lock);
    ChangeAndCheckBuffer(buffer);
  }
}

TEST(SpinLockTest, Torture) {
  char shared_buffer[kBufferSize];

  Thread thread1("thread1", bind(&testMain, static_cast<char*>(shared_buffer)));
  Thread thread2("thread2", bind(&testMain, static_cast<char*>(shared_buffer)));

  thread1.Start();
  thread2.Start();

  thread1.Join();
  thread2.Join();
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}