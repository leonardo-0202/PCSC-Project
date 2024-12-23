#include <cmath>
#include <gtest/gtest.h>
#include "Solver.h"
#include "test_utils.h"
#include <Eigen/Eigenvalues>
#include <tuple>
#include <string>
#include "utils.h"

class Matrices
{
    public:
        Matrices() {  
            A_random3 = Eigen::MatrixXcd::Random(3, 3);
            A_random5 = Eigen::MatrixXcd::Random(5, 5);
            A_identity = Eigen::MatrixXcd::Identity(3, 3);
            A_diagonal_real = Eigen::MatrixXcd::Zero(3, 3);
            A_diagonal_real.diagonal() << Eigen::dcomplex(1.0, 0.0), 
                Eigen::dcomplex(2.0, 0.0), Eigen::dcomplex(3.0, 0.0);
            A_diagonal_complex = Eigen::MatrixXcd::Zero(3, 3);
            A_diagonal_complex.diagonal() << Eigen::dcomplex(1.0, 1.0), 
                Eigen::dcomplex(2.0, 7.0), Eigen::dcomplex(3.0, 4.0);
            A_symmetric = Eigen::MatrixXcd::Zero(4,4);
            A_symmetric << Eigen::dcomplex(2.0, 0.0), Eigen::dcomplex(3.0, 0.0), Eigen::dcomplex(1.0, 0.0), Eigen::dcomplex(4.0, 0.0),
                           Eigen::dcomplex(3.0, 0.0), Eigen::dcomplex(5.0, 0.0), Eigen::dcomplex(2.0, 0.0), Eigen::dcomplex(6.0, 0.0),
                           Eigen::dcomplex(1.0, 0.0), Eigen::dcomplex(2.0, 0.0), Eigen::dcomplex(8.0, 0.0), Eigen::dcomplex(7.0, 0.0),
                           Eigen::dcomplex(4.0, 0.0), Eigen::dcomplex(6.0, 0.0), Eigen::dcomplex(7.0, 0.0), Eigen::dcomplex(9.0, 0.0);
            
        }
        // Get matrices
        Eigen::MatrixXcd getRandom3() const { return A_random3; }
        Eigen::MatrixXcd getRandom5() const { return A_random5; }
        Eigen::MatrixXcd getIdentity() const { return A_identity; }
        Eigen::MatrixXcd getDiagonalReal() const { return A_diagonal_real; }
        Eigen::MatrixXcd getDiagonalComplex() const { return A_diagonal_complex; }
        Eigen::MatrixXcd getSymmetric() const { return A_symmetric; }

    protected:
        Eigen::MatrixXcd A_random3;
        Eigen::MatrixXcd A_random5;
        Eigen::MatrixXcd A_identity;
        Eigen::MatrixXcd A_diagonal_real;
        Eigen::MatrixXcd A_diagonal_flipped;
        Eigen::MatrixXcd A_diagonal_complex;
        Eigen::MatrixXcd A_symmetric;
};

class QRSolverTest : public Matrices, public ::testing::TestWithParam<Eigen::MatrixXcd> {
protected:
    void SetUp() override {
        matrix = GetParam();  // Get the matrix parameter
        input.num_iters = 2000;
        input.tol = 1e-6;
    }
    Eigen::MatrixXcd matrix;
    InputData input;
};

class PowerBasedSolverTest : public Matrices, public ::testing::TestWithParam<std::tuple<Eigen::MatrixXcd, Eigen::dcomplex>> {
protected:
    void SetUp() override {
        matrix = std::get<0>(GetParam());
        shift = std::get<1>(GetParam());
        input.num_iters = 2000;
        input.tol = 1e-6;
        if (shift.imag() < 0) { 
            sign = "-";  
        }
        else { sign = "+"; }
    }
    Eigen::MatrixXcd matrix;
    Eigen::dcomplex shift;
    std::string sign;
    InputData input;
};

TEST_P(PowerBasedSolverTest, SolverWithShiftOption) 
{
    int n = matrix.rows();
    Eigen::MatrixXcd shifted_matrix = matrix - shift*Eigen::MatrixXcd::Identity(n,n);

    // Input setup
    std::string complex_shift = std::to_string(shift.real()) + sign + std::to_string(shift.imag()) + "i";
    input.method_config["SHIFT"] = complex_shift;
    input.input_matrix = matrix;
    input.size = n;
    
    // True eigenvalues
    Eigen::ComplexEigenSolver<Eigen::MatrixXcd> true_solver(shifted_matrix);
    std::vector<Eigen::dcomplex> sorted_eigenvals = sort_eigenvalues(true_solver.eigenvalues());
    Eigen::dcomplex largest = sorted_eigenvals[0];
    Eigen::dcomplex smallest = sorted_eigenvals.back();

    // Power Solver
    PowerSolver powerSolver(input);
    powerSolver.solve();
    Eigen::dcomplex eigenval = powerSolver.getOutput().estimated_eigenvalues[0];
    EXPECT_NEAR(eigenval.real(), largest.real() + shift.real(), 1e-2); 
    EXPECT_NEAR(eigenval.imag(), largest.imag() + shift.imag(), 1e-2); 

    // Inverse Solver
    InverseSolver invSolver(input);
    invSolver.solve();
    eigenval = invSolver.getOutput().estimated_eigenvalues[0];
    EXPECT_NEAR(eigenval.real(), smallest.real() + shift.real(), 1e-2); 
    EXPECT_NEAR(eigenval.imag(), smallest.imag() + shift.imag(), 1e-2); 
}

TEST_P(QRSolverTest, SolverQR) 
{
    // Input setup
    int n = matrix.rows();
    input.input_matrix = matrix;
    input.size = n;

    // True eigenvalues
    Eigen::ComplexEigenSolver<Eigen::MatrixXcd> true_solver(matrix);
    std::vector<Eigen::dcomplex> sorted_eigenvals = sort_eigenvalues(true_solver.eigenvalues());

    // Power Solver
    QRSolver qrSolver(input);
    qrSolver.solve();
    Eigen::VectorXcd eigenvals = qrSolver.getOutput().estimated_eigenvalues;

    // Sort eigenvaluess from QR solver
    std::vector<Eigen::dcomplex> sorted_qr = sort_eigenvalues(eigenvals);

    for (int i=0; i<n; i++)
    {
        EXPECT_NEAR(sorted_qr[i].real(), sorted_eigenvals[i].real(), 1e-2);
        EXPECT_NEAR(sorted_qr[i].imag(), sorted_eigenvals[i].imag(), 1e-2);
    }
}

INSTANTIATE_TEST_SUITE_P(
    PowerBasedTestSuite,
    PowerBasedSolverTest,
    ::testing::Values(
        std::make_tuple(Matrices().getRandom3(), Eigen::dcomplex(5.0, 3.0)),
        std::make_tuple(Matrices().getRandom3(), Eigen::dcomplex(0.0, 0.0)),
        std::make_tuple(Matrices().getRandom5(), Eigen::dcomplex(0.0, 0.0)),
        std::make_tuple(Matrices().getIdentity(), Eigen::dcomplex(0.0, 0.0)),
        std::make_tuple(Matrices().getDiagonalReal(), Eigen::dcomplex(0.0, 0.0)),
        std::make_tuple(Matrices().getDiagonalComplex(), Eigen::dcomplex(3.0, 0.0)),
        std::make_tuple(Matrices().getSymmetric(), Eigen::dcomplex(0.0, 0.0))
    )
);

INSTANTIATE_TEST_SUITE_P(
    QRTestSuite,
    QRSolverTest,
    ::testing::Values(
        Matrices().getRandom3(),
        Matrices().getRandom5(),
        Matrices().getIdentity(),
        Matrices().getDiagonalReal(),
        Matrices().getDiagonalComplex(),
        Matrices().getSymmetric()
    )
);