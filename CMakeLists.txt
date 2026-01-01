cmake_minimum_required(VERSION 3.20)

project(linear_algebra_test)

add_executable(test_linear_algebra
    test_linear_algebra.cpp
    matrix_test.hpp
    vector_test.hpp
)
target_include_directories(test_linear_algebra PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(test_linear_algebra PUBLIC cxx_std_23)
target_link_libraries(test_linear_algebra linear_algebra)

#find_library(tbb tbb)
#target_link_libraries(test_linear_algebra tbb)
