cmake_minimum_required(VERSION 3.6)
project(deque)

set(CMAKE_CXX_FLAGS "-pthread ${CMAKE_CXX_FLAGS} -lgtest -lpthread -std=c++11 -O2")

set(SOURCE_FILES main.cpp deque.h tests.h)
add_executable(deque ${SOURCE_FILES})

target_link_libraries(deque pthread gtest)