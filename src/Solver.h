#ifndef SOLVER_H
#define SOLVER_H

#include <Eigen/Dense>
#include "InputData.h"

using namespace Eigen;

class Solver
{
private:
    InputData input;
    VectorXd eigenvec;
    double eigenval;
public:
    Solver(InputData input_data);
    Eigen::VectorXd PowerMethod(int shift = 0);
    Eigen::VectorXd QR_Method();
    Eigen::MatrixXd InversesPowerMethod();
};

#endif //SOLVER_H

