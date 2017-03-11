# CMake build script
# for the AuLib project
cmake_minimum_required(VERSION 3.0.1)

# set CMAKE_BUILD_TYPE (DEBUG or RELEASE)
set(CMAKE_BUILD_TYPE "DEBUG")
set(CMAKE_VERBOSE_MAKEFILE  ON)

# Project target and version 
project(AuLibProject)
set(AuLib_MAJOR_V 1)
set(AuLib_MINOR_V 0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
-std=c++14  -DAULIB_MAJOR_V=${AuLib_MAJOR_V} \
-DAULIB_MINOR_V=${AuLib_MINOR_V} ")

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()


if(CMAKE_BUILD_TYPE MATCHES DEBUG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBETA=1 \
  -Wall -Werror ")
else()
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBETA=0")
ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG)

include_directories("/usr/local/include")
include_directories("${PROJECT_SOURCE_DIR}/src")

# find the dependencies
if(APPLE)
 message(STATUS "platform is OSX")
 find_library(LIBSNDFILE libsndfile.dylib)
 find_library(PORTAUDIO libportaudio.dylib)
 find_library(PORTMIDI libportmidi.dylib)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
 message(STATUS "platform is linux")
 find_library(LIBSNDFILE libsndfile.so)
 find_library(PORTAUDIO libportaudio.so)
 find_library(PORTMIDI libportmidi.so)
 find_library(PORTIME libporttime.so)
else()
 message(STATUS "platform is undefined")
endif(APPLE)

if(PORTAUDIO)
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_PORTAUDIO ")
else()
 message(STATUS "portaudio not found, not building realtime audio IO support")
endif(PORTAUDIO)

if(LIBSNDFILE)
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_LIBSNDFILE ")
else()
 message(STATUS "libsndfile not found, not building soundfile IO support")
endif(LIBSNDFILE)

if(PORTMIDI)
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_PORTMIDI ")
else()
 message(STATUS "Portmidi not found, not building MIDI input support")
endif(PORTMIDI)

find_package (Threads)

find_program(CLANG_FORMAT clang-format)

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
 else()
   message(STATUS "Not building documentation")
endif(DOXYGEN_FOUND)





