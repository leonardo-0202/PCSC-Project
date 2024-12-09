#include <fstream>
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <string>
#include <nlohmann/json.hpp>
#include "Reader.h"
#include "Solver.h"
using json = nlohmann::json;

Reader* createReader(std::filesystem::path file_path)
{
    // Open input stream object and check existence
    std::cout << "Opening file ... " << std::filesystem::absolute(file_path) << std::endl << std::flush;
    std::ifstream config_file(file_path);
    if (!config_file.is_open()) {
        throw std::ios_base::failure("Failed to open config. file." );
    }

    // Create json object
    json data = json::parse(config_file);
    config_file.close();
    
    // Extract run configuration

    std::string data_type;
    try {
        data_type = data.at("INPUT");
    }
    catch (const std::out_of_range& e) {
        std::cerr <<"FATAL ERROR: Missing input data type." << std::endl << std::flush;
        std::exit(EXIT_FAILURE);
    }

    std::string method = data.at("METHOD");
    try {
        method = data.at("METHOD");
    }
    catch (const std::out_of_range& e) {
        std::cerr <<"WARNING: Missing solver method. Defauting to QR method." << std::endl << std::flush;
        method = "QR";
    }

    int max_iters;
    try {
        max_iters=  data.at("MAX_ITERS");
    }
    catch (const std::out_of_range& e) {
        std::cerr <<"WARNING: Missing max. num. of iterations. Defauting to 1000." << std::endl << std::flush;
        max_iters = 1000;
    }

    double tol;
    try {
        tol=  data.at("TOLERANCE");
    }
    catch (const std::out_of_range& e) {
        std::cerr <<"WARNING: Missing tolerance. Defauting to 1e-9." << std::endl << std::flush;
        tol = 1e-9;
    }

    json opt_params = data.at(method);

    // Exception when valid option not given
    if (data_type != "FILE" && data_type != "FUNCTION"
        && data_type != "PICTURE") {
        throw std::ios_base::failure("Unsupported data type. Input data needs to be either a file, a picture. or a function");
        std::exit(EXIT_FAILURE);
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

// parses complex numbers of form a + ib
std::complex<double> parseComplex(std::string s)
{
    s.erase(remove_if(s.begin(), s.end(), isspace), s.end());
    int delim_pos = s.find('i');
    double real, imm;

    if (delim_pos == std::string::npos) {
        // No imaginary part, parse as a real number
        real = std::stod(s);

        return std::complex<double>(real, 0);
    }

    int plus_pos = s.find_last_of('+');
    int min_pos = s.find_last_of('-');
    if (plus_pos != std::string::npos){
        real = std::stod(s.substr(0,plus_pos));
        imm = std::stod(s.substr(plus_pos, delim_pos-1));
    }
    else if (min_pos != std::string::npos && min_pos > 0) {
        real = std::stod(s.substr(0, min_pos));
        imm = std::stod(s.substr(min_pos, delim_pos-1));
    }
    else {
        real = 0;
        imm = std::stod(s.substr(0,delim_pos));
    }
    return std::complex<double>(real, imm);
}
