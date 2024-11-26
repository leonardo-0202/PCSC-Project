#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <Eigen/Dense>

/*#include "Reader.h"
#include "InputData.h"*/

/*Reader::Reader(std::string file_name, std::string file_type,
               long num_rows, long num_cols, std::string method,
               long num_iters, double toll)
{

}
*/

Eigen::MatrixXd readMatrixCSV(std::string const& file_name, long const num_rows,
    long num_cols)
{
    Eigen::MatrixXd A(num_rows, num_cols);
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << file_name << std::endl;
        return Eigen::MatrixXd(); // Return an empty matrix for now,
        // can look into throwing exception
    }
    std::string line;
    long row = 0;
    while (std::getline(file, line) && row < num_rows) {
        std::stringstream ss(line);
        std::string cell;
        long col = 0;
        while (getline(ss, cell, ',') && col < num_cols) {
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
    return A;
}
