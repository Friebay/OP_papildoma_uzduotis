cmake_minimum_required(VERSION 3.10)

# Project name and version
project(WordCounter VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directories
include_directories(include)

# Add source files
set(SOURCES
    src/main.cpp
)

# Create executable
add_executable(word_counter ${SOURCES})

# Installation rules
install(TARGETS word_counter DESTINATION bin)