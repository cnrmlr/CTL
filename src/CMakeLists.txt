cmake_minimum_required(VERSION 3.16)
project(physica)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(WOLFRAM_PHYSICS_SOURCES 
   wolfram_physics/hyperedge.cpp
   wolfram_physics/hypergraph.cpp
   wolfram_physics/identifiable_base.cpp
   wolfram_physics/node.cpp
   wolfram_physics/pattern_match.cpp
   wolfram_physics/rule.cpp
   wolfram_physics/uuid.cpp
)

add_library(physica STATIC ${WOLFRAM_PHYSICS_SOURCES})
target_include_directories(physica PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(physica INTERFACE ${CMAKE_SOURCE_DIR}/include/wolfram_physics.h)
