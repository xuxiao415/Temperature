cmake_minimum_required(VERSION 3.6)
project(Temperature)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES temperature.cpp)
add_executable(Temperature ${SOURCE_FILES})