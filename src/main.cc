#include <iostream>
#include <filesystem>
#include "utils.h"
#include "Reader.h"
#include "Solver.h"
#include "OutputGenerator.h"


int main(int argc, char **argv)
{
    Reader * reader;
    try {
        if (argc==1) {
            std::filesystem::path default_conf = "../config.json";
            reader = createReader(default_conf);
        }
        else {
            std::filesystem::path config_path = argv[1];
            reader = createReader(config_path);
        }
    }
    catch (const std::ios_base::failure& e){
        std::cerr << "FATAL ERROR: " << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::cout << "Successfully created reader." << std::endl;
    Solver * solver = createSolver(reader);
    std::cout << "Successfully created solver." << std::endl;
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
