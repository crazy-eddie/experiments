cmake_minimum_required(VERSION 2.6)

project(metamap)

option(CI_BUILD "Build the system under a CI server." OFF)

set(CMAKE_CXX_FLAGS " -g -std=c++1y -Wall")

enable_testing()
include(CTest)

add_subdirectory(test)