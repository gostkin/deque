cmake_minimum_required(VERSION 3.6)
project(deque)

add_compile_options(-O2)
link_libraries(pthread)
link_libraries(gtest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp deque.h tests.h)
add_executable(deque ${SOURCE_FILES})

target_link_libraries(deque pthread gtest)