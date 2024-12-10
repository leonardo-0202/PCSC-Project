# Project 1: Eigenvalue Problems
 This document provides useful information for interacting with our eigenvalue solver code. We provide documentation for compiling, interacting, and the usage of our code.

## Table of Contents
1. [Compilation](#compilation)
2. [Program Usage](#program-usage)
    - [Program Execution](#program-execution)
    - [Configuration File](#configuration-file)
3. [Code Layout](#code-layout)
4. [Testing](#testing)
5. [Future Work](#future-work)

## Compilation
Compilation of the code is done through CMake. In our repository we have two submodules:
 - eigen
 - googletest

In order to compile the code, you should first populate the submodules.

```
git submodule update --init
```

Then, building is done as usual, e.g. with CLion or in the terminal:

```
mkdir build
cd build
cmake ..
make
```

The compilation will make an executable file called `main`. 

## Program Usage
In this section we will explore the general flow of the program's execution and how to work with the configuration file.

### Program Execution 
To interact with the code, you must work with the configuration file `config.json`. By default this file is located in the root of the repository. Our code will access the configuration file in that directory by default. Instead, users can pass a custom path to the file when executing the code.

```
./main relative_path_to_config
```

The configuration file allows a user to interact with the program by assigning values to a default JSON structure. Common parameters across different eigenvalue solvers, like the number of iterations and tolerance, will be default keys at the outermost level in the JSON object. Users can control the solver desired and the type of input provided in this configuration file. 

Once users have inputted in the configuration file how they want to use the code, you can compile and execute. 

### Configuration File

## Code Layout

## Testing

## Future Work

