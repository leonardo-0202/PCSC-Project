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
    VectorXd solve();
    VectorXd powerMethod();
    VectorXd powerMethod(int shift);
    VectorXd inversePowerMethod();
    VectorXd inversePowerMethod(int shift);
    VectorXd qrMethod();
};

#endif //SOLVER_H

