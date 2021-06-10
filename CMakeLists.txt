# CMake version
cmake_minimum_required(VERSION 3.7)

# Project name
project(emuchip8_t)

# Define the search path for header files
include_directories(inc)

# Specify the required C++ standard
#set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL2 REQUIRED)

# Define the executable
add_executable(emuchip8_t
	src/main.cpp src/emuchip8_t.cpp)

target_link_libraries(emuchip8_t SDL2::SDL2)

target_compile_options(emuchip8_t PRIVATE -Wall -Wextra -pedantic)
