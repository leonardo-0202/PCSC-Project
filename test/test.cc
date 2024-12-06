#include <cmath>
#include <gtest/gtest.h>
#include "utils.h"

TEST(createReader, Files) {
    Eigen::MatrixXcd correct_mat = Eigen::MatrixXcd::Zero(4, 4);
    EXPECT_EQ( createReader("../config.json")->getInputData().input_matrix,
        correct_mat );
}

TEST(createReader, Picture) {
    Eigen::MatrixXcd correct_mat = Eigen::MatrixXcd::Zero(4, 4);
    EXPECT_EQ( createReader("../config.json")->getInputData().input_matrix,
        correct_mat );
}

TEST(createReader, Function) {
    Eigen::MatrixXcd correct_mat = Eigen::MatrixXcd::Zero(4, 4);
    EXPECT_EQ( createReader("../config.json")->getInputData().input_matrix,
        correct_mat );
}

