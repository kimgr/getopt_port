cmake_minimum_required(VERSION 3.0)

project(getopt_port)

add_executable(test_getopt_port
  getopt.c
  getopt_tests.cpp
  getopt_long_tests.cpp
  main.cpp
  testfx.cpp
)
