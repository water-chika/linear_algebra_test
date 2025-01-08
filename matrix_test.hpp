#pragma once

#include <matrix.hpp>
#include <vector.hpp>
#include <fixsized_matrix.hpp>

#include "matrix.hpp"
#include "combined_reference_matrix.hpp"
#include <iostream>
#include <concepts>
#include <source_location>
#include <algorithm>
#include <numeric>

void log(const std::string_view message,
    const std::source_location location =
    std::source_location::current())
{
    std::clog
        << location.function_name() << "`: "
        << message << '\n';
}

bool test_gram_schmidt() {
    auto A = linear_algebra::fixsized_matrix<float, 3, 3>{
        {1, 2, 3},
        {-1, 0, -3},
        {0, -2, 3}
    };
    auto B = linear_algebra::fixsized_matrix<float, 3, 3>{
        {       1,       1,       1,},
        {      -1,       1,       1,},
        {       0,      -2,       1,},
    };
    auto passed = B == gram_schmidt(A);
    log(passed ? " passed" : "failed");
    return passed;
}

template<class Number>
bool test_determinant() {
    using Matrix = linear_algebra::fixsized_matrix<Number, 3, 3>;
    auto A = Matrix{
        {1, 1, 0},
        {1, 0, 1},
        {0, 1, 1}
    };
    auto B = Matrix{
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    auto C = linear_algebra::fixsized_matrix<Number, 6, 6>{
        {1, 1, 0, 0, 0, 0},
        {1, 0, 1, 0, 0, 0},
        {0, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 0},
        {0, 0, 0, 1, 0, 1},
        {0, 0, 0, 0, 1, 1}
    };
    auto D = linear_algebra::fixsized_matrix<Number, 6, 6>{
        {1, 1, 0, 0, 0, 0},
        {1, 0, 1, 0, 0, 0},
        {0, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 2, 3},
        {0, 0, 0, 4, 5, 6},
        {0, 0, 0, 7, 8, 9}
    };
    auto passed = determinant(A) == Number{-2} &&
        determinant(B) == Number{0} &&
        determinant(C) == determinant(A) * determinant(A) &&
        determinant(D) == determinant(A) * determinant(B);
    log(passed ? " passed" : "failed");
    return passed;
}

template<class T>
concept exact_value_type = false;

template<exact_value_type Number>
bool test_inverse() {
    using M2 = linear_algebra::fixsized_matrix<Number, 2, 2>;
    using M3 = linear_algebra::fixsized_matrix<Number, 3, 3>;
    using Vector2 = linear_algebra::fixsized_vector<Number, 2>;
    using Vector3 = linear_algebra::fixsized_vector<Number, 3>;
    auto passed = 
            Vector2{ -2, 1 } ==
            inverse(
                M2{
                {2, 5},
                {1, 4}
                }
            ) * Vector2{1, 2}
        &&
            Vector3{3.0 / 4, -1.0 / 2, 1.0 / 4} ==
            inverse(
                M3{
                {2, 1, 0},
                {1, 2, 1},
                {0, 1, 2}
                }) * Vector3{1, 0, 0};
    log(passed ? " passed" : "failed");
    return true;
}
template<class Number>
    requires std::floating_point<Number> || complex_type<Number>
bool test_inverse() {
    using M2 = linear_algebra::fixsized_matrix<Number, 2, 2>;
    using M3 = linear_algebra::fixsized_matrix<Number, 3, 3>;
    using Vector2 = linear_algebra::fixsized_vector<Number, 2>;
    using Vector3 = linear_algebra::fixsized_vector<Number, 3>;
    auto passed = 
        length(
            Vector2{ -2, 1 } -
            inverse(
                M2{
                {2, 5},
                {1, 4}
                }
            ) * Vector2{1, 2}
        ) < 0.00001 &&

        length(
            Vector3{3.0 / 4, -1.0 / 2, 1.0 / 4} -
            inverse(
                M3{
                {2, 1, 0},
                {1, 2, 1},
                {0, 1, 2}
                }) * Vector3{1, 0, 0}) < 0.00001;
    log(passed ? " passed" : "failed");
    return true;
}

bool equal_or_near_equal(std::integral auto lhs, std::integral auto rhs, auto error) {
    return lhs == rhs;
}

bool equal_or_near_equal(auto lhs, auto rhs, std::floating_point auto error) {
    return std::abs(lhs - rhs) <= std::abs(error);
}

auto operator*(std::integral auto i, complex_type auto c) {
    return static_cast<typeof(c)>(i) * c;
}
auto operator*(complex_type auto c, std::integral auto i) {
    return static_cast<typeof(c)>(i) * c;
}
auto operator/(complex_type auto c, std::integral auto i) {
    return c/static_cast<typeof(c)>(i);
}
auto operator-(std::integral auto i, complex_type auto c) {
    return static_cast<typeof(c)>(i) - c;
}
auto operator-(complex_type auto c, std::integral auto i) {
    return c - static_cast<typeof(c)>(i);
}
auto operator+(std::integral auto i, complex_type auto c) {
    return static_cast<typeof(c)>(i) + c;
}


template<class Number>
bool test_max_determinant() {
    using namespace std;
    auto elements = std::array<Number, 9>{};
    std::ranges::iota(elements, 1);
    //std::ranges::sort(elements);
    Number max_det = -std::numeric_limits<Number>::infinity();
    auto A_with_max_det = linear_algebra::fixsized_matrix<Number, 3, 3>{};
    do {
        auto A = linear_algebra::fixsized_matrix<Number, 3, 3>{};
        foreach_index(A,
            [&A, &elements](auto i_j) {
                auto [i, j] = i_j;
                A[i_j] = elements[i*3+j];
            });
        auto det = determinant(A);
        if (real(det) > real(max_det)) {
            max_det = det;
            A_with_max_det = A;
        }
    } while (
            std::ranges::next_permutation(
                elements,
                [](auto& l, auto& r) {
                    return real(l) < real(r);
                }
            ).found);
    auto passed = equal_or_near_equal(static_cast<Number>(412), max_det, 0.1);
    log(passed ? " passed" : "failed");
    return passed;
}

template<class T>
constexpr T pi_v;
template<complex_type T>
constexpr T pi_v<T> = std::numbers::pi_v<double>;
template<std::floating_point T>
constexpr T pi_v<T> = std::numbers::pi_v<T>;

template<class Number>
bool test_trapezoidal() {
    auto delta_t = 2*pi_v<Number>/32;
    auto A = linear_algebra::fixsized_matrix<Number, 2, 2>{
        {4-delta_t*delta_t, 4*delta_t},
        {-4*delta_t, 4-delta_t*delta_t}
    }/(4+delta_t*delta_t);
    auto U = linear_algebra::fixsized_vector<Number, 2>{
        1,
        0
    };
    bool passed = true;
    for (int i = 0; i <= 32*100; i++) {
        auto len2 = length_square(U);
        if (!equal_or_near_equal(len2, static_cast<Number>(1), len2*(1.0/(1<<20))*i))
        {
            std::cout << "U" << i << " = " << len2 << std::endl;
            std::cout << "error = " << len2 - 1 << std::endl;
            passed = false;
            break;
        }
        U = A * U;
    }
    log(passed ? " passed" : "failed");
    return true;
}

class linear_algebra_test {
public:
    template<class Number>
    class set_number {
    public:
        template<std::integral Integral>
        class set_index {
        public:
            static void run() {
                linear_algebra::fixsized_vector<Number, 2> v{ 4.0f, 1.0f }, u{ 2.0f, -1.0f };
                std::cout << "v = " << v << ", u = " << u << std::endl;
                auto combination = Number{ 2.0 } *v + Number{ -3.0f }*u;
                std::cout << combination << std::endl;
                std::cout << dot_product(Number{ 0.5f } *v * Number{ 2.0f }, Number{ 2.0f } *u * Number{ 0.5f }) << std::endl;
                linear_algebra::fixsized_matrix<Number, 2, 2> A{ linear_algebra::column_vector{v}, linear_algebra::column_vector{u} };
                std::cout << "A is below: " << A << std::endl;
                std::cout << A.column(0) << std::endl;
                std::cout << A.column(1) << std::endl;
                std::cout << A.row(0) << std::endl;
                std::cout << A.row(1) << std::endl;
                A.row(1) -= A.row(0) * Number{2};
                std::cout << "After subtract 2 * row 0 from row 1, A is below: " << A << std::endl;
                
                auto U = linear_algebra::eliminate(A);
                std::cout << "The elimination U of A is below: " << U << std::endl;

                {
                    using namespace linear_algebra;
                    auto A_I = make_fixsized_matrix_with_columns<Number, 3, 6>({fixsized_vector<Number, 3>{2,1,1}, fixsized_vector<Number, 3>{1,2,1}, fixsized_vector<Number, 3>{1,1,2},
                        fixsized_vector<Number, 3>{1,0,0}, fixsized_vector<Number, 3>{0,1,0}, fixsized_vector<Number, 3>{0,0,1}});
                    std::cout << "set A_I = " << A_I << std::endl;
                    auto res = eliminate(A_I);
                    std::cout << "after eliminate, it =" << res << std::endl;
                    res = back_substitution(res);
                    std::cout << "after back substitution, it =" << res << std::endl;
                }
                {
                    auto A = linear_algebra::fixsized_matrix<int, 3, 3>{
                        {1, 0, 1},
                        {2, 3, 0},
                        {5, 4, 7}
                    };
                    std::cout << "test initializer, A = " << A << std::endl;
                }
                try{
                    using namespace linear_algebra;
                    std::cout << "This is chapter 2, exercise 31." << std::endl;
                    auto A = fixsized_matrix<Number, 3, 3>{
                        {2, 1, 1},
                        {1, 2, 1},
                        {1, 1, 2}
                    };
                    std::cout << "A = " << A << std::endl;
                    std::cout << "A inverse = " << inverse(A) << std::endl;
                    auto B = fixsized_matrix<Number, 3, 3>{
                        {2, -1, -1},
                        {-1, 2, -1},
                        {-1, -1, 2}
                    };
                    std::cout << "B = " << B << std::endl;
                    std::cout << "B inverse = " <<
                        inverse(B) << std::endl;
                }
                catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                }
            }
        };
    };
};
