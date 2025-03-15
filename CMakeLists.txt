# Minimum version of CMake required
cmake_minimum_required(VERSION 3.30)

set(PROJECT_NAME "IMG2STL")

# Project name and language
project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

# Set debug mode
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Target generic x86
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=x86-64 -mtune=generic -O2")

file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add the executable by specifying all source files
add_executable(${PROJECT_NAME} ${SOURCES})

# Optionally set compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

