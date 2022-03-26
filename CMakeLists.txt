cmake_minimum_required(VERSION 3.10)

if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif ()

# set the project name
project(linuxshell)
include_directories(include)
file(GLOB SOURCES "src/*.cpp")
add_executable(linuxshell ${SOURCES})
target_compile_features(linuxshell PRIVATE cxx_range_for)

# specify the C++ standard
# set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED True)