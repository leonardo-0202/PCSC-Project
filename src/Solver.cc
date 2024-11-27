#include <Eigen/Dense>
#include "Solver.h"
#include "InputData.h"

Solver::Solver(InputData input_data)
    : input(std::move(input_data))
{
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
        ++ cnt;
    }
    return A.diagonal();
}