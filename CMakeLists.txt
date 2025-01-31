cmake_minimum_required(VERSION 3.20)

project(linear_algebra_test)

add_subdirectory(linear_algebra)

add_executable(test_linear_algebra
    test_linear_algebra.cpp
    matrix_test.hpp
    vector_test.hpp
)
target_include_directories(test_linear_algebra PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(test_linear_algebra PROPERTIES CXX_STANDARD 23)
target_link_libraries(test_linear_algebra linear_algebra)
