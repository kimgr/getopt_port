#include "testfx.h"

static test_vector& mutable_tests()
{
  static test_vector tests;
  return tests;
}

const test_vector& tests()
{
  return mutable_tests();
}

void register_test(const char* name, testfunction f)
{
  mutable_tests().push_back(std::make_pair(name, f));
}