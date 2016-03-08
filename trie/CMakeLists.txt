cmake_minimum_required(VERSION 2.6)

project(trie)

option(CI_BUILD "Build the system under a CI server." OFF)

set(CMAKE_CXX_FLAGS " -g -std=c++1y -Wall -Werror -pedantic -pedantic-errors")

enable_testing()
include(CTest)


find_package(Boost)


add_subdirectory(test)
#add_subdirectory(src)