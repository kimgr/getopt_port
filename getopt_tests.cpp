/*******************************************************************************
 * Copyright (c) 2012-2023, Kim Grasman <kim.grasman@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Kim Grasman nor the
 *     names of contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL KIM GRASMAN BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

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
  assert_equal(3, optind);  // optind should be incremented past argc looking
                            // for args
}

TEST_F(getopt_fixture, test_getopt_missing_required_argument_colon) {
  char* argv[] = {"foo.exe", "-a"};

  assert_equal(':', getopt(count(argv), argv, ":a:"));
  assert_equal('a', optopt);
  assert_equal((char*)NULL, optarg);
  assert_equal(3, optind);  // optind should be incremented past argc looking
                            // for args
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

  assert_equal('a', getopt(count(argv), argv, "a"));
  assert_equal('a', optopt);

  assert_equal('?', getopt(count(argv), argv, "a"));
  assert_equal('b', optopt);
}
