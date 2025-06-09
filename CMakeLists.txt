cmake_minimum_required(VERSION 3.14)  # Minimum CMake version required

set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/g++)

project(eae-firmware VERSION 0.1.0 LANGUAGES C CXX)  # Project name and version, specifying C and C++ languages

# Set the C and C++ standards
set(CMAKE_C_STANDARD 11)  # Set C standard to C11
set(CMAKE_C_STANDARD_REQUIRED True)  # Ensure the C standard is required

set(CMAKE_CXX_STANDARD 14)  # Set C++ standard to C++14
set(CMAKE_CXX_STANDARD_REQUIRED True)  # Ensure the C++ standard is required

# Add subdirectories for source and test files
add_subdirectory(src)  # Includes the src directory
add_subdirectory(tests)  # Includes the tests directory

include(CTest)  # Include CTest module for testing support