cmake_minimum_required(VERSION 3.20)

project(linear_algebra_test)

add_subdirectory(linear_algebra)

add_library(linear_algebra_test
    linear_algebra_test.cpp
    matrix_test.hpp
    vector_test.hpp
)
target_include_directories(linear_algebra_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(linear_algebra_test_double
    linear_algebra_test_double.cpp
    matrix_test.hpp
    vector_test.hpp
)

set_target_properties(linear_algebra_test PROPERTIES CXX_STANDARD 23)
target_link_libraries(linear_algebra_test linear_algebra)
set_target_properties(linear_algebra_test_double PROPERTIES CXX_STANDARD 23)
target_link_libraries(linear_algebra_test_double linear_algebra_test)
