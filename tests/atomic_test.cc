/*
 * Copyright (C) codedump
 */

#include <string.h>
#include <gtest/gtest.h>
#include "base/thread.h"
#include "serverkit.h"

using namespace serverkit;

struct TestItem {
  int v1;
  int v2;
};

TEST(AtomicTest, TestXchg) {
  TestItem t, t1;
  t.v1 = 1;
  t.v2 = 2;
  t1.v1 = 3;
  t1.v2 = 4;

  AtomicPointer<TestItem> at;
  at.Set(&t);

  TestItem *t2 = at.Xchg(&t1);

  ASSERT_EQ(t2, &t);
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}