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
    auto passed = determinant(A) == -2 &&
        determinant(B) == 0 &&
        determinant(C) == determinant(A) * determinant(A) &&
        determinant(D) == determinant(A) * determinant(B);
    log(passed ? " passed" : "failed");
    return passed;
}

template<class Number>
    requires (!std::floating_point<Number>)
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
template<std::floating_point Number>
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

template<class Number>
bool test_max_determinant() {
    auto elements = std::array<Number, 9>{};
    std::ranges::iota(elements, 1);
    std::ranges::sort(elements);
    Number max_det = -std::numeric_limits<Number>::infinity();
    auto A_with_max_det = linear_algebra::fixsized_matrix<Number, 3, 3>{};
    do {
        auto A = linear_algebra::fixsized_matrix<Number, 3, 3>{};
        foreach(A,
            [&A, &elements](auto i_j) {
                auto [i, j] = i_j;
                A[i_j] = elements[i*3+j];
            });
        auto det = determinant(A);
        if (det > max_det) {
            max_det = det;
            A_with_max_det = A;
        }
    } while (std::ranges::next_permutation(elements).found);
    return 412 == max_det;
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
