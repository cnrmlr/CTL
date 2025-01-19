cmake_minimum_required(VERSION 3.16)
project(physica)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES 
   hyperedge.cpp
   hypergraph.cpp
   identifiable_base.cpp
   node.cpp
   pattern_match.cpp
   rule.cpp
   uuid.cpp
)

add_library(physica STATIC ${SOURCES})
include_directories(PRIVATE ${CMAKE_SOURCE_DIR}/include)
