#pragma once

#include <vector/vector.hpp>

#include <matrix/matrix.hpp>
#include <matrix/fixsized_matrix.hpp>
#include <matrix/diagonal_matrix.hpp>
#include <matrix/dynamic_sized_matrix.hpp>
#include <matrix/combined_reference_matrix.hpp>

#include <iostream>
#include <concepts>
#include <source_location>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <random>
#include <numbers>
#include <functional>
#include <vector>

bool equal_or_near_equal(std::integral auto lhs, std::integral auto rhs, auto error) {
    return lhs == rhs;
}

template<typename T>
    requires (std::floating_point<T> || complex_type<T>)
bool equal_or_near_equal(T lhs, auto rhs, std::floating_point auto error) {
    return std::abs(lhs - rhs) <= std::abs(error);
}

bool equal_or_near_equal(linear_algebra::concept_helper::matrix auto&& A,
        linear_algebra::concept_helper::matrix auto&& B, std::floating_point auto error) {
    if (A.size() != B.size()) {
        return false;
    }
    bool equal = true;
    foreach_index(A,
            [&A, &B, &equal, error](auto index) {
                if (!equal_or_near_equal(A[index],B[index], error)) {
                    equal = false;
                }
            });
    return equal;
}

template<class Number>
class random_matrix {
public:
    random_matrix()
        : m_generator{0}, m_distribution{-2,2}
    {}
    auto operator()() {
        auto A = linear_algebra::fixsized_matrix<Number, 3, 3>{};
        this->operator()(A);
        return A;
    }
    auto& operator()(linear_algebra::matrix auto& A) {
        foreach_element(A,
                [this](auto& e) {
                    e = static_cast<Number>(m_distribution(m_generator));
                });
        return A;
    }
private:
    std::minstd_rand0 m_generator;
    std::uniform_int_distribution<int> m_distribution;
};

bool test_gram_schmidt(linear_algebra::matrix auto A) {
    using element_t = linear_algebra::element_type<decltype(A)>;
    auto [Q, R] = gram_schmidt(A);
    auto QT_Q = transpose(Q)*Q;
    bool passed = true;
    foreach_index(QT_Q,
            [&QT_Q, &passed](auto i) {
                if (i.get_column() == i.get_row()) {
                    passed = passed &&
                        ( equal_or_near_equal(QT_Q[i], static_cast<element_t>(1), 0.0001)
                          ||
                          equal_or_near_equal(QT_Q[i], static_cast<element_t>(0), 0.0001)
                        );
                }
                else {
                    passed = passed && equal_or_near_equal(QT_Q[i], static_cast<element_t>(0), 0.0001);
                }
            });
    passed = passed && equal_or_near_equal(A,Q*R, 0.0001);
    if (!passed) {
        std::cout << "A=" << A << std::endl;
        std::cout << "Q=" << Q << std::endl;
        std::cout << "QT*Q=" << QT_Q << std::endl;
        std::cout << "R=" << R << std::endl;
        std::cout << "Q*R=" << Q*R << std::endl;
    }
    return passed;
}

template<class Number>
bool test_gram_schmidt() {
    auto passed = true;
    auto get_random_matrix = random_matrix<Number>{};
    for (size_t i = 0; passed && i < 100000ul; i++) {
        auto A = get_random_matrix();
        passed = passed && test_gram_schmidt(A);
        using matrix_type = linear_algebra::dynamic_sized_matrix<Number>;
        auto B = matrix_type{{3,3}};
        get_random_matrix(B);
        passed = passed && test_gram_schmidt(B);
    }
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
    return true;
}


auto operator*(std::integral auto i, complex_type auto c) {
    return static_cast<decltype(c)>(i) * c;
}
auto operator*(complex_type auto c, std::integral auto i) {
    return static_cast<decltype(c)>(i) * c;
}
auto operator/(complex_type auto c, std::integral auto i) {
    return c/static_cast<decltype(c)>(i);
}
auto operator-(std::integral auto i, complex_type auto c) {
    return static_cast<decltype(c)>(i) - c;
}
auto operator-(complex_type auto c, std::integral auto i) {
    return c - static_cast<decltype(c)>(i);
}
auto operator+(std::integral auto i, complex_type auto c) {
    return static_cast<decltype(c)>(i) + c;
}


template<class Number>
bool test_max_determinant() {
    using namespace std;
    auto elements = std::array<Number, 9>{};
    std::iota(elements.begin(), elements.end(), 1);
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
    return passed;
}

#ifdef __cpp_lib_math_constants
template<class T>
constexpr T pi_v;
template<complex_type T>
constexpr T pi_v<T> = std::numbers::pi_v<double>;
template<std::floating_point T>
constexpr T pi_v<T> = std::numbers::pi_v<T>;
#else
template<class T>
constexpr T pi_v = 3.1415926;
template<complex_type T>
constexpr T pi_v<T> = 3.1415926;
template<std::floating_point T>
constexpr T pi_v<T> = 3.1415926;
#endif

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
    return true;
}

template<class Number>
bool test_eigenvalues() {
    using namespace std;
    auto test = [](auto A) {
        auto eigenvalues = linear_algebra::eigenvalues(A);

        auto passed = true;
        for (auto eigenvalue : eigenvalues) {
            decltype(A) eigenvalue_I = linear_algebra::I;
            eigenvalue_I = eigenvalue*eigenvalue_I;
            auto det = determinant(A - eigenvalue_I);
            passed = passed && std::abs(det) < 0.0001;
        }
        return passed;
    };

    auto t = static_cast<Number>(pi_v<Number>/16);
    auto A = linear_algebra::fixsized_matrix<Number, 2, 2> {
        {cos(t), sin(t)},
        {sin(t), 0}
    };

    auto passed = test(A) &&
        test(
                linear_algebra::fixsized_matrix<Number, 2, 2> {
                    {5, 5},
                    {5, 5}
                }
            );

    return passed;
}

template<class Number>
bool test_diagonal_matrix() {
    auto A = linear_algebra::fixsized_matrix<Number, 3, 3> {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    auto D = linear_algebra::make_diagonal_matrix<Number, 3>({1,2,3});
    auto B = A*D;

    return true;
}

bool test_svd(auto A) {
    auto [U, S, VT] = svd(A);
    auto V = transpose(VT);
    auto passed = equal_or_near_equal(A*V, U*S, 0.00001);
    if (!passed) {
        std::cout << "A*V=" << A*V << std::endl;
        std::cout << "U*S=" << U*S << std::endl;
        std::cout << "A*V-U*S=" << A*V-U*S << std::endl;
    }

    return passed;
}

template<class Number>
bool test_svd_simple() {
    auto A = linear_algebra::fixsized_matrix<Number, 10, 10> {};
    foreach_index(A,
            [&A](auto i) {
                auto c = i.get_column(), r = i.get_row();
                A[i] = c*c + r*r;
            });
    auto N = A.size().get_column();
    A /= static_cast<linear_algebra::element_type<decltype(A)>>(N*N);
    auto passed = test_svd(A);

    auto get_random_matrix = random_matrix<Number>();
    for (int i = 0; passed && i < 100; i++) {
        auto A = get_random_matrix();
        passed = passed && test_svd(A);
    }

    return passed;
}

template<class Scalar>
class matrix_tests_getter {
public:
    auto get_tests() {
        auto tests = std::vector<std::function<bool()>>{
            test_gram_schmidt<Scalar>,
            test_determinant<Scalar>,
            test_inverse<Scalar>,
            test_max_determinant<Scalar>,
            test_trapezoidal<Scalar>,
            test_eigenvalues<Scalar>,
            test_diagonal_matrix<Scalar>,
            test_svd_simple<Scalar>,
        };
        return tests;
    }
};
