#ifndef INPUT_H
#define INPUT_H

#include <Eigen/Dense>
#include <string>
#include <nlohmann/json.hpp>

/**
 * @struct InputData
 * @brief Container for the input in a solver.
 */
struct InputData
{
    std::string method;
    int size;
    int num_iters;
    double tol;
    Eigen::MatrixXcd input_matrix;
    nlohmann::json method_config;
};

#endif //INPUT_H
