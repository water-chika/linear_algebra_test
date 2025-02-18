#pragma once

#include <functional>

#include <modular_arithmetic.hpp>
#include <matrix/matrix.hpp>
#include <matrix/fixsized_matrix.hpp>
#include <matrix/combined_reference_matrix.hpp>

#include "test.hpp"

template<class Scalar>
class modular_arithmetic_tests_getter {
public:
    auto get_tests() {
        using modular_arithmetic::mod;
        using modular_arithmetic::divides;
        auto add_mod3 = [](auto lhs, auto rhs) { return mod(lhs+rhs, 3); };
        auto sub_mod3 = [](auto lhs, auto rhs) { return mod(lhs-rhs, 3); };
        auto mul_mod3 = [](auto lhs, auto rhs) { return mod(lhs*rhs, 3); };
        auto div_mod3 = [](auto lhs, auto rhs) { return divides(lhs, rhs, 3); };
        auto tests = std::vector<std::function<bool()>>{
            [add_mod3](){ return add_mod3(1,2) == 0; },
            [sub_mod3](){ return sub_mod3(1,2) == 2; },
            [mul_mod3](){ return mul_mod3(1,2) == 2; },
            [div_mod3](){ return div_mod3(1,2) == 2; },
            []() {
                auto add_mod26 = [](auto lhs, auto rhs) { return mod(lhs+rhs, 26); };
                auto sub_mod26 = [](auto lhs, auto rhs) { return mod(lhs-rhs, 26); };
                auto mul_mod26 = [](auto lhs, auto rhs) { return mod(lhs*rhs, 26); };
                auto div_mod26 = [](auto lhs, auto rhs) {
                    return modular_arithmetic::divides(lhs,rhs, 26);
                };
                auto inv_mod26 = [](auto v) {
                    return modular_arithmetic::inverse(v, 26);
                };
                auto is_invertible_mod26 = [](auto v) {
                    return modular_arithmetic::is_invertible(v, 26);
                };
                auto E = linear_algebra::fixsized_matrix<int, 3, 3>{
                    {2, 3, 15},
                    {5, 8, 12},
                    {1, 13, 4}
                };
                auto D = inverse(E, sub_mod26, mul_mod26, div_mod26, inv_mod26, is_invertible_mod26);
                auto I = linear_algebra::fixsized_matrix<int, 3, 3>{
                    {1, 0, 0},
                    {0, 1, 0},
                    {0, 0, 1}
                };
                return multiplies(D,E, add_mod26, mul_mod26)== I;
            },
        };
        return tests;
    }
};
