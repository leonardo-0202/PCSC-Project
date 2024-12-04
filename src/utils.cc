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
    std::cout << "Opening file ... " << config_path << std::endl;
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
    int size = data.at("SIZE");
    int max_iters = data.at("MAX_ITERS");
    double tol = data.at("TOLERANCE");
    json opt_params = data.at(method);

    // Exception when valid option not given
    if (data_type != "FILE" && data_type != "FUNCTION"
        && data_type != "PICTURE") {
        throw std::ios_base::failure("Unsupported data type.");
    }

    if (data_type == "FILE") {
        Reader *file_reader = new FileReader(method, size, max_iters, tol,
            opt_params, data["FILE"].at("PATH"));
        file_reader->genMatrix();   

        return file_reader;
    }
    else if (data_type == "FUNCTION") {
        FunctionReader * function_reader = new FunctionReader(method, size, max_iters, tol,
            opt_params, data["FUNCTION"].at("FUNC"));
        function_reader->genMatrix();

        return function_reader;
    }
    else if (data_type == "PICTURE") {
        PictureReader * picture_reader = new PictureReader(method, size, max_iters, tol,
            opt_params, data["PICTURE"].at("PATH"));
        picture_reader->genMatrix();

        return picture_reader;
    }
    std::cout << "Reader created succesfully." << std::endl;
}

Solver* createSolver(Reader * reader)
{
    InputData input = reader->getInputData();
    Solver * solver;
    if (input.method != "QR" && input.method != "POWER"
        && input.method != "INV") {
        throw std::ios_base::failure("Unsupported solver method.");
    }
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
        solver = new InvSolver(reader->getInputData());
    }
    return solver;
}