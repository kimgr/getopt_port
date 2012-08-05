#include "testfx.h"
#include <vector>

typedef std::vector<std::pair< const char*, testfunction> > test_vector;

const test_vector& tests();

static test_vector& mutable_tests() {
  static test_vector tests;
  return tests;
}

const test_vector& tests() {
  return mutable_tests();
}

void register_test(const char* name, testfunction f) {
  mutable_tests().push_back(std::make_pair(name, f));
}

int run_tests() {
  for (test_vector::const_iterator i = tests().begin(); i != tests().end(); ++i) {
    std::cout <<  "Running " << i->first << "..." << std::endl;
    i->second();
  }

  return 0;
}