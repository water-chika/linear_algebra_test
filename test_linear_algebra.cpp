#include <complex>

#include "modular_arithmetic_test.hpp"
#include "vector_test.hpp"
#include "matrix_test.hpp"
#include "permutation_test.hpp"

#include <iterator>

#include "hip_helper.hpp"

using std::complex;

template<typename T>
auto merge_tests(T&& lhs, T&& rhs) {
    lhs.reserve(lhs.size() + rhs.size());
    if (std::is_rvalue_reference_v<decltype(rhs)>) {
        std::move(rhs.begin(), rhs.end(), std::back_insert_iterator{lhs});
    }
    else {
        std::copy(rhs.begin(), rhs.end(), std::back_insert_iterator{lhs});
    }
    return std::forward<decltype(lhs)>(lhs);
}

template<typename... Ts1>
class merge_variant{
public:
    merge_variant(std::variant<Ts1...>){}
    template<typename... Ts2>
    std::variant<Ts1..., Ts2...> merge(std::variant<Ts2...>) {
        return {};
    }
};

template<typename T>
class variant_transform {
public:
    auto operator()(auto v) {
        T res;
        std::visit(
                [&res](auto& v) {
                    res = v;
                },
                v
                );
        return res;
    }
};

auto merge_tests(auto&& lhs, auto&& rhs) {
    using res_type = decltype(merge_variant(lhs[0]).merge(rhs[0]));
    auto res = std::vector<res_type>(lhs.size() + rhs.size());

    std::transform(rhs.begin(), rhs.end(), res.begin(),
            variant_transform<res_type>{}
            );
    std::transform(lhs.begin(), lhs.end(), res.begin()+rhs.size(),
            variant_transform<res_type>{}
            );

    return res;
}


template<template<typename> typename F, typename Scalar, typename... types>
class tests_getter {
public:
    auto get_tests() {
        F<Scalar> f;
        auto tests = f.get_tests();
        auto else_tests = tests_getter<F, types...>{}.get_tests();

        return merge_tests(else_tests, tests);
    }
};

template<template<typename> typename F, typename Scalar>
class tests_getter<F, Scalar> {
public:
    auto get_tests() {
        F<Scalar> f;
        return f.get_tests();
    }
};

template<template<typename> typename F, template<typename> typename... Fs>
class different_tests_getter {
public:
    template<typename... types>
    auto get_tests() {
        return merge_tests(
                different_tests_getter<Fs...>{}.template get_tests<types...>(),
                tests_getter<F, types...>{}.get_tests()
                );
    }
};
template<template<typename> typename F>
class different_tests_getter<F> {
public:
    template<typename... types>
    auto get_tests() {
        return tests_getter<F, types...>{}.get_tests();
    }
};

struct test_gpu {
    __device__
    static inline auto invoke() {
        using linear_algebra::fixsized_vector;
        using Number = float;
        using Scalar = float;
        uint32_t count = 0;
        count += test0<linear_algebra::fixsized_vector<Number, 2>>();
        count += test1<fixsized_vector<Number, 3>>();
        count += test_add<fixsized_vector<Number, 2>>();
        count += test_sub<fixsized_vector<Number, 2>>();
        count += test_element_multi<fixsized_vector<Number, 2>>();
        count += test_ranged_for<fixsized_vector<Number, 2>>();
        count += test_negative<fixsized_vector<Number, 2>>();

        count += test_determinant<Scalar>{}();
        return count;
    }
    __device__
    test_gpu() {
        m_res = invoke();
    }
    __device__
    operator uint32_t() {
        return m_res;
    }
    uint32_t m_res;
};

int main(int argc, const char* argv[]) {
    if (argc == 1) {
        bool success = true;

        auto tests =
            different_tests_getter<
                vector_tests_getter,
                matrix_tests_getter
                //modular_arithmetic_tests_getter,
                //permutation_tests_getter
                >{}.get_tests<
                    double,
                    float,
                    complex<double>,
                    complex<float>
                >();

        auto passed_count = run_tests("", tests);

        uint32_t gpu_pass_count = hip_helper::hybrid_call<test_gpu>{};

        if (gpu_pass_count < 8) {
            std::cerr << "gpu test pass count: " << gpu_pass_count;
        }

        return passed_count == tests.size() && gpu_pass_count ? 0 : -1;
    }
    else {
        auto tests =
            different_tests_getter<
                matrix_perf_tests_getter
            >{}.get_tests<
                double,
                float,
                complex<double>,
                complex<float>
            >();
        auto duration = run_tests("", tests);
        return 0;
    }
        
}
