cmake_minimum_required(VERSION 3.16)
project(physica-examples)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fconcepts -fpermissive")
endif()

set(SOURCES
   main.cpp
)

add_executable(physica-examples ${SOURCES})
target_link_libraries(physica-examples physica)
include_directories(PRIVATE ${CMAKE_SOURCE_DIR}/include)
include_directories(PRIVATE ${CMAKE_SOURCE_DIR}/examples)
