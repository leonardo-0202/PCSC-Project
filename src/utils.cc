//
// Created by giova on 27/11/2024.
//
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "Reader.h"


Reader* createReader(std::string const& config_path)
{
    std::ifstream configFile(config_path);
    if (!configFile.is_open()) {
        std::cerr << "Error: Unable to open configuration file." << std::endl;
        return nullptr;
    }

    std::string data_type, method;
    int size, num_iters;
    double tol;
    configFile >> data_type;
    configFile >> method;
    configFile >> size;
    configFile >> num_iters;
    configFile >> tol;

    if (data_type != "FILE" && data_type != "FUNCTION"
        && data_type != "DIAG") {
        std::cerr << "Error: Unsupported data type" << std::endl;
        return nullptr;
    }

    if (data_type == "FILE") {
        FileReader * file_reader = new FileReader();

        std::string file_path;
        configFile >> file_path;

        file_reader->setParams(method, size, num_iters, tol);
        file_reader->setFilePath(file_path);
        file_reader->readMatrix();

        configFile.close();
        return file_reader;
    }
    else if (data_type == "FUNCTION") {
        FunctionReader * function_reader = new FunctionReader();

    }
}