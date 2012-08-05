#ifndef INCLUDED_TESTFX_H
#define INCLUDED_TESTFX_H

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>

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
#define assert_equal(expected, actual) \
  check_equal(expected, actual, __FUNCTION__, #expected, #actual)

template< class T1, class T2 >
static void check_equal(const T1& expected, const T2& actual, const char* function, const char* expected_expr, const char* actual_expr) {
  if (expected != actual) {
    std::cout << function << ": assertion (" << expected_expr << " == " << actual_expr << ") failed -- expected " << expected << ", was " << actual << std::endl;
    exit(1);
  }
}

static std::string format_char(char c) {
  if (isprint(c)) {
    return std::string(1, c);
  } else {
    char buf[64] = {0};
    return std::string(_itoa(c, buf, 10));
  }
}

static void check_equal(char expected, char actual, const char* function, const char* expected_expr, const char* actual_expr) {
  if (expected != actual) {
    std::cout << function << ": assertion (" << expected_expr << " == " << actual_expr << ") failed -- expected " << format_char(expected) << ", was " << format_char(actual) << std::endl;
    exit(1);
  }
}

static void check_equal(char expected, int actual, const char* function, const char* expected_expr, const char* actual_expr) {
  if (expected != actual) {
    std::cout << function << ": assertion (" << expected_expr << " == " << actual_expr << ") failed -- expected " << format_char(expected) << ", was " << format_char(actual) << std::endl;
    exit(1);
  }
}

static void check_equal(const char* expected, const char* actual, const char* function, const char* expected_expr, const char* actual_expr) {
  if (expected == actual)
    return;

  if (expected == NULL || actual == NULL) {
    std::cout << function << ": assertion (" << expected_expr << " == " << actual_expr << ") failed -- expected " << expected << ", was " << actual << std::endl;
    exit(1);
  }

  if (strcmp(expected, actual)) {
    std::cout << function << ": assertion (" << expected_expr << " == " << actual_expr << ") failed -- expected " << expected << ", was " << actual << std::endl;
    exit(1);
  }
}

static void check_equal(const char* expected, char* actual, const char* function, const char* expected_expr, const char* actual_expr) {
  check_equal(expected, (const char*)actual, function, expected_expr, actual_expr);
}

#endif // INCLUDED_TESTFX_H
