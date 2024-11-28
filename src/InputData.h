#ifndef INPUT_H
#define INPUT_H

#include <Eigen/Dense>
#include <string>

struct InputData
{
<<<<<<< HEAD
    std::string _file_name;
    std::string _method;
    long _num_rows;
    long _num_cols;
    long _max_iters;
    double _toll;
    Eigen::MatrixXd A;
=======
    std::string method;
    long size;
    long num_iters;
    double tol;
    Eigen::MatrixXd input_matrix;
>>>>>>> e626d77faf4809bd91bca7472072c8076099e0e1
};

#endif //INPUT_H
