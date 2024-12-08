#include <algorithm>
#include <complex>
#include "test_utils.h"

bool compareMagnitude(std::complex<double> const& a, std::complex<double> const& b)
{
    return std::abs(a) > std::abs(b);
}

std::vector<std::complex<double>> sort_eigenvalues(Eigen::VectorXcd const& eigenvals)
{
    // Create vector from eigenvalues
    std::vector<std::complex<double>> sorted_eigenvals(eigenvals.data(), eigenvals.data() + eigenvals.size());
    // Sort eigenvalues by magnitude
    std::sort(sorted_eigenvals.begin(), sorted_eigenvals.end(), compareMagnitude);

    return sorted_eigenvals;
}