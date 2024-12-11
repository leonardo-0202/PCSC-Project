#ifndef READER_H
#define READER_H
#include <string>
#include "InputData.h"
#include <nlohmann/json.hpp>

/**
* @file Reader.h
* @brief This header file contains the definition
* of the Reader class and its derived classes. These classes
* are used to read the input matrix from various sources.
*/

/**
 * @class Reader
 * @brief Abstract base Reader class, for reading and generating input matrices.
 */
class Reader
{
protected:
    /// Holds the input data
    InputData input_data;
public:
    /**
     * @brief Base constructor for the abstract Reader object.
     * @param method The method to be used to find the eigenvalues.
     * @param num_iters The maximum number of iterations.
     * @param tol The tolerance level.
     * @param opt_params A JSON object containing additional optional parameters.
     */
    Reader(std::string const& method, int const& num_iters,
        double const& tol, nlohmann::json const& opt_params);
    /**
     * @brief Method to retrieve the input data.
     * @return An InputData object containing the matrix and related parameters.
     */
    InputData getInputData() const;
    /// Display matrix
    void printMatrix() const;
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
    /// Path to the file containing the matrix data
    std::string file_path;
public:
    /**
     * @brief Constructor for FileReader
     * @param method The method to be used to find the eigenvalues.
     * @param num_iters The maximum number of iterations.
     * @param tol The tolerance level.
     * @param opt_params A JSON object containing additional optional parameters.
     * @param path A path for a file containing the input matrix.
     */
    FileReader(std::string const& method, int const& num_iters,
        double const& tol, nlohmann::json const& opt_params, std::string const& path);
    /// Implemented method to generate an Eigen::MatrixXcd object
    void genMatrix();
};

/**
 * @class FunctionReader
 * @brief Derived class that creates the input matrix from a user-defined function.
 */
class FunctionReader : public Reader
{
protected:
    /// Function definition
    std::string func;
public:
    /**
     * @brief Constructor for FunctionReader
     * @param method The method to be used to find the eigenvalues.
     * @param num_iters The maximum number of iterations.
     * @param tol The tolerance level.
     * @param opt_params A JSON object containing additional optional parameters.
     * @param func The function defining the input matrix.
     * @param size The size of the matrix desired.
     */
    FunctionReader(std::string const& method, int const& num_iters, double const& tol,
        nlohmann::json const& opt_params, std::string const& func, int const& size);
    /// Implemented method to generate an Eigen::MatrixXcd object
    void genMatrix();
};

/**
 * @class PictureReader
 * @brief Derived class that creates the input matrix from an image.
 */
class PictureReader : public Reader
{
protected:
    /// Path for image
    std::string path;
public:
    /**
     * @brief Constructor for PictureReader
     * @param method The method to be used to find the eigenvalues.
     * @param num_iters The maximum number of iterations.
     * @param tol The tolerance level.
     * @param opt_params A JSON object containing additional optional parameters.
     * @param path A file path for the input image.
     */
    PictureReader(std::string const& method, int const& num_iters,
        double const& tol, nlohmann::json const& opt_params, std::string const& path);
    /// Implemented method to generate an Eigen::MatrixXcd object
    void genMatrix();
};

#endif //READER_H

