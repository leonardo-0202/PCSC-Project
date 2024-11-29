#include <iostream>
#include <Eigen/Dense>
#include <filesystem>
#include "utils.cc"
#include "Reader.h"
#include "Solver.h"

int main(int argc, char **argv)
{
    // FOR NOW CONFIG/CONFIG.TXT IS IN cmake-build-debug
    Reader * reader = createReader("config/config.json");
    // Solver solver(reader->getInputData());
    Solver * solver = createSolver(reader);
    // Eigen::VectorXd ans = solver.QR_Method();
    solver->solve();

    free(reader);
    free(solver);
    // Solver solver(data_reader.getInputData());
    // Output output = Solver.solve();
}

// Reader should take all users input, and save it to a custom
// data class, which then gets passed to a solver, which then
// creates an output which extracts solution to a CSV
