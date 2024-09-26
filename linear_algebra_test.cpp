#include "matrix_test.hpp"
#include "vector_test.hpp"

int main() {
    test_vector();
    auto tests = {
        test_gram_schmidt,
        test_determinant,
        test_inverse
    };
    for (auto& test : tests) {
        test();
    }
    return 0;
}