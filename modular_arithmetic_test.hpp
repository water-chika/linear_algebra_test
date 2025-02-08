#pragma once
#include <modular_arithmetic.hpp>

bool test_modular_arithmetic() {
    using m_t = linear_algebra::modular_arithmetic<size_t, 3>;
    auto passed = true;
    m_t a{1}, b{2};
    auto c = a+b;
    passed = passed && c == 0;
    auto d = a-b;
    passed = passed && d == 2;
    auto e = a*b;
    passed = passed && e == 2;
    return passed;
}
