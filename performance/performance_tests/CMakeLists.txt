

find_package(Boost)
find_package(Boost COMPONENTS unit_test_framework timer system)

add_custom_target(perf)

function(add_perf_test name)
  add_executable(${name} EXCLUDE_FROM_ALL ${ARGN})
  
  target_link_libraries(${name} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                                ${Boost_TIMER_LIBRARY}
                                ${Boost_SYSTEM_LIBRARY})
  
  add_dependencies(perf ${name})
endfunction()

add_perf_test(pimpl_vector pimpl_vector.cpp ../unit_tests/test_pimpl.cpp)