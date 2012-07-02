#include "getopt.h"
#include "testfx.h"

template< int argc >
int count(char* (&argv)[argc])
{
  return argc;
}

option null_opt = {0};

TEST(test_getopt_long_empty)
{
  char* argv[] = {"foo.exe"};
  option opts[] = {null_opt};

  assert_equal(-1, (char)getopt_long(count(argv), argv, "abc", opts, NULL));
}

TEST(test_getopt_long_only_nonoptions)
{
  char* argv[] = {"foo.exe", "alfred"};
  option opts[] = {null_opt};

  assert_equal(-1, (char)getopt_long(count(argv), argv, "a", opts, NULL));
}

TEST(test_getopt_long_only_shortopts)
{
  char* argv[] = {"foo.exe", "-a", "-b"};
  option opts[] = {null_opt};

  assert_equal('a', (char)getopt_long(count(argv), argv, "ab", opts, NULL));
  assert_equal('b', (char)getopt_long(count(argv), argv, "ab", opts, NULL));
}

TEST(test_getopt_long_one_longopt)
{
  char* argv[] = {"foo.exe", "--first", "-a"};

  option opts[] = {
    {"first", no_argument, NULL, 'f'},
    null_opt
  };

  assert_equal('f', (char)getopt_long(count(argv), argv, "ab", opts, NULL));
  assert_equal('a', (char)getopt_long(count(argv), argv, "ab", opts, NULL));
}

TEST(test_getopt_long_two_longopts)
{
  char* argv[] = {"foo.exe", "--second", "--first"};

  option opts[] = {
    {"first", no_argument, NULL, 'f'},
    {"second", no_argument, NULL, 's'},
    null_opt
  };

  assert_equal('s', (char)getopt_long(count(argv), argv, "fs", opts, NULL));
  assert_equal('f', (char)getopt_long(count(argv), argv, "fs", opts, NULL));
}

TEST(test_getopt_long_abbreviated)
{
  char* argv[] = {"foo.exe", "--fir", "--fif"};

  option opts[] = {
    {"first", no_argument, NULL, '1'},
    {"fifth", no_argument, NULL, '5'},
    null_opt
  };

  assert_equal('1', (char)getopt_long(count(argv), argv, "15", opts, NULL));
  assert_equal('5', (char)getopt_long(count(argv), argv, "15", opts, NULL));
}

TEST(test_getopt_long_abbreviated_with_params)
{
  char* argv[] = {"foo.exe", "--fir=100", "--fif=500"};

  option opts[] = {
    {"first", required_argument, NULL, '1'},
    {"fifth", required_argument, NULL, '5'},
    null_opt
  };

  assert_equal('1', (char)getopt_long(count(argv), argv, "15", opts, NULL));
  assert_equal("100", optarg);

  assert_equal('5', (char)getopt_long(count(argv), argv, "15", opts, NULL));
  assert_equal("500", optarg);
}

TEST(test_getopt_long_ambiguous_abbrev)
{
  char* argv[] = {"foo.exe", "--fi", "--fi"};

  option opts[] = {
    {"first", no_argument, NULL, '1'},
    {"fifth", no_argument, NULL, '5'},
    null_opt
  };

  // Ambiguous match
  assert_equal('?', (char)getopt_long(count(argv), argv, "15", opts, NULL));
}

TEST(test_getopt_long_longindex)
{
  char* argv[] = {"foo.exe", "--second", "--first"};

  option opts[] = {
    {"first", no_argument, NULL, '1'},
    {"second", no_argument, NULL, '2'},
    null_opt
  };
  int longindex = -1;

  getopt_long(count(argv), argv, "", opts, &longindex);
  assert_equal(1, longindex);

  getopt_long(count(argv), argv, "", opts, &longindex);
  assert_equal(0, longindex);
}

TEST(test_getopt_long_flag)
{
  char* argv[] = {"foo.exe", "--second", "--first"};

  int flag = 0;

  option opts[] = {
    {"first", no_argument, &flag, '1'},
    {"second", no_argument, NULL, '2'},
    null_opt
  };

  assert_equal('2', getopt_long(count(argv), argv, "", opts, NULL));

  assert_equal(0, getopt_long(count(argv), argv, "", opts, NULL));
  assert_equal('1', flag);
}

TEST(test_getopt_long_unknown)
{
  char* argv[] = {"foo.exe", "--unknown"};

  option opts[] = {
    {"first", no_argument, NULL, '1'},
    {"second", no_argument, NULL, '2'},
    null_opt
  };

  assert_equal('?', getopt_long(count(argv), argv, "", opts, NULL));
}

TEST(test_getopt_long_required_argument_same_argv)
{
  char* argv[] = {"foo.exe", "--arg=param"};

  option opts[] = {
    {"arg", required_argument, NULL, 'a'},
    null_opt
  };

  assert_equal('a', getopt_long(count(argv), argv, "", opts, NULL));
  assert_equal("param", optarg);
}

TEST(test_getopt_long_required_argument_separate_argv)
{
  char* argv[] = {"foo.exe", "--arg", "value"};

  option opts[] = {
    {"arg", required_argument, NULL, 'a'},
    null_opt
  };

  assert_equal('a', getopt_long(count(argv), argv, "", opts, NULL));
  assert_equal("value", optarg);
}

TEST(test_getopt_long_missing_required_argument)
{
  char* argv[] = {"foo.exe", "--arg"};

  option opts[] = {
    {"arg", required_argument, NULL, 'a'},
    null_opt
  };

  assert_equal(':', getopt_long(count(argv), argv, "", opts, NULL));
  assert_equal(NULL, (int)optarg);
}

TEST(test_getopt_long_optional_argument_same_argv)
{
  char* argv[] = {"foo.exe", "--arg=param"};

  option opts[] = {
    {"arg", optional_argument, NULL, 'a'},
    null_opt
  };

  assert_equal('a', getopt_long(count(argv), argv, "", opts, NULL));
  assert_equal("param", optarg);
}

TEST(test_getopt_long_optional_argument_separate_argv)
{
  char* argv[] = {"foo.exe", "--arg", "value"};

  option opts[] = {
    {"arg", optional_argument, NULL, 'a'},
    null_opt
  };

  assert_equal('a', getopt_long(count(argv), argv, "", opts, NULL));
  assert_equal("value", optarg);
}

TEST(test_getopt_long_missing_optional_argument)
{
  char* argv[] = {"foo.exe", "--arg"};

  option opts[] = {
    {"arg", optional_argument, NULL, 'a'},
    null_opt
  };

  assert_equal('a', getopt_long(count(argv), argv, "", opts, NULL));
  assert_equal(NULL, (int)optarg);
}

TEST(test_getopt_long_empty_required_argument)
{
  char* argv[] = {"foo.exe", "--arg="};

  option opts[] = {
    {"arg", required_argument, NULL, 'a'},
    null_opt
  };

  assert_equal('a', getopt_long(count(argv), argv, "", opts, NULL));
  assert_equal("", optarg);
}

TEST(test_getopt_long_empty_optional_argument)
{
  char* argv[] = {"foo.exe", "--arg="};

  option opts[] = {
    {"arg", optional_argument, NULL, 'a'},
    null_opt
  };

  assert_equal('a', getopt_long(count(argv), argv, "", opts, NULL));
  assert_equal("", optarg);
}