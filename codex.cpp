#include "codex.h"
#include <UnitTest++/UnitTest++.h>
#include <vector>
#include <iostream>

int main() {
  return UnitTest::RunAllTests();
}

TEST(sanity) {
  CHECK(1 + 1 == 2);
}

/*
TEST(uninit_vec) {
  std::vector<int> q;
  q.emplace_back(1);
  std::cout << q[0] << std::endl; 
}
*/
