/*
 * Copyright (C) codedump
 */

#include <iostream>
#include <sstream>
#include <vector>
#include "base/hash.h"
#include "base/string.h"

namespace serverkit {

string 
Stringf(const char *fmt, ...) {
  // initialize use of the variable argument array
  va_list args;
  va_start(args, fmt);

  // reliably acquire the size from a copy of
  // the variable argument array
  // and a functionally reliable call
  // to mock the formatting
  va_list copy;
  va_copy(copy, args);
  const int iLen = std::vsnprintf(NULL, 0, fmt, copy);
  va_end(copy);

  // return a formatted string without
  // risking memory mismanagement
  // and without assuming any compiler
  // or platform specific behavior
  std::vector<char> zc(iLen + 1);
  std::vsnprintf(zc.data(), zc.size(), fmt, args);
  va_end(args);

  return std::string(zc.data(), zc.size());
}

uint64_t 
HashString(const string& str) {
  return HashBytes(str.c_str(), static_cast<int>(str.size()));
}

string 
StringToHex(const string& str) {
  string hex = "0x";
  for (size_t i = 0; i < str.size(); ++i) {
    hex += Stringf("%x", static_cast<uint8_t>(str[i]));
  }
  return hex;
}

};  // namespace serverkit
