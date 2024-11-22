#include <iostream>

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include "Reader.h"
#include "Solver.h"
#include "Output.h"

int main(int argc, char **argv) 
{
    // Read data and get matrix
    Reader data_reader(argv[0]);
    Eigen::VectorXd input_matrix = data_reader.readData();
    
    // Solver
    Solver eigen_solver(input_matrix);
    Eigen::VectorXd eigens = solver.PowerMethod();
    // Other methods

    // Output 
    Output output(eigens); // other args maybe from input
}
