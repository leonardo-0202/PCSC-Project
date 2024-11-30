cmake_minimum_required (VERSION 3.26)
project (PCSC-project)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wa,-mbig-obj)

set(GIT_EXECUTABLE "git")
option(GIT_SUBMODULE "Check submodules during build" ON)
if(GIT_SUBMODULE)
    message(STATUS "Submodule update")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive googletest WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()
endif(GIT_SUBMODULE)


option(DOCUMENTATION "Activate documentation" ON)
if (DOCUMENTATION)
    set(DOXYGEN_EXECUTABLE doxygen)
    set(DOXYFILE Doxyfile)
    add_custom_target(doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/${DOXYFILE}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
endif(DOCUMENTATION)

include_directories(eigen external)

add_library(eigenvalues
        src/Reader.cpp
        src/Solver.cc
        src/utils.cc
        src/OutputGenerator.cc
)

add_subdirectory(googletest)

add_executable(main src/main.cc)

target_link_libraries(main eigenvalues)

add_executable(test_pcsc test/test.cc)
target_link_libraries(test_pcsc gtest_main gtest pthread eigenvalues)

add_custom_target(test ./test_pcsc DEPENDS test_pcsc)