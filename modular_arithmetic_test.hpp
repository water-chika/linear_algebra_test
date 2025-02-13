#pragma once

#include <functional>

#include <modular_arithmetic.hpp>

#include "test.hpp"

bool test_modular_arithmetic() {
    auto passed = true;
    using modular_arithmetic::mod;
    using modular_arithmetic::divides;
    auto add_mod3 = [](auto lhs, auto rhs) { return mod(lhs+rhs, 3); };
    auto sub_mod3 = [](auto lhs, auto rhs) { return mod(lhs-rhs, 3); };
    auto mul_mod3 = [](auto lhs, auto rhs) { return mod(lhs*rhs, 3); };
    auto div_mod3 = [](auto lhs, auto rhs) { return divides(lhs, rhs, 3); };
    auto tests = std::vector<std::function<bool()>>{
        [&add_mod3](){ return add_mod3(1,2) == 0; },
        [&sub_mod3](){ return sub_mod3(1,2) == 2; },
        [&mul_mod3](){ return mul_mod3(1,2) == 2; },
        [&div_mod3](){ return div_mod3(1,2) == 2; },
    };
    return run_simple_tests("modular_arithmetic", tests);
}
