#ifndef TEST_UTILS_H
#define TEST_UTILS_H 

#include <vector>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

bool compareMagnitude(std::complex<double> const& a, std::complex<double> const& b);
std::vector<std::complex<double>> sort_eigenvalues(Eigen::VectorXcd const& eigenvals);

#endif // TEST_UTILS_H