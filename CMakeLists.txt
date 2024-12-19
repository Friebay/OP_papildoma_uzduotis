cmake_minimum_required(VERSION 3.10)

# Project name and version
project(OP_projektas VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure consistent runtime libraries
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS_DEBUG "/MDd /W4")
    set(CMAKE_CXX_FLAGS_RELEASE "/MD /W4")
else()
    set(CMAKE_CXX_FLAGS_DEBUG "-g -D_DEBUG -Wall -Wextra -pedantic")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -Wall -Wextra -pedantic")
endif()

# Set build type to Release by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Add include directories
include_directories(include)

# Add source files
set(SOURCES
    src/main.cpp
)

# Create executable
add_executable(program ${SOURCES})

# Installation rules
install(TARGETS program DESTINATION bin)