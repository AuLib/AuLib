# CMake build script
# for the AuLib project
cmake_minimum_required(VERSION 3.0.1)

# set CMAKE_BUILD_TYPE (DEBUG or RELEASE)
set(CMAKE_BUILD_TYPE "DEBUG")

# Project target and version 
project(AuLibProject)
set(AuLib_MAJOR_V 1)
set(AuLib_MINOR_V 0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
-std=c++14 -DAULIB_MAJOR_V=${AuLib_MAJOR_V} \
-DAULIB_MINOR_V=${AuLib_MINOR_V}")

if(CMAKE_BUILD_TYPE MATCHES DEBUG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBETA=1 \
  -Wall -Werror -Wno-unused-function -Wno-overloaded-virtual")
else()
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBETA=0")
ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG)

include_directories("${PROJECT_SOURCE_DIR}/src")

# AuLib sources
add_subdirectory(src)

# Example programs
add_subdirectory(examples)

# Documentation 
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.doxygen
          ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc ${DOXYGEN_EXECUTABLE}
            ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
     WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
endif(DOXYGEN_FOUND)





