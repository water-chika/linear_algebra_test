#pragma once
#include <vector.hpp>
#include <fixsized_vector.hpp>
#include <vector>
#include <functional>
#include <resizeable_vector.hpp>

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
    return 0.75 == dot_product(a, a);
}

template<class Vector>
bool test_add() {
    Vector a{ 0.5, -0.5 };
    Vector b{ -1.0, 1.5 };
    return a + b == linear_algebra::fixsized_vector<double, 2>{-0.5, 1.0};
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
    for (auto& e : a) {
        e = 1.0;
    }
    bool success = true;
    for (auto e : a) {
        success = success && e == 1.0;
    }
    return success;
}

template<class Vector>
bool test_negative() {
    Vector a{1.0, -1.0};
    return -a == Vector{-1.0, 1.0};
}

bool test_vector() {
    using namespace linear_algebra;
    std::vector<std::function<bool()>> tests{
        test0<fixsized_vector<float, 2>>,
        test0<resizeable_vector<float>>,
        test1<fixsized_vector<double, 3>>,
        test1<resizeable_vector<double>>,
        test_add<fixsized_vector<double, 2>>,
        test_add<resizeable_vector<double>>,
        test_sub<fixsized_vector<double, 2>>,
        test_sub<resizeable_vector<double>>,
        test_element_multi<fixsized_vector<double, 2>>,
        test_element_multi<resizeable_vector<double>>,
        test_ranged_for<fixsized_vector<double, 2>>,
        test_ranged_for<resizeable_vector<double>>,
        test_negative<fixsized_vector<double, 2>>,
        test_negative<resizeable_vector<double>>,
    };
    for (auto& test : tests) {
        if (test()) {
            std::cout << "passed" << std::endl;
        }
        else {
            std::cout << "failed" << std::endl;
        }
    }
    return true;
}
