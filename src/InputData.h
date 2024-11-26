#ifndef INPUT_H
#define INPUT_H

#include <Eigen/Dense>
#include <string>

struct InputData
{
    std::string _file_name;
    std::string _method;
    long _num_rows;
    long _num_cols;
    long _max_iters;
    double _toll;
    Eigen::MatrixXd _input_matrix;
};

#endif //INPUT_H
