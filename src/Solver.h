#ifndef SOLVER_H
#define SOLVER_H

#include <Eigen/Dense>
#include <Eigen/Sparse>

class Solver 
{
    private:
        Eigen::VectorXd A;
        int max_iter;
    public:
        Solver(Eigen::VectorXd input_matrix, int iterations);
        Eigen::VectorXd PowerMethod();
        Eigen::VectorXd InversePowerMethod();
        Eigen::VectorXd QR();
};


#endif // SOLVER_H