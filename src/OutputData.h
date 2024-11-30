#ifndef OUTPUTDATA_H
#define OUTPUTDATA_H


struct OutputData {
    std::string method;
    double estimated_error;
    double execution_time;
    int iterations;
    Eigen::VectorXd estimated_eigenvalues;
};

#endif //OUTPUTDATA_H