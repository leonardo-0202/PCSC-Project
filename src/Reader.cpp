#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include <exprtk/exprtk.hpp>
#include "utils.h"
#include "InputData.h"
#include "Reader.h"
#include "Exceptions.h"

/** @file Reader.cpp
 * @brief Implementation file for Reader based classes.
 */

/**
 * @brief Base constructor for the abstract Reader object.
 * @param method The method to be used to find the eigenvalues.
 * @param num_iters The maximum number of iterations.
 * @param tol The tolerance level.
 * @param opt_params A JSON object containing additional optional parameters.
 */
Reader::Reader(std::string const& method, int const& num_iters,
    double const& tol, nlohmann::json const& opt_params)
{
    input_data.method = method;
    input_data.num_iters = num_iters;
    input_data.tol = tol;
    input_data.method_config = opt_params;
}

/**
 * @brief Constructor for FileReader
 * @param method The method to be used to find the eigenvalues.
 * @param num_iters The maximum number of iterations.
 * @param tol The tolerance level.
 * @param opt_params A JSON object containing additional optional parameters.
 * @param path A path for a file containing the input matrix.
 */
FileReader::FileReader(std::string const& method, int const& num_iters, double const& tol,
    nlohmann::json const& opt_params, std::string const& path)
    : Reader(method, num_iters, tol, opt_params)
{
    file_path = path;
}

/**
 * @brief Constructor for FunctionReader
 * @param method The method to be used to find the eigenvalues.
 * @param num_iters The maximum number of iterations.
 * @param tol The tolerance level.
 * @param opt_params A JSON object containing additional optional parameters.
 * @param func The function defining the input matrix.
 * @param size The size of the matrix desired.
 */
FunctionReader::FunctionReader(std::string const& method, int const& num_iters,
    double const& tol, nlohmann::json const& opt_params, std::string const& genFunc, int size)
    : Reader(method, num_iters, tol, opt_params)
{
    func = genFunc;
    input_data.size = size;
}

/**
 * @brief Constructor for PictureReader
 * @param method The method to be used to find the eigenvalues.
 * @param num_iters The maximum number of iterations.
 * @param tol The tolerance level.
 * @param opt_params A JSON object containing additional optional parameters.
 * @param path A file path for the input image.
 */
PictureReader::PictureReader(std::string const& method, int const& num_iters,
    double const& tol, nlohmann::json const& opt_params, std::string const& img_path)
    : Reader(method, num_iters, tol, opt_params)
{
    path = img_path;
}

/**
 * @brief Method to retrieve the input data.
 * @return An InputData object containing the matrix and related parameters.
 */
InputData Reader::getInputData() const
{
    return input_data;
}

/// Display the input matrix in the terminal.
void Reader::printMatrix() const
{
    for(int i=0; i<input_data.size; i++) {
        for(int j=0; j<input_data.size; j++) {
            std::cout << input_data.input_matrix(i,j) << " ";
        }
        std::cout << std::endl;
    }
}

/// Implemented method for FileReader to generate an Eigen::MatrixXcd object
void FileReader::genMatrix()
{
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw ReaderError("Error opening file: " + file_path);
    }
    std::cout << "Opening file: " << file_path << " ..." << std::endl << std::flush;

    input_data.size = 0;
    std::string line;
    int expected_columns = 0;

    // Count number of rows and columns (in the first row)
    while (std::getline(file, line)) {
        if (input_data.size == 0) {
            std::stringstream ss(line);
            std::string cell;
            while (std::getline(ss, cell, ',')) {
                expected_columns++;
            }
        }
        input_data.size++;
    }

    if (input_data.size != expected_columns) {
        throw ReaderError("The matrix is not square. Rows: " + std::to_string(input_data.size) + ", Columns: " + std::to_string(expected_columns));
    }

    file.clear();
    file.seekg(0);

    Eigen::MatrixXcd A(input_data.size, input_data.size);
    long row = 0;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        long col = 0;
        while (getline(ss, cell, ',') && col < input_data.size) {
            try {
                A(row, col) = parseComplex(cell);
            } catch (const std::invalid_argument& e) {
                std::cerr << "FATAL ERRROR: Invalid number at row " << row << ", column " << col << std::endl << std::flush;
                std::exit(EXIT_FAILURE);
            }
            col++;
        }

        // check for column mismatch
        if (col != input_data.size) {
            throw ReaderError("Row " + std::to_string(row) + " does not have " + std::to_string(input_data.size) + " columns.");
        }

        row++;
    }
    file.close();

    input_data.input_matrix = A;
}
/// Implemented method for FunctionReader to generate an Eigen::MatrixXcd object
void FunctionReader::genMatrix()
{
    input_data.size = input_data.method_config;
    Eigen::MatrixXcd A(input_data.size, input_data.size);
    double i, j;

    exprtk::symbol_table<double> symbol_table;
    symbol_table.add_variable("i", i);
    symbol_table.add_variable("j", j);
    exprtk::expression<double> expression;
    expression.register_symbol_table(symbol_table);

    exprtk::parser<double> parser;
    parser.compile(func, expression);

    for(int row = 0; row < input_data.size; row++) {
        for(int col = 0; col<input_data.size; col++) {
            i = row + 1;
            j = col + 1;
            //A(row,col) = expression.value();
            std::cout << A(row,col) << std::endl;
        }
    }
    input_data.input_matrix = A;
}

/// Implemented method for PictureReader to generate an Eigen::MatrixXcd object
void PictureReader::genMatrix() {
    int height, width, channels;
    const char * img_path = path.c_str();
    unsigned char* data = stbi_load(img_path, &height, &width, &channels, 1);
    if (!data) {
        throw ReaderError("Error opening image: " + path);
    }
    if (height != width) {
        std::cerr << "WARNING: height must be equal to width. Cropping." << std::endl;
    }
    std::cout << "Opening image: " << path << std::endl;
    int sz = std::min(height,width);
    Eigen::MatrixXcd A(sz,sz);
    for(int i=0; i<sz; i++) {
        for(int j=0; j<sz; j++) {
            A(i,j) = static_cast<double>(data[i*sz + j]);
        }
    }
    input_data.input_matrix = A;
    std::cout << "Successfully loaded picture as matrix." << std::endl;
    stbi_image_free(data);
}
