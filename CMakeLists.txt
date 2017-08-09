cmake_minimum_required(VERSION 3.0)

project(getopt_port)

add_executable(test_getopt_port
  getopt.c
  getopt_tests.cpp
  getopt_long_tests.cpp
  main.cpp
  testfx.cpp
)

add_executable(test_getopt_port_c
  getopt.c
  main.c
)

# Have the tests accept const char* -> char* decay-
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  target_compile_options(test_getopt_port
    PRIVATE
    "-Wno-c++11-compat-deprecated-writable-strings")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  target_compile_options(test_getopt_port
    PRIVATE
    "-Wno-write-strings")
endif()
