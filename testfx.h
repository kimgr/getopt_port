#ifndef INCLUDED_TESTFX_H
#define INCLUDED_TESTFX_H

#include <iostream>
#include <vector>

typedef void (*testfunction)();

typedef std::vector<std::pair< const char*, testfunction> > test_vector;

const test_vector& tests();
void register_test(const char* name, testfunction f);

struct test_registrar
{
  test_registrar(const char* name, testfunction f)
  {
    register_test(name, f);
  }
};

#define PASTE(prefix, unique) prefix ## unique
#define MAKE_NAME(prefix, unique) PASTE(prefix, unique)

#define TEST(f) \
  void f(); \
  test_registrar MAKE_NAME(r, __LINE__) (#f, f); \
  void f()

#define assert_equal(expected, actual) \
  check_equal(expected, actual, __FUNCTION__, #expected, #actual)

template< class T1, class T2 >
static void check_equal(const T1& expected, const T2& actual, const char* function, const char* expected_expr, const char* actual_expr)
{
  if (expected != actual)
  {
    std::cout << function << ": assertion (" << expected_expr << " == " << actual_expr << ") failed -- expected " << expected << ", was " << actual << std::endl;
    exit(1);
  }
}

static void check_equal(const char* expected, const char* actual, const char* function, const char* expected_expr, const char* actual_expr)
{
  if (expected == actual)
    return;

  if (expected == NULL || actual == NULL)
  {
    std::cout << function << ": assertion (" << expected_expr << " == " << actual_expr << ") failed -- expected " << expected << ", was " << actual << std::endl;
    exit(1);
  }

  if (strcmp(expected, actual))
  {
    std::cout << function << ": assertion (" << expected_expr << " == " << actual_expr << ") failed -- expected " << expected << ", was " << actual << std::endl;
    exit(1);
  }
}

static void check_equal(const char* expected, char* actual, const char* function, const char* expected_expr, const char* actual_expr)
{
  check_equal(expected, (const char*)actual, function, expected_expr, actual_expr);
}

#endif 
