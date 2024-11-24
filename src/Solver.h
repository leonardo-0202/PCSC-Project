#ifndef SOLVER_H
#define SOLVER_H

#include <Eigen/Dense>
#include "InputData.h"

class Solver
{
private:
    InputData input;
public:
    Solver(Eigen::MatrixXd input_matrix, int maxIterations);
    Eigen::VectorXd PowerMethod(int shift = 0);
    Eigen::MatrixXd InversesPowerMethod();
};



#endif //SOLVER_H

