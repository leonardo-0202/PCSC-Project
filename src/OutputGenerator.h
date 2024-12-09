#ifndef OUTPUT_H
#define OUTPUT_H

#include "OutputData.h"

/** @file OutputGenerator.h
 * @brief Header file for the class handling output data
 */

/**
 * @class OutputGenerator
 * @brief Object for handling a solver's output
 */
class OutputGenerator
{
protected:
    OutputData out; /// Container for solver's output
public:
    /**
     * @brief OutputGenerator constructor
     * @param output OutputData struct containing the solver's outputs
     */
    OutputGenerator(OutputData output);
    /**
     * @brief Function to write data in html format
     * @param vec Eigen::VectorXcd const& containing the eigenvalues from the solver
     * @return std::string containing the html structure
     */
    std::string writeToHTML(const Eigen::VectorXcd & vec);
    /**
     * @brief Function to write eigenvalues to a CSV file
     * @param vec Eigen::VectorXcd const& containing the eigenvalues from the solver
     * @param out_path std::string containing the output file path
     * @return void: Write to a CSV file
     */
    void writeToCSV(const Eigen::VectorXcd & vec, std::string out_path);
    /**
     * @brief Function to save the output of our solver code
     * @return void: Write data to various files
     */
    void saveOutput();
};

#endif //OUTPUT_H