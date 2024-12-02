cmake_minimum_required(VERSION 3.15)

# Project name
project(Rtype LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
enable_testing()

# Add subdirectories
add_subdirectory(common)
add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(tests)

