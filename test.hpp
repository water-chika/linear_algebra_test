#pragma once

#include <chrono>
#include <execution>
#include <algorithm>
#include <syncstream>
#include <variant>
#include <vector>

template<typename... Ts>
struct variant_container {
    variant_container(Ts... ts) : m_list{ts...} {
    }

    operator std::vector<std::variant<Ts...>> () {
        return m_list;
    }
    auto move_list() {
        return std::move(m_list);
    }

    std::vector<std::variant<Ts...>> m_list;
};

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

    std::cout << "{" << std::endl;
    std::atomic<bool> first_test = true;

    using res_type = sum_t<decltype(std::visit([](auto&test) { return test(); }, tests[0]))>;
    res_type sum{};
    std::for_each(std::execution::seq, tests.begin(), tests.end(),
            [&sum, &first_test](auto& test) {
                auto duration = std::visit(
                        [](auto& test) {
                            return test();
                        },
                        test
                        );
                sum += duration;

                std::osyncstream synced_out(std::cout);
                std::visit(
                        [&synced_out, &first_test](auto& test) {
                            if (!first_test) {
                                synced_out << ',';
                            }
                            synced_out << std::endl << "\"" << test.get_name() << "\"";
                            first_test = false;
                        },
                        test
                        );
                synced_out << ": ";
                synced_out << std::format("{}", duration);
            });

    std::cout << std::endl << "}" << std::endl;
    return sum;
}
