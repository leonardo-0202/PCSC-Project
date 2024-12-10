#ifndef UTILS_H
#define UTILS_H

#include "Reader.h"
#include "Solver.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

/** @file utils.h
 * @brief Header file for utility functions
 */

/** 
 * @brief Create Reader object based off of a configuration file.
 * @param config_path 'std::filesystem::path' object for the path to a run's configuration file. 
 * @return Reader object which points to a dynamically allocated daughter of Reader class
 */
Reader * createReader(std::filesystem::path config_path);

/**
 * @brief Create a Solver object based off of Reader object provided
 * @param reader Reader object that contains InputData needed for solver
 * @return Solver object which points to a dynamically allocated daughter of Solver class
 */
Solver * createSolver(Reader * reader);

/**
 * @brief Set default solving parameters in case they weren't provided
 * @param method String that specifies the solving method
 * @param opt_params Json object that contains the optional parameters to set
 */
void setDefaultVals(std::string method, json &opt_params);

/**
 * @brief Create complex number representation of a string in the format "a + bi"
 * @param s 'std::string' in the format: real number + imag number * i
 * @return 'std::complex<double>' container with the complex number
 */
std::complex<double> parseComplex(std::string s);

/**
 * @brief Tries to call the genMatrix method, handling exceptions
 * @param reader 'Reader *' Reader object to try
 */
void tryGenMatrix(Reader * reader);

/**
 * @brief Helper function to set a missing parameter to fixed default one
 * @param data 'const json&' The json config file
 * @param key 'const std::string&' The parameter to check
 * @param warn_msg 'const std::string' Warning message to print in case of missing parameter
 * @param default_value 'T&' The default value
 * @return 'T' Default value to set
 */
template<typename T>
T getJsonValueOptional(const json& data, const std::string& key, const std::string warn_msg,
    const T& default_value) {
 try {
  return data.at(key).get<T>();
 } catch (const std::exception&) {
  std::cerr << warn_msg << std::endl;
  return default_value;
 }
};

/**
 * @brief Helper function to check for necessary missing parameters
 * @param data 'const json&' The json config file
 * @param key 'const std::string&' The parameter to check
 * @param error 'const std::exception' Exception to throw in case of missing parameter
 */
template<typename T, typename E>
T getJsonValueNecessary(const json& data, const std::string& key,
    const std::string & err_msg) {
 try {
  return data.at(key).get<T>();
 } catch (const std::exception&) {
  throw E(err_msg);
 }
};

#endif //UTILS_H
