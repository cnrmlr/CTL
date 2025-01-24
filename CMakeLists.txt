cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

project(physica)

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(test)
