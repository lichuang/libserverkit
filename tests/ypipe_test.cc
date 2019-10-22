/*
 * Copyright (C) codedump
 */

#include <string.h>
#include <gtest/gtest.h>
#include "base/ypipe.h"

using namespace serverkit;

TEST(YPIPETest, test_check_read_empty) {
    ypipe_t<int, 1> ypipe;
    ASSERT_FALSE(ypipe.CheckRead());
}

TEST(YPIPETest, test_read_empty) {
    ypipe_t<int, 1> ypipe;
    int read_value = -1;
    ASSERT_FALSE(ypipe.Read(&read_value));
    ASSERT_EQ(-1, read_value);
}

TEST(YPIPETest, test_write_complete_and_check_read_and_read) {
    const int value = 42;
    ypipe_t<int, 1> ypipe;
    ypipe.Write(value, false);
    ASSERT_FALSE(ypipe.CheckRead());
    int read_value = -1;
    ASSERT_FALSE(ypipe.Read(&read_value));
    ASSERT_EQ(-1, read_value);
}

TEST(YPIPETest, test_write_complete_and_flush_and_check_read_and_read) {
    const int value = 42;
    ypipe_t<int, 1> ypipe;
    ypipe.Write(value, false);
    ypipe.Flush();
    ASSERT_TRUE(ypipe.CheckRead());
    int read_value = -1;
    ASSERT_TRUE(ypipe.Read(&read_value));
    ASSERT_EQ(value, read_value);
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}