# Install script for directory: C:/Users/giova/Documents/GitHub/pcsc-project-template/eigen/unsupported/Eigen

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/PCSC-project")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/Program Files/JetBrains/CLion 2024.2.1/bin/mingw/bin/objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Devel" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/unsupported/Eigen" TYPE FILE FILES
    "C:/Users/giova/Documents/GitHub/pcsc-project-template/eigen/unsupported/Eigen/AdolcForward"
    "C:/Users/giova/Documents/GitHub/pcsc-project-template/eigen/unsupported/Eigen/AlignedVector3"
    "C:/Users/giova/Documents/GitHub/pcsc-project-template/eigen/unsupported/Eigen/ArpackSupport"
    "C:/Users/giova/Documents/GitHub/pcsc-project-template/eigen/unsupported/Eigen/AutoDiff"
    "C:/Users/giova/Documents/GitHub/pcsc-project-template/eigen/unsupported/Eigen/BVH"
    "C:/Users/giova/Documents/GitHub/pcsc-project-template/eigen/unsupported/Eigen/EulerAngles"
    "C:/Users/giova/Documents/GitHub/pcsc-project-template/eigen/unsupported/Eigen/FFT"
    "C:/Users/giova/Documents/GitHub/pcsc-project-template/eigen/unsupported/Eigen/IterativeSolvers"
    "C:/Users/giova/Documents/GitHub/pcsc-project-template/eigen/unsupported/Eigen/KroneckerProduct"
    "C:/Users/giova/Documents/GitHub/pcsc-project-template/eigen/unsupported/Eigen/LevenbergMarquardt"
    "C:/Users/giova/Documents/GitHub/pcsc-project-template/eigen/unsupported/Eigen/MatrixFunctions"
    "C:/Users/giova/Documents/GitHub/pcsc-project-template/eigen/unsupported/Eigen/MPRealSupport"
    "C:/Users/giova/Documents/GitHub/pcsc-project-template/eigen/unsupported/Eigen/NNLS"
    "C:/Users/giova/Documents/GitHub/pcsc-project-template/eigen/unsupported/Eigen/NonLinearOptimization"
    "C:/Users/giova/Documents/GitHub/pcsc-project-template/eigen/unsupported/Eigen/NumericalDiff"
    "C:/Users/giova/Documents/GitHub/pcsc-project-template/eigen/unsupported/Eigen/OpenGLSupport"
    "C:/Users/giova/Documents/GitHub/pcsc-project-template/eigen/unsupported/Eigen/Polynomials"
    "C:/Users/giova/Documents/GitHub/pcsc-project-template/eigen/unsupported/Eigen/SparseExtra"
    "C:/Users/giova/Documents/GitHub/pcsc-project-template/eigen/unsupported/Eigen/SpecialFunctions"
    "C:/Users/giova/Documents/GitHub/pcsc-project-template/eigen/unsupported/Eigen/Splines"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Devel" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/unsupported/Eigen" TYPE DIRECTORY FILES "C:/Users/giova/Documents/GitHub/pcsc-project-template/eigen/unsupported/Eigen/src" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/giova/Documents/GitHub/pcsc-project-template/cmake-build-debug/eigen/unsupported/Eigen/CXX11/cmake_install.cmake")

endif()

