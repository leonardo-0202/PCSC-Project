#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <string>
#include "Reader.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Reader* createReader(std::string const& config_path)
{
    // Open input stream object and check existence
    std::filesystem::path file_path = config_path;
    std::ifstream config_file(file_path);
    if (!config_file.is_open()) {
        std::cerr << "Error: Unable to open configuration file." << std::endl;
        return nullptr; // maybe raise error
    }

    // Create json object
    json data = json::parse(config_file);
    config_file.close();
    
    // Extract run configuration
    std::string data_type = data.at("INPUT");
    std::string method = data.at("METHOD");
    int size = data.at("SIZE");
    int max_iters = data.at("MAX_ITERS");
    double tol = data.at("TOLERANCE");
    json opt_params = data.at(method);

    // Exception when valid option not given
    if (data_type != "FILE" && data_type != "FUNCTION"
        && data_type != "DIAG") {
        std::cerr << "Error: Unsupported data type" << std::endl;
        return nullptr;
    }

    if (data_type == "FILE") {
        Reader *file_reader = new FileReader(method, size, max_iters, tol, opt_params, data["FILE"].at("PATH"));
        file_reader->readMatrix();

        return file_reader;
    }
    else if (data_type == "FUNCTION") {
        FunctionReader * function_reader = new FunctionReader();

    }
}

Solver* createSolver(Reader * reader)
{
    if (input.method == "QR") 
    {
        Solver *solver = new QR_Solver(reader->getInputData());
    }
    else if (input.method == "POWER")
    {
        Solver *solver = new PowerSolver(reader->getInputData());
    }
    else if (input.method == "INV")
    {
        Solver *solver = new InvSolver(reader->getInputData());
    }
    free(reader)
    return solver;
}