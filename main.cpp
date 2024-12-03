#include <iostream>
#include <complex>
#include <Eigen/Dense>

class Solver
{
    protected:
        int n;
        int num_iters;
        double tol;
        Eigen::MatrixXcd A;
    public:
        Solver(Eigen::MatrixXcd matrix, int n, int num_iters, double tol) : A(matrix),
            n(n), num_iters(num_iters), tol(tol) {};
        virtual void solve() = 0;
};

class PowerSolver : public Solver
{
    protected:
        double shift;
    public:
        PowerSolver(Eigen::MatrixXcd matrix, int n, int num_iters, double tol, double shift) :
            Solver(matrix, n, num_iters, tol), shift(shift) {}
        Eigen::dcomplex powerMethod(Eigen::VectorXcd &b) 
        {
            // Declare eigenvalue
            Eigen::dcomplex eigenval;

            for (int i=0; i<num_iters; i++)
            {
                Eigen::VectorXcd b_tmp(n);
                if (shift != 0) {
                    b_tmp = A.partialPivLu().solve(b);
                } else {
                    b_tmp = A * b;
                }
                // Compute the norm
                auto norm = b_tmp.norm();
                // Update the eigenvector b
                b = b_tmp / norm;
                // Compute the dominant eigenvalue via Rayleigh quotient
                eigenval = Eigen::dcomplex(b.adjoint() * A * b) 
                                / Eigen::dcomplex(b.adjoint() * b);

                // Check for convergence
                if ( (b - b_tmp).norm() < tol) {
                    break;
                }
            }
            return eigenval;   
        }

        void solve()
        {
            // Create container for all eigenvalues
            Eigen::VectorXcd eigenvals(n);
            Eigen::VectorXcd b = Eigen::VectorXcd::Random(n);
            // Apply shift to matrix A
            A -= shift * Eigen::MatrixXcd::Identity(n,n);

            //auto a = std::chrono::high_resolution_clock::now();
            for (int i=0; i<n; i++)
            {
                // Get the largest eigenvalue via power method
                eigenvals(i) = powerMethod(b);
                // Deflate matrix
                A -= eigenvals(i) * b * b.adjoint();
            }
            

            std::cout << eigenvals << std::endl;
            
        }
};

class InvSolver : public Solver
{
    protected:
        double shift;
    public:
        InvSolver(Eigen::MatrixXcd matrix, int n, int num_iters, double tol, double shift) :
            Solver(matrix, n, num_iters, tol), shift(shift) {};
        Eigen::dcomplex invPowerMethod(Eigen::VectorXcd &b);
        void solve();
};

void InvSolver::solve()
{
    // Create container for all eigenvalues
    Eigen::VectorXcd eigenvals(n);
    Eigen::VectorXcd b = Eigen::VectorXcd::Random(n);
    // Apply shift to matrix A
    A -= shift * Eigen::MatrixXcd::Identity(n,n);

    //auto a = std::chrono::high_resolution_clock::now();
    for (int i=0; i<n; i++)
    {
        // Get the largest eigenvalue via power method
        eigenvals(i) = invPowerMethod(b);
        // Deflate matrix
        A -= eigenvals(i) * b * b.adjoint();
    }

    std::cout << eigenvals << std::endl;
}

Eigen::dcomplex InvSolver::invPowerMethod(Eigen::VectorXcd &b)
{
    // Declare eigenvalue
    Eigen::dcomplex eigenval;
    // Matrix decomposition
    auto decomp = A.completeOrthogonalDecomposition();
    for (int i=0; i<num_iters; i++)
    {
        Eigen::VectorXcd b_tmp(n);
        // Approximation of eigenvector
        b_tmp = decomp.solve(b);
        // Compute the norm
        auto norm = b_tmp.norm();
        // Update the eigenvector b
        b = b_tmp / norm;
        // Compute the dominant eigenvalue via Rayleigh quotient
        eigenval = Eigen::dcomplex(b.adjoint() * A * b) 
                        / Eigen::dcomplex(b.adjoint() * b);

        // Check for convergence
        if ( (b - b_tmp).norm() < tol) {
            break;
        }
    }
    return eigenval;   
}

class ShiftSolver : public Solver
{
    protected:
        double shift;
    public:
        ShiftSolver(Eigen::MatrixXcd matrix, int n, int num_iters, double tol, double shift) :
            Solver(matrix, n, num_iters, tol), shift(shift) {};
        Eigen::dcomplex shiftMethod(Eigen::VectorXcd &b);
        void solve();  
};

Eigen::dcomplex ShiftSolver::shiftMethod(Eigen::VectorXcd &b)
{
    // Declare eigenvalue
    Eigen::dcomplex eigenval;

    auto decomp = A.completeOrthogonalDecomposition();
    for (int i=0; i<num_iters; i++)
    {
        Eigen::VectorXcd b_tmp(n);
        b_tmp = decomp.solve(b);
        // Compute the norm
        auto norm = b_tmp.norm();
        // Update the eigenvector b
        b = b_tmp / norm;
        // Compute the dominant eigenvalue via Rayleigh quotient
        eigenval = Eigen::dcomplex(b.adjoint() * A * b) 
                        / Eigen::dcomplex(b.adjoint() * b);

        // Check for convergence
        if ( (b - b_tmp).norm() < tol) {
            break;
        }
    }
    return eigenval;   
}

void ShiftSolver::solve()
{
    // Create container for all eigenvalues
    Eigen::VectorXcd eigenvals(n);
    Eigen::VectorXcd b = Eigen::VectorXcd::Random(n);
    // Apply shift to matrix A
    A -= shift * Eigen::MatrixXcd::Identity(n,n);

    //auto a = std::chrono::high_resolution_clock::now();
    for (int i=0; i<n; i++)
    {
        // Get the largest eigenvalue via power method
        eigenvals(i) = shiftMethod(b);
        // Deflate matrix
        A -= eigenvals(i) * b * b.adjoint();
    }

    
    std::cout << eigenvals + Eigen::VectorXcd::Constant(n, shift) << std::endl;
}

int main()
{
    // Example: Create a 3x3 matrix A
    Eigen::MatrixXcd A(3, 3);
    A << std::complex<double>(4, 0), std::complex<double>(1, 0), std::complex<double>(1, 0),
         std::complex<double>(1, 0), std::complex<double>(3, 0), std::complex<double>(1, 0),
         std::complex<double>(1, 0), std::complex<double>(1, 0), std::complex<double>(2, 0);

    // Create a PowerSolver instance and solve for eigenvalues
    ShiftSolver solver(A, 3, 1000, 1e-6, 1.0);
    solver.solve();

    return 0;
}