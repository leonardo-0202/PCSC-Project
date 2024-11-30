#ifndef OUTPUT_H
#define OUTPUT_H

#include "OutputData.h"

class OutputGenerator
{
protected:
    OutputData out;
public:
    OutputGenerator(OutputData output);
    void saveOutput();
};

#endif //OUTPUT_H