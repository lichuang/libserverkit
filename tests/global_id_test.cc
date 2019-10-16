/*
 * Copyright (C) codedump
 */

#include <stdint.h>
#include <string.h>
#include <map>
#include <vector>
#include <gtest/gtest.h>
#include "base/thread.h"
#include "util/global_id.h"

using namespace std;
using namespace serverkit;

class StressThread : public Thread {
public:
  StressThread(vector<uint64_t>* g, int n)
    : Thread("stress") {
    global_ids = g;
    num = n;
  }

  virtual ~StressThread() {

  }

protected:
  virtual void Run() {
    int i = num;
    for (; i > 0; --i) {
      global_ids->push_back(NewGlobalID());
    }
  }

public:
  vector<uint64_t>* global_ids;
  int num;
};

TEST(TestGlobalId, threadTestGlobalId) {
  int numThread = 20;
  int idPerThread = 50000;
  int i;

  vector< vector<uint64_t> > generated_ids;
  vector<StressThread*> threads;

  generated_ids.resize(numThread);
  threads.resize(numThread);

  for (i = 0; i < numThread; ++i) {
    StressThread* thread = new StressThread(&generated_ids[i], idPerThread);

    thread->Start();

    threads[i] = thread;
  }

  map<uint64_t, bool> all_ids;
  for (i = 0; i < numThread; ++i) {
    StressThread* thread = threads[i];
    thread->Join();
    delete thread;

    vector<uint64_t>* ids = &generated_ids[i];
    size_t n;
    for (n = 0; n < ids->size(); ++n) {
      uint64_t c = (*ids)[n];
      EXPECT_EQ(all_ids.find(c), all_ids.end());
      all_ids[c] = true;
    }

    all_ids.clear();
  }
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}