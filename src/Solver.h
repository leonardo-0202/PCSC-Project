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
<<<<<<< HEAD
    VectorXd solve();
    double powerMethod();
    VectorXd powerMethod(int shift);
    VectorXd inversePowerMethod();
    VectorXd inversePowerMethod(int shift);
    VectorXd qrMethod();
=======
    Eigen::VectorXd PowerMethod(int shift = 0);
    Eigen::VectorXd QR_Method();
    Eigen::MatrixXd InversesPowerMethod();
>>>>>>> e626d77faf4809bd91bca7472072c8076099e0e1
};

#endif //SOLVER_H

