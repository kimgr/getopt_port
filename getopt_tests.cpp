#include "getopt.h"
#include "testfx.h"

template< int argc >
int count(char* (&argv)[argc]) {
  return argc;
}

TEST(test_getopt_empty) {
  char* argv[] = {"foo.exe"};
  assert_equal(-1, (char)getopt(count(argv), argv, "abc"));
}

TEST(test_getopt_simple) {
  char* argv[] = {"foo.exe", "-a"};
  assert_equal('a', (char)getopt(count(argv), argv, "a"));
}

TEST(test_getopt_unrecognized) {
  char* argv[] = {"foo.exe", "-a"};
  assert_equal('?', (char)getopt(count(argv), argv, "cde"));
}

TEST(test_getopt_only_nonoptions) {
  char* argv[] = {"foo.exe", "alfred"};
  assert_equal(-1, (char)getopt(count(argv), argv, "a"));
}

TEST(test_getopt_consume_subset) {
  char* argv[] = {"foo.exe", "-f", "-g", "-h"};
  assert_equal('f', (char)getopt(count(argv), argv, "fgh"));
  assert_equal('g', (char)getopt(count(argv), argv, "fgh"));
}

TEST(test_getopt_multiple_options_separate_argv) {
  char* argv[] = {"foo.exe", "-a", "-b"};
  assert_equal('a', (char)getopt(count(argv), argv, "ab"));
  assert_equal('b', (char)getopt(count(argv), argv, "ab"));
}

TEST(test_getopt_multiple_options_same_argv) {
  char* argv[] = {"foo.exe", "-ba"};
  assert_equal('b', (char)getopt(count(argv), argv, "ab"));
  assert_equal('a', (char)getopt(count(argv), argv, "ab"));
}

TEST(test_getopt_embedded_nonoptions) {
  char* argv[] = {"foo.exe", "-a", "filename", "bobbycar", "-b"};
  assert_equal('a', (char)getopt(count(argv), argv, "ab"));

  // The non-option argument "filename" terminates the scan
  assert_equal(-1, (char)getopt(count(argv), argv, "ab"));
}

TEST(test_getopt_argument_same_argv) {
  char* argv[] = {"foo.exe", "-afilename"};

  assert_equal('a', (char)getopt(count(argv), argv, "a:"));
  assert_equal("filename", optarg);
}

TEST(test_getopt_argument_separate_argv) {
  char* argv[] = {"foo.exe", "-a", "filename"};

  assert_equal('a', (char)getopt(count(argv), argv, "a:"));
  assert_equal("filename", optarg);
}

TEST(test_getopt_optional_argument) {
  char* argv[] = {"foo.exe", "-afilename"};

  assert_equal('a', (char)getopt(count(argv), argv, "a::"));
  assert_equal("filename", optarg);
}

TEST(test_getopt_missing_optional_argument) {
  char* argv[] = {"foo.exe", "-a"};

  assert_equal('a', (char)getopt(count(argv), argv, "a::"));
  assert_equal((char*)NULL, optarg);
}
