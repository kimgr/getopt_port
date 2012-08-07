#include "testfx.h"
#include <vector>

typedef std::vector<std::pair< const char*, testfunction> > test_vector;

static test_vector& tests() {
  static test_vector tests;
  return tests;
}

void register_test(const char* name, testfunction f) {
  tests().push_back(std::make_pair(name, f));
}

int run_tests() {
  int failures = 0;
  for (test_vector::const_iterator i = tests().begin(); i != tests().end(); ++i) {
    try {
      std::cout <<  "Running " << i->first << "..." << std::endl;
      i->second();
    }
    catch (const assertion_failure& failure) {
      std::cout << failure.what() << std::endl;
      ++failures;
    }
  }

  return failures;
}