cmake_minimum_required(VERSION 3.13)
project(visit)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(catch2)

set(BENCHMARK_ENABLE_TESTING false)
add_subdirectory(gbench)

set(VISIT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(visit INTERFACE)
target_include_directories(visit INTERFACE $<BUILD_INTERFACE:${VISIT_INCLUDE_DIR}>)
target_include_directories(visit INTERFACE $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>)

add_executable(visit_test main.cpp include/visit.hpp tests.cpp)
target_link_libraries(visit_test PRIVATE visit Catch2::Catch2)

add_executable(visit_benchmark benchmark.cpp include/visit.hpp)
target_link_libraries(visit_benchmark PRIVATE visit benchmark benchmark_main)
