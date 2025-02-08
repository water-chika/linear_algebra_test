#pragma once

bool run_simple_tests(std::string name, std::vector<std::function<bool()>> simple_tests) {
    auto simple_tests_size = simple_tests.size();
    bool all_passed = true;
    for (decltype(simple_tests_size) i = 0; i < simple_tests_size; i++) {
        auto& test = simple_tests[i];
        bool passed = test();
        all_passed = all_passed && passed;

        std::cout << name << ": simple test " << i << " ";
        if (passed) {
            std::cout << "passed" << std::endl;
        }
        else {
            std::cout << "failed" << std::endl;
        }
    }

    return all_passed;
}
