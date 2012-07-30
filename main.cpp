#include "testfx.h"
#include "getopt.h"

int main(int argc, char* argv[]) {
  for (test_vector::const_iterator i = tests().begin(); i != tests().end(); ++i) {
    // TODO(kimgr): introduce some fixture mechanism, so we can do this in tests.
    optind = 1; // HACK: Reset optind before every test, so that getopt() runs "isolated"
    std::cout <<  "Running " << i->first << "..." << std::endl;
    i->second();
  }

  return 0;
}
