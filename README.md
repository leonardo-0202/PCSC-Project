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

Once users have altered the configuration file with how they want to use the code, you can compile and execute. 

### Configuration File
In this section we will give all details about the configuration file and how a user can interact with it. The file can be effectively split into three parts:
- Basic configuration used for every run
- Input type specific configuration
- Method specific configuration 

#### Basic Configuration
This is the part of the file that is used everytime a solver is run, no matter the specific methodology used. Here is a non-exhaustive list of the parameters that the user must interact with upon use:
- "MAX_ITERS"
- "TOLERANCE"
- "INPUT"
- "METHOD"

Every solver implemented in our code is an iterative method, thus `MAX_ITERS` expects an integer value signifying the maximum number of iterations for the method. 

`TOLERANCE` expects a real number and is used to meaure convergence. 

`INPUT` allows the user to specify how they want to provide an input matrix. Our current implementation supports three input types:
- "FILE": a text file containing the full matrix
- "FUNCTION": a function for how to build the matrix
- "PICTURE": an image that will be read as gray-scale

`METHOD` allows the user to specify which type of solver they want to use for finding eigenvalues. Our current implementation supports three method types:
- "POWER": finds the largest magnitude eigenvalue
- "INVERSE": finds the smallest magnitude eigenvalue
- "QR": finds all eigenvalues

`POWER` and `INVERSE` methods allow support for applying shifts to the input matrix. This allows you to find different eigenvalues than the default methods seek out.

Based on the values given under the `INPUT` and `METHOD` keys, there are specific nested objects for each option. The code works by accessing the nested objects corresponding to the user-provided `INPUT` and `METHOD` values. Inside these nested objects are specific parameters for the type. 

#### Input Type Configuration
Each `INPUT` type has a specific configuration. 

`FILE`: 
- "PATH": string containing a path to the input matrix file

`FUNCTION`:
- "FUNC": string containing an expression with row *i* and column *j*
- "SIZE": integer specifying how large of a matrix to create

`PICTURE`:
- "PATH": string containing a path to the input image

#### Method Specific Configuration
`METHOD` types can have a specific configuration as well.

`POWER`:
- "SHIFT": a complex number in the format "5 + 3i" specifying the shift to apply to the input matrix

`INVERSE`:
- "SHIFT": a complex number in the format "5 + 3i" specifying the shift to apply to the input matrix

The QR method does not have any specific configuration. Future developers can easily add the configuration for a new method using a unique nested object.

## Code Layout

## Testing

## Future Work

