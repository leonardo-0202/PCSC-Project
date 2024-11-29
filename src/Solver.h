#ifndef SOLVER_H
#define SOLVER_H

#include <Eigen/Dense>
#include "InputData.h"
#include "Output.h"

using namespace Eigen;

class Solver
{
    protected:
        int n;
        int num_iters;
        double tol;
        Eigen::MatrixXd A;
        //OutputData output;
    public:
        Solver(InputData input);
        virtual void solve() = 0;
};

class PowerSolver : public Solver
{
    protected:
        double shift;
    public:
        PowerSolver(InputData input);
        void solve();
};

class QRSolver : public Solver
{
    public:
        QRSolver(InputData input);
        void solve();
};

class InvSolver : public Solver
{
    protected:
        double shift;
    public:
        InvSolver(InputData input);
        void solve();
};

#endif //SOLVER_H

