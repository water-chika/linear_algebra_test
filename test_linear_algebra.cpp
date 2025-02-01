#include <complex>

#include "matrix_test.hpp"
#include "vector_test.hpp"

template<typename Scalar>
bool run() {
    test_vector<Scalar>();
    auto tests = {
        test_gram_schmidt,
        test_determinant<Scalar>,
        test_inverse<Scalar>,
        test_max_determinant<Scalar>,
        test_trapezoidal<Scalar>,
        test_eigenvalues<Scalar>,
        test_diagonal_matrix<Scalar>,
        test_svd<Scalar>,
    };
    bool success = true;
    for (auto& test : tests) {
        success = test();
        if (!success)
            break;
    }
    using t = linear_algebra_test::template set_number<Scalar>::template set_index<size_t>;
    t::run();
    return success;
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
