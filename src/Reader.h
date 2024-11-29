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
    Reader(std::string const& method, int const& size, int const& num_iters, 
        double const& tol, json const& opt_params);
    InputData getInputData() const;
    virtual void readMatrix() = 0;
};

class FileReader : public Reader
{
protected:
    std::string file_path;
public:
    FileReader(std::string const& path);
    void readMatrix();
};

class FunctionReader : public Reader
{
protected:
    
public:
    void readMatrix();
};

class DiagonalReader : public Reader
{
    void readMatrix();
};

#endif //READER_H

