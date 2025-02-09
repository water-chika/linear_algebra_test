#pragma once

#include <functional>

#include <modular_arithmetic.hpp>

#include "test.hpp"

bool test_modular_arithmetic() {
    auto passed = true;
    auto a = linear_algebra::modular_arithmetic{1, linear_algebra::prime_number{std::integral_constant<int,3>{}}};
    decltype(a) b{2,std::integral_constant<int,3>{}};
    auto tests = std::vector<std::function<bool()>>{
        [a,b](){ return static_cast<int>(a+b) == 0; },
        [a,b](){ return static_cast<int>(a-b) == 2; },
        [a,b](){ return static_cast<int>(a*b) == 2; },
        [a,b](){ return static_cast<int>(a/b) == 2; },
    };
    return run_simple_tests("modular_arithmetic", tests);
}
