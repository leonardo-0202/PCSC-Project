#include <Eigen/Dense>
#include "Solver.h"
#include "InputData.h"

Solver::Solver(InputData input) 
{
    int n = input.size;
    int num_iters = input.num_iters;
    double tol = input.tol;
    Eigen::MatrixXd A;
}

PowerSolver(InputData input) : Solver(input) 
{
    shift = input.at("POWER")["SHIFT"];
}

InvSolver(InputData input) : Solver(input) 
{
    shift = input.at("INV")["SHIFT"];
}

QRSolver(InputData input) : Solver(input) {}

// Power method algorithm
double PowerSolver::solve()
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
    return eigenval;
}

Eigen::VectorXd QRSolver::solve()
{
    int cnt = 0;
    while (cnt < num_iters) {
        auto QR = A.householderQr();
        auto Q = QR.householderQ();
        A = Q.transpose() * A * Q;
    }
    return A.diagonal();
}