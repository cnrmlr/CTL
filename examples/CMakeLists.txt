cmake_minimum_required(VERSION 3.16)
project(physica-examples)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
   main.cpp
)

add_executable(physica-examples ${SOURCES})
target_link_libraries(physica-examples physica)
include_directories(PRIVATE ${CMAKE_SOURCE_DIR}/include)
