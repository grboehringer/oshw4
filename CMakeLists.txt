cmake_minimum_required(VERSION 2.8)

project(assignment4)

# add c and cpp flags
set(CMAKE_C_FLAGS "-std=c11 -Wall -Wextra -Wshadow -Werror")
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow -Werrori")

# include folder in CMake search path
include_directories(include)

# Add libraries

# openmp executable
add_executable(openmp src/3way-openmp/openmp)

# link libraries 
