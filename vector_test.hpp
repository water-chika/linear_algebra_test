#pragma once
#include <vector/vector.hpp>
#include <vector/fixsized_vector.hpp>
#include <vector>
#include <functional>
#include <vector/resizeable_vector.hpp>

#include "test.hpp"

using namespace std::literals;

template<typename Child>
struct test_for {
    auto get_name() {
        return typeid(Child).name();
    }
};

struct simple_vector_test {
    simple_vector_test(bool (*p)()) : m_fun(p) {}
    bool operator()() {
        return m_fun();
    }
    auto get_name() {
        return "simple vector test"s;
    }
    std::function<bool()> m_fun;
};

template<class Vector>
__device__ __host__
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
class vector_tests_getter {
public:
    auto get_tests() {
        using namespace linear_algebra;
        auto simple_tests = std::vector<std::variant<simple_vector_test>>{
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
        return simple_tests;
    }
};
