#ifndef UTILS_H
#define UTILS_H

#include "Reader.h"
#include "Solver.h"

Reader * createReader(std::filesystem::path config_path);
Solver * createSolver(Reader * reader);
std::complex<double> parseComplex(std::string s);

#endif //UTILS_H
