#ifndef SOLVER_H
#define SOLVER_H

#include <Eigen/Dense>
#include "InputData.h"

class Solver
{
private:
    InputData input;
public:
    Solver(InputData input_data);
    Eigen::VectorXd PowerMethod(int shift = 0);
    Eigen::VectorXd QR_Method();
    Eigen::MatrixXd InversesPowerMethod();
};



#endif //SOLVER_H

