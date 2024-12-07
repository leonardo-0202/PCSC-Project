#ifndef SOLVER_H
#define SOLVER_H

#include "InputData.h"
#include "OutputData.h"
#include <Eigen/Dense>

/** @file Solver.h
 * @brief Header file for all Solver based classes.
 */

class Solver
{
    protected:
        int n; /// Dimension of input matrix
        int num_iters; /// Maximum number of iterations for method
        double tol; /// Tolerance for measuring convergence
        Eigen::MatrixXcd A; /// Input matrix
        OutputData output; /// Struct for data gathered during solving
        std::string method_name; /// Name of method used
    public:
        /**
         * @brief Abstract Solver constructor
         * @param input The input data of the problem to initialize protected attributes.
         */
        Solver(InputData input);
        /**
         * @brief Method to get OutputData struct
         * @return OutputData struct
         */
        OutputData getOutput();
        /**
         * @brief Pure virtual function for running a solver
         * @return void: Alter OutputData object.
         */
        virtual void solve() = 0;
};

/**
 * @class PowerBasedSolver
 * @brief Derived Solver class. Abstract class for power based solvers. 
 * 
 * The eigenvec_approx method must be overriden by daughter classes.
 */
class PowerBasedSolver : public Solver
{
    protected:
        /// Shifted matrix given by: @f$ A - \mu I @f$ 
        Eigen::MatrixXcd shifted_matrix;
        /**
         * @brief Pure virtual function for eigenvector approximations. 
         * @param b Current approximation of the eigenvector. Passed to the method as a const reference to 'Eigen::VectorXcd'.
         * @return 'Eigen::VectorXcd' representing a step in approximating the eigenvector before normalization.
         */
        virtual Eigen::VectorXcd eigenvec_approx(Eigen::VectorXcd const& b) = 0;
    public:
        /**
        * @brief Base constructor for any PowerBased derived class.
        * @param input The input data of the problem contained in InputData struct.
        */
        PowerBasedSolver(InputData input); 
        /**
         * @brief Overriden solve method from Solver class. 
         * Generalized to work for shift, power, and inverse methods. 
         * @return void: Alter OutputData object from Solver class.
         */
        void solve() override;
};

/**
 * @class PowerSolver
 * @brief Derived PowerBasedSolver class. Implements specifics of power method. 
 * 
 * The eigenvec_approx method is implemented. 
 */
class PowerSolver : public PowerBasedSolver
{
    public:
        /**
         * @brief PowerSolver constructor
         * @param input The input data of the problem contained in InputData struct.
         */
        PowerSolver(InputData input);
        /**
         * @brief Power method implementation of eigenvector approximation at each iteration. 
         */
        Eigen::VectorXcd eigenvec_approx(Eigen::VectorXcd const& b) override;
};

/**
 * @class InverseSolver
 * @brief Derived PowerBasedSolver class. Implements specifics of inverse power method. 
 * 
 * The eigenvec_approx method is implemented. 
 */
class InverseSolver : public PowerBasedSolver
{
    protected:
        /// Matrix decomposition
        Eigen::CompleteOrthogonalDecomposition<Eigen::MatrixXcd> decomp;
    public:
        /**
         * @brief InverseSolver constructor
         * @param input The input data of the problem contained in InputData struct.
         */
        InverseSolver(InputData input);
        /**
         * @brief Inverse power method implementation of eigenvector approximation at each iteration.
         */
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
        * @brief Constructs a QRSolver object.
        * @param input The input data of the problem contained in InputData struct.
        */
        QRSolver(InputData input);
        /**
        * @brief Overriden solve method from Solver class. 
        * Solves for the eigenvalues using the QR method.
        */
        void solve() override;
};

#endif //SOLVER_H

