#pragma once

#include <vector>
#include <functional>

#include <set.hpp>

template<class Scalar>
class set_tests_getter {
public:
    auto get_tests() {
        return std::vector<std::function<bool()>>{
            []() {
                return set::is_subset_v<int32_t, int64_t>;
            },
            []() {
                return !set::is_subset_v<uint32_t, int32_t>;
            },
            []() {
                return set::is_subset_v<int32_t, std::variant<int32_t, uint32_t>>;
            }
        };
    }
};
