#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <Eigen/Dense>
#include "Reader.h"
#include "InputData.h"

Reader::Reader(std::string file_name, std::string file_type,
               long num_rows, long num_cols, std::string method,
               long num_iters, double toll)
{

}

Eigen::MatrixXd Reader::readMatrixCSV(std::string file_name, long num_rows,
    long num_cols)
{
    Eigen::MatrixXd A;
    std::ifstream file(file_name);
    if (!file.is_open()) {
        // throw error
    }

    std::string line;
    int row = 0;
    while (getline(file, line) && row < num_rows) {
        std::stringstream ss(line);
        std::string cell;
        int col = 0;
        while (getline(ss, cell, ',') && col < num_cols) {
            A[row][col] = cell;
            col++;
        }
        row++;
    }
}
