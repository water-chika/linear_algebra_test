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

    auto tests = std::vector<std::function<bool()>>{
        run<double>,
        run<float>,
        run<complex<double>>,
        run<complex<float>>
    };

    auto all_passed = run_simple_tests("different element type", tests);
    
    return all_passed ? 0 : -1;
}
