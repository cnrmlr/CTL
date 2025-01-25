cmake_minimum_required(VERSION 3.16)
project(ctl-tests)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(TEST_SOURCES
    main.cpp
    test_hypergraph.cpp
)

add_subdirectory(googletest)
add_executable(ctl-tests ${TEST_SOURCES})
target_link_libraries(ctl-tests PRIVATE gtest_main)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(./googletest/googletest/)
add_test(NAME ctl-tests COMMAND ctl-tests)
