#include <iostream>
#include <filesystem>
#include <exception>
#include "utils.h"
#include "Reader.h"
#include "Solver.h"
#include "Exceptions.h"
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
    catch (const ConfigError &e) {
        std::cout << "Configuration File error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (const ReaderError &e) {
        std::cout << "Reader error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (const std::exception &e){
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Successfully created reader." << std::endl;
    Solver * solver = createSolver(reader);
    std::cout << "Successfully created solver." << std::endl;
    solver->solve();

    OutputGenerator x = OutputGenerator(solver->getOutput());
    x.saveOutput();

    free(reader);
    free(solver);
}