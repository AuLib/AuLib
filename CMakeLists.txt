# CMake build script
# for the AuLib project

cmake_minimum_required(VERSION 3.0.1)

project(AuLibProject)
set(AuLib_VERSION_MAJOR 0)
set(AuLib_VERSION_MINOR 0)

include_directories("${PROJECT_SOURCE_DIR}/src")

# AuLib sources
add_subdirectory(src)

# Example programs
add_subdirectory(examples)





