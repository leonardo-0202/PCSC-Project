#ifndef INPUT_H
#define INPUT_H

#include <Eigen/Dense>
#include <string>

struct InputData
{
    std::string method;
    long size;
    long num_iters;
    double tol;
    Eigen::MatrixXd input_matrix;
};

#endif //INPUT_H
