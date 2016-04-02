cmake_minimum_required(VERSION 2.6)

project(expression-templates)

set(CMAKE_CXX_FLAGS " -g -std=c++1z -Wall -Werror -pedantic -pedantic-errors")


enable_testing()

find_package(Boost)
find_package(Boost COMPONENTS unit_test_framework)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})
add_custom_target(tests)

include_directories(${Boost_INCLUDE_DIR})

function(add_boost_test name)
  add_executable(${name} EXCLUDE_FROM_ALL ${ARGN})

  target_link_libraries(${name} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
  
  add_test(${name} ./${name})
  
  add_dependencies(check ${name})
  add_dependencies(tests ${name})
endfunction()

add_boost_test(const_array const_array.cpp)
add_boost_test(indexed_storage indexed_storage.cpp)
add_boost_test(invocation_list invocation_list.cpp)