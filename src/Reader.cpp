#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <Eigen/Dense>
#include "Reader.h"

void Reader::setParams(std::string const& method, int const& size,
    int const& num_iters, double const& tol)
{
    input_data.method = method;
    input_data.size = size;
    input_data.num_iters = num_iters;
    input_data.tol = tol;
}

InputData Reader::getInputData() const
{
    return input_data;
}

void FileReader::setFilePath(std::string path)
{
    file_path = path;
}

void FileReader::readMatrix()
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
    input_data.input_matrix = A;
}

// can add reading from function, or put vector as diagonal