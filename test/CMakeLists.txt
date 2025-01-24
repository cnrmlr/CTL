cmake_minimum_required(VERSION 3.16)
project(physica-tests)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(TEST_SOURCES
    main.cpp
    test_hypergraph.cpp
    test_rule.cpp
)

add_subdirectory(googletest)
add_executable(physica-tests ${TEST_SOURCES})
target_link_libraries(physica-tests PRIVATE gtest_main physica)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(./googletest/googletest/)
add_test(NAME physica-tests COMMAND physica-tests)
