#ifndef OUTPUT_H
#define OUTPUT_H

#include "OutputData.h"

class OutputGenerator
{
protected:
    OutputData out;
public:
    OutputGenerator(OutputData output);
    std::string writeToHTML(const Eigen::VectorXcd & vec);
    void writeToCSV(const Eigen::VectorXcd & vec, std::string out_path);
    std::string warningHTML(int err);
    void saveOutput();
};

#endif //OUTPUT_H