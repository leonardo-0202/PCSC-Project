#include <fstream>
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <string>
#include <nlohmann/json.hpp>
#include "Exceptions.h"
#include "utils.h"
#include "Reader.h"
#include "Solver.h"

/** @file utils.cc
 * @brief Implementation file for utility functions.
 */

using json = nlohmann::json;

/** 
 * @brief Create Reader object based off of a configuration file.
 * @param file_path 'std::filesystem::path' object for the path to a run's configuration file. 
 * @return Reader object which points to a dynamically allocated daughter of Reader class
 */
Reader* createReader(std::filesystem::path file_path)
{
    // Open input stream object and check existence
    std::cout << "Opening file ... " << std::filesystem::absolute(file_path) << std::endl << std::flush;
    std::ifstream config_file(file_path);
    if (!config_file.is_open()) {
        throw ConfigError("ERROR: Config. file not found.");
    }

    // Create json object
    json data = json::parse(config_file);
    config_file.close();

    // Extract run configuration
    std::string data_type;
    data_type = getJsonValueNecessary<std::string, ConfigError>(data, "INPUT",
        "ERROR: Missing input data type.");

    std::string method;
    method = getJsonValueOptional<std::string>(data, "METHOD",
        "WARNING: Missing solver method. Defaulting to QR method.","QR");

    int max_iters;
    max_iters = getJsonValueOptional<int>(data, "MAX_ITERS",
        "WARNING: Missing maximum number of iterations. Defaulting to 1000.", 1000);

    double tol;
    tol = getJsonValueOptional<double>(data, "TOLERANCE",
        "WARNING: Missing tolerance. Defaulting to 1e-9.", 1e-9);

    // Method-specific parameters. If missing, use default ones.
    json opt_params;
    try {
        data.at(method);
    }
    catch(const std::exception &e) {
        std::cerr << "Missing solver parameters. Using default values." << std::endl << std::flush;
        setDefaultVals(method, opt_params);
    }

    // Exception when valid option not given
    std::vector<std::string> supported_data_types = {"FILE", "FUNCTION", "PICTURE"};
    std::vector<std::string> supported_methods = {"QR", "POWER", "INV"};
    if (std::find(supported_data_types.begin(), supported_data_types.end(), data_type) == supported_data_types.end()) {
        throw ConfigError("Unsupported data type. Input data needs to be either a file, a picture. or a function");
    }
    if (std::find(supported_methods.begin(), supported_methods.end(), method) == supported_methods.end()) {
        std::cerr <<"WARNING: Unsupported solver method. Defaulting to QR method." << std::endl << std::flush;
        method = "QR";
    }

    // Adjust parameters
    if (max_iters < 0) {
        std::cerr <<"WARNING: Invalid max_iters. Defaulting to 100." << std::endl << std::flush;
        max_iters = 100;
    }
    if (tol <= 1e-16) {
        std::cerr <<"WARNING: Too small tolerance. Defaulting to 1e-16." << std::endl << std::flush;
        tol = 1e-16;
    }

    // Creating reader depending on the type of input data
    if (data_type == "FILE") {
        std::string file_path;
        file_path = getJsonValueNecessary<std::string, ConfigError>(data["FILE"], "PATH",
            "ERROR: Missing input data file path.");

        Reader *file_reader = new FileReader(method, max_iters, tol,
            opt_params, file_path);
        tryGenMatrix(file_reader);
        return file_reader;
    }
    else if (data_type == "FUNCTION") {
        std::string func;
        int size;
        func = getJsonValueNecessary<std::string, ConfigError>(data["FUNCTION"], "FUNC",
            "ERROR: Missing matrix generating function.");
        size = getJsonValueNecessary<int, ConfigError>(data["FUNCTION"], "SIZE",
            "ERROR: Missing matrix size.");
        FunctionReader * function_reader = new FunctionReader(method, max_iters, tol,
            opt_params, func, size);
        tryGenMatrix(function_reader);
        return function_reader;
    }
    else if (data_type == "PICTURE") {
        std::string picture_path;
        picture_path = getJsonValueNecessary<std::string, ConfigError>(data["PICTURE"], "PATH",
            "ERROR: Missing picture path.");

        PictureReader * picture_reader = new PictureReader(method, max_iters, tol,
            opt_params, picture_path);
        tryGenMatrix(picture_reader);
        return picture_reader;
    }
}

/**
 * @brief Create a Solver object based off of Reader object provided
 * @param reader Reader object that contains InputData needed for solver
 * @return Solver object which points to a dynamically allocated daughter of Solver class
 */
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

/**
 * @brief Set default solving parameters in case they weren't provided
 * @param method String that specifies the solving method
 * @param opt_params Json object that contains the optional parameters to set
 */
void setDefaultVals(std::string method, json &opt_params) {
    if (method == "QR") {
        opt_params = json(); // QR takes no params
    }
    else if (method == "POWER") {
        opt_params["SHIFT"] = 0;
    }
    else if (method == "INVERSE") {
        opt_params["SHIFT"] = 0;
    }
}

/**
 * @brief Create complex number representation of a string in the format "a + bi"
 * @param s 'std::string' in the format: real number + imag number * i
 * @return 'std::complex<double>' container with the complex number
 */
std::complex<double> parseComplex(std::string s)
{
    s.erase(remove_if(s.begin(), s.end(), isspace), s.end());
    int delim_pos = s.find('i');
    double real, imm;


    // No imaginary part, parse as a real number
    if (delim_pos == std::string::npos) {
        real = std::stod(s);
        return std::complex<double>(real, 0);
    }
    // Some edge cases (i "means" 1i)
    if (s == "i") {
        return std::complex<double>(0.0, 1.0);
    } else if (s == "-i") {
        return std::complex<double>(0.0, -1.0);
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

/**
 * @brief Tries to call the genMatrix method, handling exceptions
 * @param reader 'Reader *' Reader object to try
 */
void tryGenMatrix(Reader *reader) {
    try {
        reader->genMatrix();
    }
    catch (const ReaderError& e) {
        throw;
    }
}


