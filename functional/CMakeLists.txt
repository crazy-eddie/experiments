cmake_minimum_required(VERSION 2.6)

project(functional)

option(CI_BUILD "Build the system under a CI server." OFF)

set(CMAKE_CXX_FLAGS " -g -std=c++1y -Wall")


# CMake is sensitive to the name of the command.  It checks that the string 'valgrind' appears in the command.
# If not it will look for other common memory checkers.  Failure to find one in the name of the command results
# in CMake complaining that it doesn't understand the memory checker.  It never tries to run it first.
# Thus the name of the script MUST contain "valgrind".
if (CI_BUILD)
    set(MEMORYCHECK_COMMAND "${CMAKE_SOURCE_DIR}/cmake/valgrind.sh")
    set(MEMORYCHECK_COMMAND_OPTIONS "\"${CMAKE_BINARY_DIR}\"")
    
    #if (CMAKE_COMPILER_IS_GNUCC)
        
        SET(CMAKE_C_FLAGS "-g -O3 -Wall -fprofile-arcs -ftest-coverage")
        SET(CMAKE_CXX_FLAGS "-g -O3 -std=c++11 -Wall -fprofile-arcs -ftest-coverage")
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage -lgcov")
        
    #endif()
endif(CI_BUILD)

enable_testing()
include(CTest)

FIND_PACKAGE(Doxygen)
IF(DOXYGEN_FOUND)
  ADD_CUSTOM_TARGET(doxygen ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/doc/Doxyfile 
      COMMENT "Generating Doxygen API Documentation")
#  INSTALL(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
#      DESTINATION doc COMPONENT Documentation
#  )
ENDIF(DOXYGEN_FOUND)

#add_subdirectory(src)
add_subdirectory(test)