/*
 * Copyright (C) codedump
 */

#ifndef __SERVERKIT_BASE_LIKELY_H__
#define __SERVERKIT_BASE_LIKELY_H__

namespace serverkit {

#if defined __GNUC__
#define likely(x) __builtin_expect ((x), 1)
#define unlikely(x) __builtin_expect ((x), 0)
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif

};  // namespace serverkit

#endif  // __SERVERKIT_BASE_LIKELI_H__

