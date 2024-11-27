#ifndef OUTPUT_H
#define OUTPUT_H

#include <Eigen/Dense>

class Output
{
public:
    void saveOutput(Eigen::VectorXd eigenvalues);
};

#endif //OUTPUT_H