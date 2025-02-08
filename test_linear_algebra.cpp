#include <complex>

#include "modular_arithmetic_test.hpp"
#include "vector_test.hpp"
#include "matrix_test.hpp"

template<typename Scalar>
bool run() {
    bool passed = true;
    passed = test_modular_arithmetic();
    passed = passed && test_vector<Scalar>();
    passed = passed && test_matrix<Scalar>();
    return passed;
}

using std::complex;

int main() {
    bool success = true;
    
    if (success) success = run<double>();
    if (success) success = run<float>();
    if (success) success = run<complex<double>>();
    if (success) success = run<complex<float>>();
    
    return 0;
}
