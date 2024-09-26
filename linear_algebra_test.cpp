#include "matrix_test.hpp"
#include "vector_test.hpp"

int main() {
    std::cout << "test gram schmidt: " << (test_gram_schmidt()?"PASSED":"FAILED") << std::endl;
    test_vector();
    return 0;
}