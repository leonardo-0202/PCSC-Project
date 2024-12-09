#ifndef OUTPUTDATA_H
#define OUTPUTDATA_H

/**
 * @struct OutputData
 * @brief Container for the output of a solver.
 */
struct OutputData {
    std::string method;
    double estimated_error;
    double execution_time;
    int iterations;
    Eigen::VectorXcd estimated_eigenvalues;
};

#endif //OUTPUTDATA_H