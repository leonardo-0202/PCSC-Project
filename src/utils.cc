#include <fstream>
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <string>
#include <nlohmann/json.hpp>
#include "Reader.h"
#include "Solver.h"
using json = nlohmann::json;

Reader* createReader(std::string const& config_path)
{
    // Open input stream object and check existence
    std::cout << "Opening file ... " << config_path << std::endl << std::flush;
    std::filesystem::path file_path = config_path;
    std::ifstream config_file(file_path);
    if (!config_file.is_open()) {
        throw std::ios_base::failure("Failed to open config. file: " + config_path);
    }

    // Create json object
    json data = json::parse(config_file);
    config_file.close();
    
    // Extract run configuration

    std::string data_type = data.at("INPUT");
    std::string method = data.at("METHOD");
    int max_iters = data.at("MAX_ITERS");
    double tol = data.at("TOLERANCE");
    json opt_params = data.at(method);

    // Exception when valid option not given
    if (data_type != "FILE" && data_type != "FUNCTION"
        && data_type != "PICTURE") {
        throw std::ios_base::failure("Unsupported data type.");
    }
    if (method != "QR" && method != "POWER"
        && method != "INV") {
        std::cerr <<"WARNING: Unsupported solver method. Defaulting to QR method." << std::endl << std::flush;
        method = "QR";
    }
    if (max_iters < 0) {
        std::cerr <<"WARNING: Invalid max_iters. Defaulting to 100." << std::endl << std::flush;
        max_iters = 100;
    }
    if (tol <= 1e-16) {
        std::cerr <<"WARNING: Too small tolerance. Defaulting to 1e-16." << std::endl << std::flush;
        tol = 1e-16;
    }

    if (data_type == "FILE") {
        Reader *file_reader = new FileReader(method, max_iters, tol,
            opt_params, data["FILE"].at("PATH"));
        try {
            file_reader->genMatrix();
        }
        catch (const std::runtime_error& e) {
            std::cerr <<"FATAL ERROR: Error reading file." << std::endl << std::flush;
            std::exit(EXIT_FAILURE);
        }
        return file_reader;
    }
    else if (data_type == "FUNCTION") {
        FunctionReader * function_reader = new FunctionReader(method, max_iters, tol,
            opt_params, data["FUNCTION"].at("FUNC"), data["FUNCTION"].at("SIZE"));
        function_reader->genMatrix();

        return function_reader;
    }
    else if (data_type == "PICTURE") {
        PictureReader * picture_reader = new PictureReader(method, max_iters, tol,
            opt_params, data["PICTURE"].at("PATH"));
        try {
            picture_reader->genMatrix();
        }
        catch (const std::runtime_error& e) {
            std::cerr <<"FATAL ERROR: Error reading picture." << std::endl << std::flush;
            std::exit(EXIT_FAILURE);
        }
        return picture_reader;
    }
}

Solver* createSolver(Reader * reader)
{
    InputData input = reader->getInputData();
    Solver * solver;
    if (input.method == "QR")
    {
        solver = new QRSolver(reader->getInputData());
    }
    else if (input.method == "POWER")
    {
        solver = new PowerSolver(reader->getInputData());
    }
    else if (input.method == "INV")
    {
        solver = new InverseSolver(reader->getInputData());
    }
    return solver;
}