#ifndef INCLUDED_TESTFX_H
#define INCLUDED_TESTFX_H

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <stdexcept>
#include <stdio.h>
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
