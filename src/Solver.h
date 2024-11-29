#ifndef SOLVER_H
#define SOLVER_H

#include <Eigen/Dense>
#include "InputData.h"

using namespace Eigen;

class Solver
{
    protected:
        InputData input;
        OutputData output;
    public:
        Solver();
        virtual void solve();
};

class PowerSolver : public Solver
{
    protected:
        double shift;
    public:
        PowerSolver(InputData input);
        void solve();
}

class QRSolver : public Solver
{
    public:
        QRSolver(InputData input);
        void solve();
}

class InvSolver : public Solver
{
    public:
        InvSolver(InputData input);
        void solve();
}

#endif //SOLVER_H

