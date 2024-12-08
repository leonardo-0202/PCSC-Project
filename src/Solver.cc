#include <iostream>
#include <chrono>
#include <ctime>
#include <nlohmann/json.hpp>
#include "InputData.h"
#include "Solver.h"

/**
 * @file Solver.cc
 * @brief Implementation file for all Solver based classes.
 */

/**
 * @brief Solver constructor to initialize the problem parameters.
 * 
 * Initializes the base class attributes using the provided input data.
 * @param input The input data of the problem to initialize protected attributes.
 */
Solver::Solver(InputData input) 
{
    n = input.size;
    num_iters = input.num_iters;
    tol = input.tol;
    A = input.input_matrix;
    method_name = input.method;
}

/**
 * @brief PowerBasedSolver constructor to initialize solver specific attributes.
 * 
 * Initializes the shifted matrix by applying the shift value to the input matrix.
 * @param input The input data of the problem contained in InputData struct.
 */
PowerBasedSolver::PowerBasedSolver(InputData input) : Solver(input)
{
    double shift = input.method_config.at("SHIFT");
    shifted_matrix = A - shift*Eigen::MatrixXcd::Identity(n,n);
}

/**
 * @brief PowerSolver constructor. Inherits from PowerBasedSolver.
 * 
 * Initializes the power method solver using the provided input data.
 * @param input The input data of the problem contained in InputData struct.
 */
PowerSolver::PowerSolver(InputData input) : PowerBasedSolver(input) {}

/**
 * @brief InverseSolver constructor. Inherits from PowerBasedSolver.
 * 
 * Initializes the inverse power method solver and performs matrix decomposition.
 * @param input The input data of the problem contained in InputData struct.
 */
InverseSolver::InverseSolver(InputData input) : PowerBasedSolver(input)
{
    decomp = shifted_matrix.completeOrthogonalDecomposition();
}

/**
 * @brief QRSolver constructor to initialize the QR method solver.
 * 
 * Initializes the QR-based solver with the given input data.
 * @param input The input data of the problem contained in InputData struct.
 */
QRSolver::QRSolver(InputData input) : Solver(input) {}

/**
 * @brief Solves the eigenvalue problem using a power-based method.
 * 
 * Iterates for a maximum number of iterations to approximate the eigenvector
 * and calculate the eigenvalue using the Rayleigh quotient. Convergence is
 * measured based on the residual norm.
 */
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
    output.estimated_eigenvalues(eigenval);
    output.estimated_error = residual.norm();
    output.execution_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    output.iterations = i;
    output.method = method_name;
}

/**
 * @brief Performs eigenvector approximation for the power method.
 * 
 * Multiplies the shifted matrix with the current eigenvector approximation.
 * @param b Current approximation of the eigenvector. Passed to the method as a const reference to 'Eigen::VectorXcd'.
 * @return Eigen::VectorXcd representing a step in approximating the eigenvector before normalization.
 */
Eigen::VectorXcd PowerSolver::eigenvec_approx(Eigen::VectorXcd const& b)
{
    return shifted_matrix*b;
}

/**
 * @brief Performs eigenvector approximation for the inverse power method.
 * 
 * Uses matrix decomposition to solve the system and approximate the eigenvector.
 * @param b Current approximation of the eigenvector. Passed to the method as a const reference to 'Eigen::VectorXcd'.
 * @return Eigen::VectorXcd representing a step in approximating the eigenvector before normalization.
 */
Eigen::VectorXcd InverseSolver::eigenvec_approx(Eigen::VectorXcd const& b)
{
    return decomp.solve(b);
}

/**
 * @brief Find all eigenvalues using the QR method.
 * 
 * Iteratively applies the QR algorithm to compute the eigenvalues of the matrix.
 */
void QRSolver::solve()
{
    int cnt = 0;
    double err = tol + 1;

    auto start_time = std::chrono::high_resolution_clock::now();
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
    auto end_time = std::chrono::high_resolution_clock::now();
    output.estimated_eigenvalues = A.diagonal();
    output.estimated_error = err;
    output.execution_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    output.iterations = cnt;
    output.method = "QR Method";
}

/**
 * @brief Returns the output data collected during the solving process.
 * 
 * Provides the estimated eigenvalues, error, execution time, iterations, and method used.
 * @return OutputData struct containing the results.
 */
OutputData Solver::getOutput() {
    return output;
}