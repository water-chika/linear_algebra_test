#pragma once

#include <chrono>
#include <execution>
#include <algorithm>
#include <syncstream>
#include <variant>
#include <vector>

template<typename... Ts>
auto variant_vector(Ts... ts) {
    return std::vector<std::variant<Ts...>>{ts...};
}

template<typename T>
struct sum_type {
    using type = T;
};
template<>
struct sum_type<bool> {
    using type = uint32_t;
};
template<typename T>
using sum_t = sum_type<T>::type;

auto run_tests(std::string name, auto& tests) {
    auto tests_size = tests.size();


    using res_type = sum_t<decltype(std::visit([](auto&test) { return test(); }, tests[0]))>;
    res_type sum{};
    std::for_each(std::execution::seq, tests.begin(), tests.end(),
            [&sum](auto& test) {
                auto duration = std::visit(
                        [](auto& test) {
                            return test();
                        },
                        test
                        );
                sum += duration;

                std::osyncstream synced_out(std::cout);
                std::visit(
                        [&synced_out](auto& test) {
                            synced_out << test.get_name();
                        },
                        test
                        );
                synced_out << ": ";
                synced_out << std::format("{}", duration) << std::endl;
            });
    return sum;
}
