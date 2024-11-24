#include "pcsc.hh"
#include "Reader.h"
#include "Solver.h"
#include <Eigen/Dense>
#include <iostream>

int main(int argc, char **argv)
{
    std::string file_name = "./test";
    std::string file_type = "csv";
    std::string method = "Power";
    long num_rows = 100;
    long num_cols = 100;
    long num_iters = 100;
    double toll = 10e-5;

    Reader data_reader = Reader(
        file_name, file_type, num_rows, num_cols,
        method, num_iters, toll
        );

    // Solver solver(data_reader.getInputData());
    // Output output = Solver.solve();
}

// Reader should take all users input, and save it to a custom
// data class, which then gets passed to a solver, which then
// creates an output which extracts solution to a CSV
