cmake_minimum_required(VERSION 3.16)
project(ctl-examples)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
   main.cpp
)

add_executable(ctl-examples ${SOURCES})
include_directories(PRIVATE ${CMAKE_SOURCE_DIR}/include)
