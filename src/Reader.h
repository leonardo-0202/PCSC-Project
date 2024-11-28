#ifndef READER_H
#define READER_H
#include <string>
#include <Eigen/Dense>
#include "InputData.h"

class Reader
{
protected:
    InputData input_data;
public:
<<<<<<< HEAD
    Reader(std::string file_name, std::string file_type,
        long num_rows, long num_cols, std::string method,
        long num_iters, double toll);
    // WE SHOULD ADD A CHECK SOMEWHERE THAT THE INPUT MATRIX IS SQUARE
    Eigen::MatrixXd readMatrixCSV(std::string file_name, long num_rows, long num_cols);
    Eigen::MatrixXd readMatrixBinary(std::string file_name);
    InputData getInputData();
=======
    void setParams(std::string const& method, int const&  size,
        int const& num_iters, double const&  tol);
    InputData getInputData() const;
    virtual void readMatrix() = 0;
};

class FileReader : public Reader
{
protected:
    std::string file_path;
public:
    void setFilePath(std::string path);
    void readMatrix();
};

class FunctionReader : public Reader
{
    void readMatrix();
};

class DiagonalReader : public Reader
{
    void readMatrix();
>>>>>>> e626d77faf4809bd91bca7472072c8076099e0e1
};

#endif //READER_H

