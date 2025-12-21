#pragma once

#include <chrono>
#include <execution>
#include <algorithm>
#include <syncstream>

bool run_simple_tests(std::string name, std::vector<std::function<bool()>> simple_tests) {
    auto simple_tests_size = simple_tests.size();
    bool all_passed = true;
    auto clock = std::chrono::high_resolution_clock{};

    std::for_each(std::execution::par_unseq, simple_tests.begin(), simple_tests.end(),
            [&name, &all_passed, &clock, &simple_tests](auto& test) {
                auto begin_time = clock.now();
                bool passed = test();
                auto end_time = clock.now();
                auto duration = end_time - begin_time;
                all_passed = all_passed && passed;

                auto i = &test - simple_tests.data();

                std::osyncstream synced_out(std::cout);
                synced_out << name << ": simple test " << i << " ";
                if (passed) {
                    synced_out << "passed in " << duration << std::endl;
                }
                else {
                    synced_out << "failed" << std::endl;
                }
            });

    return all_passed;
}

void run_perf_tests(std::string name, auto& tests) {
    auto tests_size = tests.size();

    std::for_each(std::execution::seq, tests.begin(), tests.end(),
            [&name, &tests](auto& test) {
                auto duration = test();

                auto i = &test - tests.data();

                std::osyncstream synced_out(std::cout);
                synced_out << name << ": perf test " << i << " ";
                synced_out << duration << std::endl;
            });
}
