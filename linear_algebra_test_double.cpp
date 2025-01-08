#include "matrix_test.hpp"
#include "vector_test.hpp"

int main() {
    test_vector<double>();
    auto tests = {
        test_gram_schmidt,
        test_determinant<double>,
        test_inverse<double>,
        test_max_determinant<double>,
        test_trapezoidal<double>,
    };
    for (auto& test : tests) {
        test();
    }
    linear_algebra_test::set_number<double>::set_index<size_t>::run();
    return 0;
}
