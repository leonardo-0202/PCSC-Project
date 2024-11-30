#ifndef UTILS_H
#define UTILS_H

#include "Reader.h"
#include "Solver.h"

Reader * createReader(std::string const& config_path);
Solver * createSolver(Reader * reader);

#endif //UTILS_H
