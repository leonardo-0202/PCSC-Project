#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
//#include <exprtk/exprtk.hpp>
#include "InputData.h"
#include "Reader.h"

Reader::Reader(std::string const& method, int const& size, int const& num_iters,
    double const& tol, nlohmann::json const& opt_params)
{
    input_data.method = method;
    input_data.size = size;
    input_data.num_iters = num_iters;
    input_data.tol = tol;
    input_data.method_config = opt_params;
}

FileReader::FileReader(std::string const& method, int const& size,
    int const& num_iters, double const& tol, nlohmann::json const& opt_params,
    std::string const& path)
    : Reader(method, size, num_iters, tol, opt_params)
{
    file_path = path;
}

FunctionReader::FunctionReader(std::string const& method, int const& size,
    int const& num_iters, double const& tol, nlohmann::json const& opt_params,
    std::string const& genFunc)
    : Reader(method, size, num_iters, tol, opt_params)
{
    func = genFunc;
}

PictureReader::PictureReader(std::string const& method, int const& size,
    int const& num_iters, double const& tol, nlohmann::json const& opt_params,
    std::string const& img_path)
    : Reader(method, size, num_iters, tol, opt_params)
{
    path = img_path;
}

InputData Reader::getInputData() const
{
    return input_data;
}

void Reader::printMatrix() const
{
    for(int i=0; i<input_data.size; i++) {
        for(int j=0; j<input_data.size; j++) {
            std::cout << input_data.input_matrix(i,j) << " ";
        }
        std::cout << std::endl;
    }
}

void FileReader::genMatrix()
{
    Eigen::MatrixXcd A(input_data.size, input_data.size);
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << file_path << std::endl;
        A =  Eigen::MatrixXcd();
        return;
        // return an empty matrix for now
        // can look into throwing exception
    }
    std::cout << "Opening file: " << file_path << " ..." << std::endl;
    std::string line;
    long row = 0;
    while (std::getline(file, line) && row < input_data.size) {
        std::stringstream ss(line);
        std::string cell;
        long col = 0;
        while (getline(ss, cell, ',') && col < input_data.size) {
            try {
                A(row, col) = parseComplex(cell);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid number at row " << row << ", column " << col << std::endl;
                A(row, col) = -10e9;
            }
            col++;
        }
        row++;
    }
    file.close();
    
    input_data.input_matrix = A;
}

// parses complex numbers of form a + bi
std::complex<double> FileReader::parseComplex(std::string s)
{
    s.erase(remove_if(s.begin(), s.end(), isspace), s.end());
    int delim_pos = s.find('i');
    double real, imm;

    if (delim_pos == std::string::npos) {
        // No imaginary part, parse as a real number
        real = std::stod(s);
        return std::complex<double>(real, 0);
    }

    int plus_pos = s.find_last_of('+');
    int min_pos = s.find_last_of('-');
    if (plus_pos != std::string::npos){
        real = std::stod(s.substr(0,plus_pos));
        imm = std::stod(s.substr(plus_pos, delim_pos-1));
    }
    else if (min_pos != std::string::npos && min_pos > 0) {
        real = std::stod(s.substr(0, min_pos));
        imm = std::stod(s.substr(min_pos, delim_pos-1));
    }
    else {
        real = 0;
        imm = std::stod(s.substr(0,delim_pos));
    }

    return std::complex<double>(real, imm);
}

void FunctionReader::genMatrix()
{
    Eigen::MatrixXcd A(input_data.size, input_data.size);
    double i, j;

    /*
    exprtk::symbol_table<double> symbol_table;
    symbol_table.add_variable("i", i);
    symbol_table.add_variable("j", j);
    exprtk::expression<double> expression;
    expression.register_symbol_table(symbol_table);

    exprtk::parser<double> parser;
    parser.compile(func, expression);
    */

    for(int row = 0; row < input_data.size; row++) {
        for(int col = 0; col<input_data.size; col++) {
            i = row + 1;
            j = col + 1;
            //A(row,col) = expression.value();
            std::cout << A(row,col) << std::endl;
        }
    }
    input_data.input_matrix = A;
}

void PictureReader::genMatrix() {
    int height, width, channels;
    const char * img_path = path.c_str();
    unsigned char* data = stbi_load(img_path, &height, &width, &channels, 1);
    if (!data) {
        std::cerr << "Error: Unable to load image at " << img_path << std::endl;
    }
    if (height != width) {
        std::cerr << "Warning: height must be equal to width. Cropping." << std::endl;
    }
    int sz = std::min(height,width);
    Eigen::MatrixXcd A(sz,sz);
    for(int i=0; i<sz; i++) {
        for(int j=0; j<sz; j++) {
            A(i,j) = static_cast<double>(data[i*sz + j]);
        }
    }
    input_data.input_matrix = A;
    stbi_image_free(data);
}
