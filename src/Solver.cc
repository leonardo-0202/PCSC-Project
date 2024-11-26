<<<<<<< HEAD
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
Eigen::EigenVectorXd Solver::powerMethod()
{
    // Get the number of cols in the matrix A
    int n = input.input_matrix.cols();
    // Initialize the eigenvector and eigenvalue
    VectorXd x = VectorXd::Random(n);
    double eigenval = 0.0;

    for (int i=0; i<input.max_iters; i++)
    {
        VectorXd 
    }
=======
#include <Eigen/Dense>

Eigen::VectorXd QR_Method(Eigen::MatrixXd A, double const max_iters)
{
    long cnt = 0;
    while (cnt < max_iters) {
        auto QR = A.householderQr();
        auto Q = QR.householderQ();
        A = Q.transpose() * A * Q;
        ++ cnt;
    }
    return A.diagonal();
>>>>>>> 3425354e0cb9ed0ff0e24ee013cfab9bb2ba024f
}