#
# Copyright [2016] [Eugeny Gostkin]
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
# either express or implied. See the License for the specific
# language governing permissions and limitations under the License.
#
#

cmake_minimum_required(VERSION 3.2.2)

set(BIN deque)
project(${BIN})

if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    # require at least gcc 5.2
    if (CMAKE_C_COMPILER_VERSION VERSION_LESS 5.2)
        message(FATAL_ERROR "GCC version must be at least 5.2!")
    endif()
else()
    message(WARNING "You are using an unsupported compiler! Compilation has only been tested with GCC.")
endif()

find_package(Threads)
find_package(GTest)

add_compile_options(-O2)
link_libraries(pthread)
link_libraries(gtest)

set(HEADERS deque.h tests.h)
set(SOURCES main.cpp)

set(REQUIRED_LIBRARIES pthread gtest)

set(INSTALL_PATH /usr/local/bin/)

add_compile_options(-std=c++14 -g -Wall)

add_executable(${BIN} ${SOURCES})

target_link_libraries(${BIN} ${EXTRA_LIBS} ${REQUIRED_LIBRARIES})
install(TARGETS ${BIN} DESTINATION ${INSTALL_PATH})
