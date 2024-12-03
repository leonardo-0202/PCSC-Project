/**
* @file Reader.h
* @brief This header file will contain the definition
* of the Reader class and its derived classes. These classes
* are used to read the input matrix from various sources.
*/

#ifndef READER_H
#define READER_H
#include <string>
#include "InputData.h"
#include <nlohmann/json.hpp>

/**
 * @class Reader
 * @brief Abstract base Reader class, for reading and generating input matrices.
 */
class Reader
{
protected:
    /**
     * @brief Holds the input data
     */
    InputData input_data;
public:
    /**
     * @brief Constructs a Reader object.
     * @param method The method to be used to find the eigenvalues.
     * @param size The size of the matrix.
     * @param num_iters The maximum number of iterations.
     * @param tol The tolerance level.
     * @param opt_params A JSON object containing additional optional parameters.
     */
    Reader(std::string const& method, int const& size, int const& num_iters, 
        double const& tol, nlohmann::json const& opt_params);
    /**
     * @brief Retrieves the input data.
     * @return An InputData object containing the matrix and related parameters.
     */
    InputData getInputData() const;
    /**
     * @brief Pure virtual function to generate the matrix.
     *        Must be implemented by derived classes.
     */
    virtual void genMatrix() = 0;
};

/**
 * @class FileReader
 * @brief Derived class that reads the input matrix from a file.
 */
class FileReader : public Reader
{
protected:
    /**
     * @brief Path to the file containing the matrix data.
     */
    std::string file_path;
public:
    FileReader(std::string const& method, int const& size, int const& num_iters,
        double const& tol, nlohmann::json const& opt_params, std::string const& path);
    void genMatrix();
};

class FunctionReader : public Reader
{
protected:
    std::string func;
public:
    FunctionReader(std::string const& method, int const& size, int const& num_iters,
        double const& tol, nlohmann::json const& opt_params, std::string const& func);
    void genMatrix();
};

class PictureReader : public Reader
{
protected:
    std::string path;
public:
    PictureReader(std::string const& method, int const& size, int const& num_iters,
        double const& tol, nlohmann::json const& opt_params, std::string const& path);
    void genMatrix();
};

#endif //READER_H

