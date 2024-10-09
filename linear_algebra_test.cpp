#include "matrix_test.hpp"
#include "vector_test.hpp"

int main() {
    test_vector();
    auto tests = {
        test_gram_schmidt,
        test_determinant,
        test_inverse,
        test_max_determinant
    };
    for (auto& test : tests) {
        test();
    }
    linear_algebra_test::set_number<double>::set_index<size_t>::run();
    return 0;
}