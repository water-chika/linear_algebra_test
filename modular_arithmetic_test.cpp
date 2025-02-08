#pragma once
#include <modular_arithmetic.hpp>

bool test_modular_arithmetic() {
    using m_t = linear_algebra::modular_arithmetic<size_t, 3>;
    m_t a{1}, b{2};
    auto c = a+b;
    assert(c == 0);
    auto d = a-b;
    assert(d == 2);
    auto e = a*b;
    assert(e == 2);
    return true;
}
