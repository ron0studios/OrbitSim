cmake_minimum_required(VERSION 3.16)

project(OrbitSim
  VERSION 1.0
  LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define compile flags for different configurations
set(DEBUG_FLAGS "-g;-O0;-Wall;-Wextra;-DDEBUG")
set(RELEASE_FLAGS "-O3;-DNDEBUG")

add_subdirectory(dependencies)
add_subdirectory(src)
