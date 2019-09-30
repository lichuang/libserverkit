/*
 * Copyright (C) codedump
 */

#include <string.h>
#include <gtest/gtest.h>
#include "base/buffer.h"

using namespace serverkit;

TEST(BufferTest, BufferTest) {
  Buffer buf;

  EXPECT_EQ(buf.Length(), 0);
  EXPECT_EQ(buf.Available(), kBufferSize);

  string str = "hello";
  buf.AppendString(str);
  EXPECT_EQ(buf.Length(), static_cast<int>(str.length()));
  EXPECT_EQ(buf.Available(), static_cast<int>(kBufferSize - str.length()));

  buf.Reset();
  EXPECT_EQ(buf.Length(), 0);
  EXPECT_EQ(buf.Available(), kBufferSize);  
}

TEST(BufferListTest, testBufferList) {
  BufferList buf;

  EXPECT_EQ(static_cast<int>(buf.ReadableSize()), 0);
  EXPECT_EQ(static_cast<int>(buf.WriteableSize()), kBufferSize);
  EXPECT_TRUE(buf.Empty());

  string str = "hello";
  buf.WriteString(str);
  EXPECT_EQ(buf.ReadableSize(), str.length());
  EXPECT_EQ(buf.WriteableSize(), kBufferSize - str.length());
  EXPECT_FALSE(buf.Empty());

  char data[10];
  size_t n = buf.Read(data, sizeof(data));
  data[n] = '\0';
  EXPECT_EQ(str, data);
}

TEST(BufferListTest, testLargeBuffer) {
  BufferList buf;
  char data[4 * kBufferSize];

  string tmp;
  string str = "hello";
  size_t n = 0;
  while (n < 3 * kBufferSize) {
    n += buf.Write(str.c_str(), str.length());
    tmp += str;
  }

  EXPECT_EQ(buf.WriteSize(), n);
  EXPECT_EQ(static_cast<int>(buf.ReadSize()), 0);
  EXPECT_EQ(buf.ReadableSize(), n);

  size_t read = buf.Read(data, sizeof(data));
  EXPECT_EQ(read, n);
  data[read] = '\0';
  EXPECT_EQ(tmp, data);
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}