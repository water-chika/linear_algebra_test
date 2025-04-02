#pragma once

#include <permutation.hpp>

template<typename Scalar>
bool test_permutation() {
    using permutation::permutation;
    permutation s0{{2,0,1}}, s1{{1,2,0}};
    auto s2 = s0*s1;
    return s0(0)==2 && s0(1)==0 && s0(2)==1 &&
        s2(0) == 0 && s2(1) == 1 && s2(2) == 2;
}

template<class Scalar>
class permutation_tests_getter {
public:
    auto get_tests() {
        auto tests = std::vector<std::function<bool()>>{
            test_permutation<Scalar>,
        };
        return tests;
    }
};
