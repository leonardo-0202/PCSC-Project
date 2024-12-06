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
    method_name = input.method;
}

PowerBasedSolver::PowerBasedSolver(InputData input) : Solver(input)
{
    double shift = input.method_config.at("SHIFT");
    shifted_matrix = A - shift*Eigen::MatrixXcd::Identity(n,n);
}

PowerSolver::PowerSolver(InputData input) : PowerBasedSolver(input) {}

InverseSolver::InverseSolver(InputData input) : PowerBasedSolver(input)
{
    decomp = shifted_matrix.completeOrthogonalDecomposition();
}

QRSolver::QRSolver(InputData input) : Solver(input) {}

void PowerBasedSolver::solve()
{
    // Declare eigenvalue and error
    Eigen::dcomplex eigenval;
    Eigen::VectorXcd residual;
    // Get random starting eigenvector
    Eigen::VectorXcd b = Eigen::VectorXcd::Random(n);
    
    auto start_time = std::chrono::high_resolution_clock::now();
    int i = 0;
    for (i; i<num_iters; i++)
    {
        // Approximation of eigenvector
        Eigen::VectorXcd b_tmp = eigenvec_approx(b);
        // Compute the norm
        auto norm = b_tmp.norm();
        // Update the eigenvector b
        b = b_tmp / norm;
        // Compute the eigenvalue via Rayleigh quotient
        eigenval = Eigen::dcomplex(b.adjoint() * A * b) 
                        / Eigen::dcomplex(b.adjoint() * b);

        // Check for convergence
        residual = A*b - eigenval*b;
        if (residual.norm() < tol) {
            break;
        }
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    output.estimated_eigenvalues[1] = eigenval;
    output.estimated_error = residual.norm();
    output.execution_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    output.iterations = i;
    output.method = method_name;
}

Eigen::VectorXcd PowerSolver::eigenvec_approx(Eigen::VectorXcd const& b)
{
    return shifted_matrix*b;
}

Eigen::VectorXcd InverseSolver::eigenvec_approx(Eigen::VectorXcd const& b)
{
    return decomp.solve(b);
}

Eigen::MatrixXcd QRSolver::QRDecompQ(Eigen::MatrixXcd A)
{
    Eigen::MatrixXcd Q = Eigen::MatrixXd::Identity(n,n);
    Eigen::MatrixXcd R = A;
    for (int j=0; j<n; j++) {
        Eigen::VectorXcd x = R.block(j, j, n - j, 1);
        Eigen::VectorXcd e = Eigen::VectorXcd::Zero(n - j);
        e(0) = std::complex<double>(x.norm(), 0);
        Eigen::VectorXcd v = x - e;
        v.normalize();

        Eigen::MatrixXcd H = Eigen::MatrixXcd::Identity(n, n);
        Eigen::MatrixXcd H_sub = Eigen::MatrixXcd::Identity(n - j, n - j) - 2.0 * (v * v.adjoint());
        H.block(j, j, n - j, n - j) = H_sub;

        Q = Q * H;
        R = H * R;
    }
    return Q;
}

void QRSolver::solve()
{
    int cnt = 0;
    double err = tol + 1;

    auto start_time = std::chrono::high_resolution_clock::now();
    while (cnt < num_iters && err >= tol) {
        auto Q = QRDecompQ(A);
        A = Q.transpose() * A * Q;
        cnt ++;

        // error calculation (sub_diagonal norm)
        err = 0;
        for(int row=0; row<n-1; row++) {
            err += pow(norm(A(row+1, row)) , 2);
        }
        err = sqrt(err);
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    output.estimated_eigenvalues = A.diagonal();
    output.estimated_error = err;
    output.execution_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    output.iterations = cnt;
    output.method = "QR Method";
}

OutputData Solver::getOutput() {
    return output;
}