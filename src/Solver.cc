#include "Solver.h"

// Custom constructor
Solver::Solver(InputData input_data)
{
    input = input_data;
}

// Member function to call the proper solving method 
Eigen::VectorXd Solver::solver()
{
    // something like this to abstract calling specific solver
    if (input._method == "power") 
    {
        Eigen::VectorXd eigenvec = powerMethod();
    }
    else if (input._method == "inverse")
    {
        Eigen::VectorXd eigenvec = inversePowerMethod();
    }
    else if (input._method == "power_shift") 
    {
        Eigen::VectorXd eigenvec = powerMethod(input._shift);
    }

    return eigenvec;
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

Eigen::VectorXd QR_Method(Eigen::MatrixXd A, double const max_iters)
{
    for (int i=0; i<max_iters; i++) 
    {
        auto QR = A.householderQr();
        auto Q = QR.householderQ();
        A = Q.transpose() * A * Q;
    }
    return A.diagonal();
}