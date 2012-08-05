#include "getopt.h"
#include "testfx.h"
#include "testsupport.h"

TEST_F(getopt_fixture, test_getopt_empty) {
  char* argv[] = {"foo.exe"};
  assert_equal(-1, getopt(count(argv), argv, "abc"));
}

TEST_F(getopt_fixture, test_getopt_simple) {
  char* argv[] = {"foo.exe", "-a"};
  assert_equal('a', getopt(count(argv), argv, "a"));
}

TEST_F(getopt_fixture, test_getopt_unrecognized) {
  char* argv[] = {"foo.exe", "-a"};
  assert_equal('?', getopt(count(argv), argv, "cde"));
}

TEST_F(getopt_fixture, test_getopt_only_nonoptions) {
  char* argv[] = {"foo.exe", "nonoption"};
  assert_equal(-1, getopt(count(argv), argv, "a"));
}

TEST_F(getopt_fixture, test_getopt_consume_subset) {
  char* argv[] = {"foo.exe", "-f", "-g", "-h"};
  assert_equal('f', getopt(count(argv), argv, "fgh"));
  assert_equal('g', getopt(count(argv), argv, "fgh"));
}

TEST_F(getopt_fixture, test_getopt_multiple_options_separate_argv) {
  char* argv[] = {"foo.exe", "-a", "-b"};
  assert_equal('a', getopt(count(argv), argv, "ab"));
  assert_equal('b', getopt(count(argv), argv, "ab"));
}

TEST_F(getopt_fixture, test_getopt_multiple_options_same_argv) {
  char* argv[] = {"foo.exe", "-ba"};
  assert_equal('b', getopt(count(argv), argv, "ab"));
  assert_equal('a', getopt(count(argv), argv, "ab"));
}

TEST_F(getopt_fixture, test_getopt_embedded_nonoptions) {
  char* argv[] = {"foo.exe", "-a", "nonoption1", "nonoption2", "-b"};
  assert_equal('a', getopt(count(argv), argv, "ab"));

  // The non-option argument "nonoption1" terminates the scan
  assert_equal(-1, getopt(count(argv), argv, "ab"));
}

TEST_F(getopt_fixture, test_getopt_argument_same_argv) {
  char* argv[] = {"foo.exe", "-aargument"};

  assert_equal('a', getopt(count(argv), argv, "a:"));
  assert_equal("argument", optarg);
}

TEST_F(getopt_fixture, test_getopt_argument_separate_argv) {
  char* argv[] = {"foo.exe", "-a", "argument"};

  assert_equal('a', getopt(count(argv), argv, "a:"));
  assert_equal("argument", optarg);
}

TEST_F(getopt_fixture, test_getopt_missing_required_argument) {
  char* argv[] = {"foo.exe", "-a"};

  assert_equal('?', getopt(count(argv), argv, "a:"));
  assert_equal('a', optopt);
  assert_equal((char*)NULL, optarg);
  assert_equal(3, optind);
}

TEST_F(getopt_fixture, test_getopt_missing_required_argument_colon) {
  char* argv[] = {"foo.exe", "-a"};

  assert_equal(':', getopt(count(argv), argv, ":a:"));
  assert_equal('a', optopt);
  assert_equal((char*)NULL, optarg);
  assert_equal(3, optind);
}

TEST_F(getopt_fixture, test_getopt_optional_argument) {
  char* argv[] = {"foo.exe", "-aargument"};

  assert_equal('a', getopt(count(argv), argv, "a::"));
  assert_equal("argument", optarg);
}

TEST_F(getopt_fixture, test_getopt_missing_optional_argument) {
  char* argv[] = {"foo.exe", "-a"};

  assert_equal('a', getopt(count(argv), argv, "a::"));
  assert_equal((char*)NULL, optarg);
}

TEST_F(getopt_fixture, test_getopt_optopt) {
  char* argv[] = {"foo.exe", "-a", "-b"};

  getopt(count(argv), argv, "a");
  assert_equal('a', optopt);

  assert_equal('?', getopt(count(argv), argv, "a"));
  assert_equal('b', optopt);
}
