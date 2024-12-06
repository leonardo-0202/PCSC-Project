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
        std::string method_name;
    public:
        Solver(InputData input);
        OutputData getOutput();
        virtual void solve() = 0;
};

class PowerBasedSolver : public Solver
{
    protected:
        Eigen::MatrixXcd shifted_matrix;
        virtual Eigen::VectorXcd eigenvec_approx(Eigen::VectorXcd const& b) = 0;
    public:
        PowerBasedSolver(InputData input); 
        void solve() override;
};

class PowerSolver : public PowerBasedSolver
{
    public:
        PowerSolver(InputData input);
        Eigen::VectorXcd eigenvec_approx(Eigen::VectorXcd const& b) override;
};

class InverseSolver : public PowerBasedSolver
{
    protected:
        Eigen::CompleteOrthogonalDecomposition<Eigen::MatrixXcd> decomp;
    public:
        InverseSolver(InputData input);
        Eigen::VectorXcd eigenvec_approx(Eigen::VectorXcd const& b) override;
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

#endif //SOLVER_H

