# CMake build script
# for the AuLib project

cmake_minimum_required(VERSION 3.0.1)

project(AuLibProject)
set(AuLib_VERSION_MAJOR 0)
set(AuLib_VERSION_MINOR 0)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

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





