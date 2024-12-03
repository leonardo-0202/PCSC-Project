#ifndef SOLVER_H
#define SOLVER_H

#include "InputData.h"
#include "OutputData.h"
#include <Eigen/Dense>

class Solver
{
    protected:
        int n;
        int num_iters;
        double tol;
        Eigen::MatrixXcd A;
        OutputData output;
    public:
        Solver(InputData input);
        OutputData getOutput();
        virtual void solve() = 0;
};

class PowerSolver : public Solver
{
    protected:
        double shift;
    public:
        PowerSolver(InputData input);
        std::complex<double> powerMethod(Eigen::VectorXcd &b);
        void solve();
};

/**
 * @class QRSolver
 * @brief Derived Solver class that finds the eigenvalues using the QR Method.
 */
class QRSolver : public Solver
{
    public:
        /**
        * @brief Consturcts a QRSolver object.
        * @param input The input data of the problem.
        */
        QRSolver(InputData input);
        /**
        * @brief Computes the Q matrix in the QR decomposition of the input matrix.
        * @param A The matrix to find the decomposition of.
        */
        Eigen::MatrixXcd QRDecompQ(Eigen::MatrixXcd A);
        /**
        * @brief Solves for the eigenvalues using the QR method.
        */
        void solve();
};

class InvSolver : public Solver
{
    protected:
        double shift;
    public:
        InvSolver(InputData input);
        std::complex<double> invPowerMethod(Eigen::VectorXcd &b);
        void solve();
};

#endif //SOLVER_H

