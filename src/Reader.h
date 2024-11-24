#ifndef READER_H
#define READER_H

#include <string>
#include "InputData.h"

class Reader
{
private:
    InputData input_data;
public:
    Reader(std::string file_name, std::string file_type,
        long num_rows, long num_cols, std::string method,
        long num_iters, double toll);
    Eigen::MatrixXd readMatrixCSV(std::string file_name, long num_rows, long num_cols);
    Eigen::MatrixXd readMatrixBinary(std::string file_name);
    InputData getInputData();
};

#endif //READER_H

