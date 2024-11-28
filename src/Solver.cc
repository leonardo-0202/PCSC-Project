#include <Eigen/Dense>
#include "Solver.h"
#include "InputData.h"

// Custom constructor
Solver::Solver(InputData input_data)
    : input(std::move(input_data))
{
}

Solver::Solver(InputData input_data)
{
    input = input_data;
}

// Power method algorithm
double Solver::powerMethod()
{
    // Get the number of cols in the matrix A
    int n = input.A.cols();
    // Initialize the eigenvector and temp vector
    Eigen::VectorXd b = VectorXd::Random(n);
    Eigen::VectorXd b_tmp(n);
    // Declare eigenvalue and norm 
    double eigenval;
    double norm;

    for (int i=0; i<input.max_iters; i++)
    {
        // Compute matrix-vector multiplication
        b_tmp = input.A * b;
        // Compute the norm
        norm = b_tmp.norm();
        // Update the eigenvector b
        b = b_tmp / norm;
        // Compute the dominant eigenvalue via Rayleigh quotient w/ denominator = 1
        eigenval = b.transpose() * input.A * b;

        // Check for convergence
        if ( (b - b_tmp).norm() < input.tolerance) {
            break;
        }
    }
    return eigenval;
}

Eigen::VectorXd Solver::QR_Method()
{
    auto A = input.input_matrix;
    auto num_iters = input.num_iters;
    long cnt = 0;
    while (cnt < num_iters) {
        auto QR = A.householderQr();
        auto Q = QR.householderQ();
        A = Q.transpose() * A * Q;
    }
    return A.diagonal();
}