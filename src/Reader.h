#ifndef READER_H
#define READER_H

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <string>

class Reader
{
    private:
        std::string file_name;
    public:
        // constructor for file name
        Reader(std::string file_name);
        Eigen::VectorXd readData();
};

#endif // READER_H