#ifndef INCLUDED_TESTSUPPORT_H
#define INCLUDED_TESTSUPPORT_H

template< int argc >
static int count(char* (&argv)[argc]) {
  return argc;
}

struct getopt_fixture {
  getopt_fixture() {
    // Reset optind before every test, so that getopt() runs "isolated"
    ::optind = 1;
  }
};

#endif // INCLUDED_TESTSUPPORT_H