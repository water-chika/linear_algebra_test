#include "matrix_test.hpp"
#include "vector_test.hpp"

int main() {
    std::cout << "test gram schmidt: " << (test_gram_schmidt()?"PASSED":"FAILED") << std::endl;
    test_vector();
    std::cout << "test determinant: " << (test_determinant() ? "PASSED" : "FAILED") << std::endl;
    return 0;
}