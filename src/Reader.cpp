#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>
//#include <exprtk/exprtk.hpp>
#include "Reader.h"

Reader::Reader(std::string const& method, int const& size, int const& num_iters,
    double const& tol, nlohmann::json const& opt_params)
{
    input_data.method = method;
    input_data.size = size;
    input_data.num_iters = num_iters;
    input_data.tol = tol;
    input_data.method_config = opt_params;
}

FileReader::FileReader(std::string const& method, int const& size,
    int const& num_iters, double const& tol, nlohmann::json const& opt_params,
    std::string const& path)
    : Reader(method, size, num_iters, tol, opt_params)
{
    file_path = path;
}

FunctionReader::FunctionReader(std::string const& method, int const& size,
    int const& num_iters, double const& tol, nlohmann::json const& opt_params,
    std::string const& genFunc)
    : Reader(method, size, num_iters, tol, opt_params)
{
    func = genFunc;
}

InputData Reader::getInputData() const
{
    return input_data;
}

void FileReader::genMatrix()
{
    Eigen::MatrixXd A(input_data.size, input_data.size);
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << file_path << std::endl;
        A =  Eigen::MatrixXd();
        return;
        // return an empty matrix for now
        // can look into throwing exception
    }
    std::cout << "Opening file: " << file_path << " ..." << std::endl;
    std::string line;
    long row = 0;
    while (std::getline(file, line) && row < input_data.size) {
        std::stringstream ss(line);
        std::string cell;
        long col = 0;
        while (getline(ss, cell, ',') && col < input_data.size) {
            try {
                A(row, col) = std::stod(cell);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid number at row " << row << ", column " << col << std::endl;
                A(row, col) = -10e9;
            }
            col++;
        }
        row++;
    }
    file.close();
    
    input_data.input_matrix = A;
}

void FunctionReader::genMatrix()
{
    Eigen::MatrixXd A(input_data.size, input_data.size);
    double i, j;

    /*
    exprtk::symbol_table<double> symbol_table;
    symbol_table.add_variable("i", i);
    symbol_table.add_variable("j", j);
    exprtk::expression<double> expression;
    expression.register_symbol_table(symbol_table);

    exprtk::parser<double> parser;
    parser.compile(func, expression);
    */

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
