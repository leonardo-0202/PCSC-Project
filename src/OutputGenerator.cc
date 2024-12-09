#include <iostream>
#include <fstream>
#include <string>
#include "Eigen/Dense"
#include "OutputGenerator.h"

/** @file OutputGenerator.cc
 * @brief Implementation file for the class handling output data
 */

/**
 * @brief OutputGenerator constructor
 * @param output OutputData struct containing the solver's outputs
 * 
 * Initialize OutputData class member with OutputData constructed from solver.
 */
OutputGenerator::OutputGenerator(OutputData output) : out(output) {}

/**
 * @brief Function to write data in html format
 * @param vec Eigen::VectorXcd const& containing the eigenvalues from the solver
 * @return std::string containing the html structure
 * 
 * Creates an HTML formatted string for displaying eigenvalues.
 */
std::string OutputGenerator::writeToHTML(const Eigen::VectorXcd & vec) {
    std::string html = "<p style=\"text-align: center;\">";
    for (int i = 0; i < vec.size(); ++i) {
        html += "λ" + std::to_string(i + 1) + " = "
            + std::to_string(vec[i].real()) + " + i" + std::to_string(vec[i].imag());
        if (i < vec.size() - 1) {
            html += ", ";
        }
    }
    html += "</p>";
    return html;
}

/**
 * @brief Function to write eigenvalues to a CSV file
 * @param vec Eigen::VectorXcd const& containing the eigenvalues from the solver
 * @param out_path std::string containing the output file path
 * @return void: Write to a CSV file
 */
void OutputGenerator::writeToCSV(const Eigen::VectorXcd & vec, std::string out_path) {
    std::ofstream out_file(out_path, std::ios::out);
    if (!out_file) {
        std::cerr << "WARNING: Could not open the file for writing eigenvalues. Printing to terminal instead." << std::endl;
        std::cout << vec << std::endl;
        return;
    }
    for (int i = 0; i < vec.size(); ++i) {
        out_file << vec[i].real() << " " << vec[i].imag() << "\n";
    }
    out_file.close();
}

/**
 * @brief Function to save the output of our solver code
 * @return void
 * 
 * Write eigenvalues to a CSV file and solver metrics to an HTML file.
 */
void OutputGenerator::saveOutput() {
    std::cout << "Saving output file..." << std::endl;

    std::ofstream htmlFile("../output.html");
    std::string output_path = "../output.csv";

    if (!htmlFile) {
        std::cerr << "WARNING: Could not open the file for writing metrics. Printing to terminal instead." << std::endl;
        std::cout << "Method: " + out.method << std::endl;
        std::cout << "Error: " + std::to_string(out.estimated_error) << std::endl;
        std::cout << "Execution Time: " + std::to_string(out.execution_time) + " microseconds" << std::endl;
        std::cout << "Iterations: " + std::to_string(out.iterations) << std::endl;
        return;
    }

    writeToCSV(out.estimated_eigenvalues, output_path);
    std::cout << "Eigenvalue output saved to output.csv" << std::endl;
    
    htmlFile << R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Metrics Report</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 20px;
            line-height: 1.6;
        }
        h2 {
            text-align: center;
            color: #333;
        }
        table {
            width: 80%;
            margin: 20px auto;
            border-collapse: collapse;
            box-shadow: 0 2px 5px rgba(0, 0, 0, 0.2);
        }
        th, td {
            padding: 10px 15px;
            text-align: center;
            border: 1px solid #ddd;
        }
        th {
            background-color: #f4f4f4;
            font-weight: bold;
        }
        tr:nth-child(even) {
            background-color: #f9f9f9;
        }
        img {
            display: block;
            margin: 20px auto;
            max-width: 100%;
            height: auto;
        }
        p {
            text-align: center;
            color: #555;
            font-size: 14px;
        }
    </style>
</head>
<body>
    <h2>Metrics Report</h2>
    <table>
        <tr>
            <th>Metric</th>
            <th>Value</th>
        </tr>
)";

    htmlFile << "<tr><td>Method</td><td>" + out.method + "</td></tr>";
    htmlFile << "<tr><td>Estimated Error</td><td>" + std::to_string(out.estimated_error) + "</td></tr>";
    htmlFile << "<tr><td>Execution Time</td><td>" + std::to_string(out.execution_time) + " microseconds</td></tr>";
    htmlFile << "<tr><td>Iterations</td><td>" + std::to_string(out.iterations) + "</td></tr>";

    htmlFile << R"(
    </table>
    <h2>Plot</h2>
    <img src="eigenvalues_plot.png" alt="Eigenvalues Plot">
</body>
</html>
)";

    htmlFile.close();
    std::cout << "Solver metrics output saved to output.html" << std::endl;
}
