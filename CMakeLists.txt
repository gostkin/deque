cmake_minimum_required(VERSION 3.6)
project(deque)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp deque.h)
add_executable(deque ${SOURCE_FILES})