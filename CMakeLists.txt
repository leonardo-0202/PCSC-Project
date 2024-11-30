cmake_minimum_required (VERSION 3.16)
project (PCSC-project)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wa,-mbig-obj)

include_directories(eigen external)

add_library(eigenvalues
        src/Reader.cpp
        src/InputData.cc
        src/Solver.cc
        src/utils.cc
)

add_subdirectory(googletest)

add_executable(main src/main.cc)

target_link_libraries(main eigenvalues)

add_executable(test_pcsc test/test.cc)
target_link_libraries(test_pcsc gtest_main gtest pthread eigenvalues)

add_custom_target(test ./test_pcsc DEPENDS test_pcsc)