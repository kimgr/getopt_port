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

#ifndef INCLUDED_TESTFX_H
#define INCLUDED_TESTFX_H

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <stdexcept>
#include <stdio.h>
#include <string.h>
#include <sstream>

// Test registration
typedef void (*testfunction)();

void register_test(const char* name, testfunction f);
int run_tests();

struct test_registrar {
  test_registrar(const char* name, testfunction f) {
    register_test(name, f);
  }
};

#define PASTE(prefix, unique) prefix ## unique
#define MAKE_NAME(prefix, unique) PASTE(prefix, unique)

#define TEST(func) \
  static void func(); \
  test_registrar MAKE_NAME(MAKE_NAME(r, func), __LINE__) (#func, func); \
  \
  static void func()

#define TEST_F(fixture, func) \
  static void func##bootstrap(); \
  static void func(fixture& f); \
  test_registrar MAKE_NAME(MAKE_NAME(r, func), __LINE__) (#func, func##bootstrap); \
  \
  static void func##bootstrap() { \
    fixture f; \
    func(f); \
  } \
  static void func(fixture& f) \

// Assertions
struct assertion_failure : std::runtime_error {
  explicit assertion_failure(const char* message)
    : std::runtime_error(message) {
  }
};

#define assert_equal(expected, actual) \
  check_equal(expected, actual, __FUNCTION__, #expected, #actual)

static std::string printable(const char* s) {
  if (s == NULL)
    return "NULL";

  return "\"" + std::string(s) + "\"";
}

static std::string printable(char c) {
  if (isprint(c)) {
    return "\'" + std::string(1, c) + "\'";
  } else {
    char buf[64] = {0};
    sprintf(buf, "%d", c);
    return std::string(buf);
  }
}

template< class T1, class T2 >
static std::string build_message(const char* function, const char* expected_expr, const char* actual_expr, const T1& expected, const T2& actual) {
  std::ostringstream builder;
  builder << function << ": assertion (" << expected_expr << " == " << actual_expr << ") failed -- expected " << expected << ", was " << actual;

  return builder.str();
}

static std::string build_message(const char* function, const char* expected_expr, const char* actual_expr, const char* expected, const char* actual) {
  return build_message(function, expected_expr, actual_expr, printable(expected), printable(actual));
}

static std::string build_message(const char* function, const char* expected_expr, const char* actual_expr, int expected, int actual) {
  return build_message(function, expected_expr, actual_expr, printable(expected), printable(actual));
}

static std::string build_message(const char* function, const char* expected_expr, const char* actual_expr, char expected, int actual) {
  return build_message(function, expected_expr, actual_expr, printable(expected), printable(actual));
}

static std::string build_message(const char* function, const char* expected_expr, const char* actual_expr, char expected, char actual) {
  return build_message(function, expected_expr, actual_expr, printable(expected), printable(actual));
}

template< class T1, class T2 >
static void check_equal(const T1& expected, const T2& actual, const char* function, const char* expected_expr, const char* actual_expr) {
  if (expected != actual) {
    throw assertion_failure(build_message(function, expected_expr, actual_expr, expected, actual).c_str());
  }
}

static void check_equal(const char* expected, const char* actual, const char* function, const char* expected_expr, const char* actual_expr) {
  if (expected == actual)
    return;

  if (expected == NULL || actual == NULL) {
    throw assertion_failure(build_message(function, expected_expr, actual_expr, expected, actual).c_str());
  }

  if (strcmp(expected, actual)) {
    throw assertion_failure(build_message(function, expected_expr, actual_expr, expected, actual).c_str());
  }
}

static void check_equal(const char* expected, char* actual, const char* function, const char* expected_expr, const char* actual_expr) {
  check_equal(expected, (const char*)actual, function, expected_expr, actual_expr);
}

static void check_equal(char* expected, char* actual, const char* function, const char* expected_expr, const char* actual_expr) {
  check_equal((const char*)expected, (const char*)actual, function, expected_expr, actual_expr);
}

#endif // INCLUDED_TESTFX_H
