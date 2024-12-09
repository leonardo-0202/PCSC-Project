#ifndef UTILS_H
#define UTILS_H

#include "Reader.h"
#include "Solver.h"

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
 * @brief Create complex number representation of a string in the format "a + bi"
 * @param s 'std::string' in the format: real number + imag number * i
 * @return 'std::complex<double>' container with the complex number
 */
std::complex<double> parseComplex(std::string s);

#endif //UTILS_H
