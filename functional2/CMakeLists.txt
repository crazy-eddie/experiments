cmake_minimum_required(VERSION 2.6)

project(functional2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall")

set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

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