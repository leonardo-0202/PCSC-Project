#include <iostream>
#include <filesystem>
#include "utils.h"
#include "Reader.h"
#include "Solver.h"
#include "OutputGenerator.h"


int main(int argc, char **argv)
{
    Reader * reader = createReader("..\\config.json");
    Solver * solver = createSolver(reader);

    solver->solve();
    OutputGenerator x = OutputGenerator(solver->getOutput());
    x.saveOutput();
    free(reader);
    free(solver);
    // Solver solver(data_reader.getInputData());
    // Output output = Solver.solve();
}

// Reader should take all users input, and save it to a custom
// data class, which then gets passed to a solver, which then
// creates an output which extracts solution to a CSV
