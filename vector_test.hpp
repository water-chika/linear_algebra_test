#pragma once
#include <vector/vector.hpp>
#include <vector/fixsized_vector.hpp>
#include <vector>
#include <functional>
#include <vector/resizeable_vector.hpp>

#include "test.hpp"

template<class Vector>
bool test0() {
    using Element = float;
    Vector a{1.0f, 0.0f};
    a *= 2.0f;
    return a == Vector{2.0f, 0.0f};
}

template<class Vector>
bool test1() {
    Vector a{ 0.5, 0.5, 0.5 };
    return 0.75 == length_square(a);
}

template<class Vector>
bool test_add() {
    Vector a{ 0.5, -0.5 };
    Vector b{ -1.0, 1.5 };
    return a + b == Vector{-0.5, 1.0};
}

template<class Vector>
bool test_sub() {
    Vector a{ 0.5, -0.5 };
    Vector b{ -1.0, 1.5 };
    return a - b == Vector{1.5, -2.0};
}

template<class Vector>
bool test_element_multi() {
    Vector a{ 0.5, -0.5 };
    Vector b{ -1.0, 1.5 };
    return element_multi(a, b) == Vector{-0.5, -0.5*1.5};
}

template<class Vector>
bool test_ranged_for() {
    Vector a{};
    linear_algebra::element_type<Vector> one = 1;
    for (auto& e : a) {
        e = one;
    }
    bool success = true;
    for (auto e : a) {
        success = success && e == one;
    }
    return success;
}

template<class Vector>
bool test_negative() {
    Vector a{1.0, -1.0};
    return -a == Vector{-1.0, 1.0};
}

template<class Number>
bool test_vector() {
    using namespace linear_algebra;
    std::vector<std::function<bool()>> simple_tests{
        test0<fixsized_vector<Number, 2>>,
        test0<resizeable_vector<Number>>,
        test1<fixsized_vector<Number, 3>>,
        test1<resizeable_vector<Number>>,
        test_add<fixsized_vector<Number, 2>>,
        test_add<resizeable_vector<Number>>,
        test_sub<fixsized_vector<Number, 2>>,
        test_sub<resizeable_vector<Number>>,
        test_element_multi<fixsized_vector<Number, 2>>,
        test_element_multi<resizeable_vector<Number>>,
        test_ranged_for<fixsized_vector<Number, 2>>,
        test_ranged_for<resizeable_vector<Number>>,
        test_negative<fixsized_vector<Number, 2>>,
        test_negative<resizeable_vector<Number>>,
    };
    auto all_passed = run_simple_tests("vector tests", simple_tests);
    return all_passed;
}
