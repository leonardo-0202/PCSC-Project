#include <iostream>
#include <chrono>
#include <ctime>
#include <nlohmann/json.hpp>
#include "InputData.h"
#include "Solver.h"

Solver::Solver(InputData input) 
{
    n = input.size;
    num_iters = input.num_iters;
    tol = input.tol;
    A = input.input_matrix;
}

PowerSolver::PowerSolver(InputData input) : Solver(input)
{
    shift = input.method_config.at("POWER")["SHIFT"];
}

InvSolver::InvSolver(InputData input) : Solver(input)
{
    shift = input.method_config.at("INV")["SHIFT"];
}

QRSolver::QRSolver(InputData input) : Solver(input) {}

OutputData Solver::getOutput() {
    return output;
}

Eigen::dcomplex PowerSolver::powerMethod(Eigen::VectorXcd &b)
{
    // Declare eigenvalue
    Eigen::dcomplex eigenval;

    for (int i=0; i<num_iters; i++)
    {
        // Compute matrix-vector multiplication
        Eigen::VectorXcd b_tmp = A * b;
        // Compute the norm
        auto norm = b_tmp.norm();
        // Update the eigenvector b
        b = b_tmp / norm;
        // Compute the dominant eigenvalue via Rayleigh quotient w/ denominator = 1
        eigenval = Eigen::dcomplex(b.adjoint() * A * b) 
                    / Eigen::dcomplex((b.adjoint() * b));

        // Check for convergence
        if ( (b - b_tmp).norm() < tol) {
            break;
        }
    }
    return eigenval;
}

void PowerSolver::solve()
{
    // Create container for all eigenvalues
    Eigen::VectorXcd eigenvals(n);
    Eigen::VectorXcd b = VectorXcd::Random(n);
    // Apply shift to matrix A
    A -= shift * Eigen::MatrixXcd::Identity(n,n);

    auto a = std::chrono::high_resolution_clock::now();
    for (int i=0; i<n; i++)
    {
        // Get the largest eigenvalue via power method
        eigenvals(i) = powerMethod(b);
        // Deflate matrix
        A -= eigenvals(i) * b * b.adjoint();
    }

    auto b = std::chrono::high_resolution_clock::now();
    output.estimated_eigenvalues = eigenvals;
    //output.estimated_error = err;
    output.execution_time = std::chrono::duration_cast<std::chrono::microseconds>(b - a).count();
    // output.iterations = cnt;
    output.method = "Power Method";    
}

void InvSolver::solve()
{
    // Create container for all eigenvalues
    Eigen::VectorXcd eigenvals(n);
    Eigen::VectorXcd b = Eigen::VectorXcd::Random(n);
    // Apply shift to matrix A
    A -= shift * Eigen::MatrixXcd::Identity(n,n);

    //auto a = std::chrono::high_resolution_clock::now();
    for (int i=0; i<n; i++)
    {
        // Get the largest eigenvalue via power method
        eigenvals(i) = invPowerMethod(b);
        // Deflate matrix
        A -= eigenvals(i) * b * b.adjoint();
    }

    std::cout << eigenvals << std::endl;
}

Eigen::dcomplex InvSolver::invPowerMethod(Eigen::VectorXcd &b)
{
    // Declare eigenvalue
    Eigen::dcomplex eigenval;
    // Matrix decomposition
    auto decomp = A.completeOrthogonalDecomposition();
    for (int i=0; i<num_iters; i++)
    {
        Eigen::VectorXcd b_tmp(n);
        // Approximation of eigenvector
        b_tmp = decomp.solve(b);
        // Compute the norm
        auto norm = b_tmp.norm();
        // Update the eigenvector b
        b = b_tmp / norm;
        // Compute the dominant eigenvalue via Rayleigh quotient
        eigenval = Eigen::dcomplex(b.adjoint() * A * b) 
                        / Eigen::dcomplex(b.adjoint() * b);

        // Check for convergence
        if ( (b - b_tmp).norm() < tol) {
            break;
        }
    }
    return eigenval;   
}

void QRSolver::solve()
{
    int cnt = 0;
    double err = tol + 1;

    auto a = std::chrono::high_resolution_clock::now();
    while (cnt < num_iters && err >= tol) {
        auto QR = A.householderQr();
        auto Q = QR.householderQ();
        A = Q.transpose() * A * Q;
        cnt ++;

        // error calculation (sub_diagonal norm)
        err = 0;
        for(int row=0; row<n-1; row++) {
            err += pow(norm(A(row+1, row)) , 2);
        }
        err = sqrt(err);
    }
    auto b = std::chrono::high_resolution_clock::now();
    output.estimated_eigenvalues = A.diagonal();
    output.estimated_error = err;
    output.execution_time = std::chrono::duration_cast<std::chrono::microseconds>(b - a).count();
    output.iterations = cnt;
    output.method = "QR Method";
}