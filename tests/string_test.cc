/*
 * Copyright (C) codedump
 */

#include <string.h>
#include <gtest/gtest.h>
#include "util/string.h"

using namespace serverkit;

TEST(StringPrintfTest, StringPrintfEmpty) {
  EXPECT_EQ("", StringPrintf("%s", ""));
}