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

// Power method algorithm
void PowerSolver::solve()
{
    // Initialize the eigenvector and temp vector
    Eigen::VectorXd b = VectorXd::Random(n);
    Eigen::VectorXd b_tmp(n);
    // Declare eigenvalue and norm 
    double eigenval;
    double norm;

    for (int i=0; i<num_iters; i++)
    {
        // Compute matrix-vector multiplication
        b_tmp = A * b;
        // Compute the norm
        norm = b_tmp.norm();
        // Update the eigenvector b
        b = b_tmp / norm;
        // Compute the dominant eigenvalue via Rayleigh quotient w/ denominator = 1
        eigenval = b.transpose() * A * b;

        // Check for convergence
        if ( (b - b_tmp).norm() < tol) {
            break;
        }
    }
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
            err += pow(A(row+1, row), 2);
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

void InvSolver::solve()
{

}