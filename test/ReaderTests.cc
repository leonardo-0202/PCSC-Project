#include <cmath>
#include <gtest/gtest.h>
#include <complex>
#include "utils.h"
#include "Exceptions.h"

// Test 1: 2x2 real matrix
TEST(createReader, Test1) {
    Eigen::MatrixXcd expected_matrix(2, 2);
    expected_matrix << std::complex<double>(1, 0), std::complex<double>(2, 0),
                       std::complex<double>(3, 0), std::complex<double>(4, 0);

    EXPECT_EQ(
        createReader("../test/ReaderTests/test1.json")->getInputData().input_matrix,
        expected_matrix
    );
}

// Test 2: 3x3 complex matrix
TEST(createReader, Test2) {
    Eigen::MatrixXcd expected_matrix(3, 3);
    expected_matrix << std::complex<double>(1, 0), std::complex<double>(2, 2.1), std::complex<double>(3, 0),
                       std::complex<double>(4.5, 5), std::complex<double>(5, 0), std::complex<double>(6, 1),
                       std::complex<double>(7, -1), std::complex<double>(8, 0.123), std::complex<double>(9, 0);

    EXPECT_EQ(
        createReader("../test/ReaderTests/test2.json")->getInputData().input_matrix,
        expected_matrix
    );
}

// Test 3: 5x5 mixed matrix
TEST(createReader, Test3) {
    Eigen::MatrixXcd expected_matrix(5, 5);
    expected_matrix << std::complex<double>(1, 0),   std::complex<double>(2, 1),  std::complex<double>(3.5, -2),
                       std::complex<double>(4.2, 0), std::complex<double>(5, 0),
                       std::complex<double>(6, -3),  std::complex<double>(7.1, 1.2), std::complex<double>(0, 8),
                       std::complex<double>(9.3, 0), std::complex<double>(10, -1.5),
                       std::complex<double>(11, 2),  std::complex<double>(12, -2.3), std::complex<double>(13, 0),
                       std::complex<double>(14, 0),  std::complex<double>(15, 0),
                       std::complex<double>(16.7, -3.1), std::complex<double>(17, 1), std::complex<double>(18, 2),
                       std::complex<double>(19, 0), std::complex<double>(20, -4),
                       std::complex<double>(21, 0),  std::complex<double>(22, 0), std::complex<double>(23, -5),
                       std::complex<double>(24, 6), std::complex<double>(25, 0);

    EXPECT_EQ(
        createReader("../test/ReaderTests/test3.json")->getInputData().input_matrix,
        expected_matrix
    );
}

// Test 4: Invalid config path
TEST(createReader, InvalidConfigPath) {
    // Expect the createReader function to throw an exception when given an invalid config path
    EXPECT_THROW(
        {
            createReader("../random_path/invalid.json");
        },
        ConfigError
    );
}

// Test 5: Invalid csv file path
TEST(createReader, InvalidFilePath) {
    // Expect the createReader function to throw an exception when given an invalid csv path
    EXPECT_THROW(
        {
            createReader("../test/ReaderTests/InvalidPath.json");
        },
        ReaderError
    );
}

// Test 6: Non-square matrix
TEST(createReader, Rectangular_Matrix) {
    // Expect the createReader function to throw an exception when given an invalid csv path
    EXPECT_THROW(
        {
            createReader("../test/ReaderTests/RectangularMatrix.json");
        },
        ReaderError
    );
}

// Test 7: parsing edgecase (numbers like 1 + i)
TEST(createReader, parse_edgecase){
    Eigen::MatrixXcd expected_matrix(1, 1);
    expected_matrix << std::complex<double>(1, 1);
    EXPECT_EQ(createReader("../test/ReaderTests/parse_edgecase.json")->getInputData().input_matrix,
        expected_matrix);
}

// Test 7: Function i * j
TEST(createReader, function1) {
    Eigen::MatrixXcd expected_matrix(3, 3);
    expected_matrix << std::complex<double>(1, 0), std::complex<double>(2, 0), std::complex<double>(3, 0),
             std::complex<double>(2, 0), std::complex<double>(4, 0), std::complex<double>(6, 0),
             std::complex<double>(3, 0), std::complex<double>(6, 0), std::complex<double>(9, 0);
    EXPECT_EQ(createReader("../test/ReaderTests/functiontest1.json")->getInputData().input_matrix,
        expected_matrix);
}

// Test 8: Function i^2 + 2
TEST(createReader, function2) {
    Eigen::MatrixXcd expected_matrix(3, 3);
    expected_matrix << std::complex<double>(3, 0), std::complex<double>(3, 0), std::complex<double>(3, 0),
             std::complex<double>(6, 0), std::complex<double>(6, 0), std::complex<double>(6, 0),
             std::complex<double>(11, 0), std::complex<double>(11, 0), std::complex<double>(11, 0);
    EXPECT_EQ(createReader("../test/ReaderTests/functiontest2.json")->getInputData().input_matrix,
        expected_matrix);
}

// Test 9: Constant function, 5
TEST(createReader, function3) {
    Eigen::MatrixXcd expected_matrix(3, 3);
    expected_matrix << std::complex<double>(5, 0), std::complex<double>(5, 0), std::complex<double>(5, 0),
             std::complex<double>(5, 0), std::complex<double>(5, 0), std::complex<double>(5, 0),
             std::complex<double>(5, 0), std::complex<double>(5, 0), std::complex<double>(5, 0);
    EXPECT_EQ(createReader("../test/ReaderTests/functiontest3.json")->getInputData().input_matrix,
        expected_matrix);
}

// Test 10: Negative size in function
TEST(createReader, function_negative_size) {
    EXPECT_THROW(createReader("../test/ReaderTests/func_neg_size.json"),
        ReaderError);
}

// Test 11: Greyscale picture
TEST(createReader, Greyscale_Picture) {
    EXPECT_NO_THROW(
        {
            createReader("../test/ReaderTests/greyscalepic.json");
        });
}

// Test 12: Colored picture
TEST(createReader, Colored_Picture) {
    EXPECT_NO_THROW(
        {
            createReader("../test/ReaderTests/coloredpic.json");
        });
}

// Test 13: Cropping
TEST(createReader, Cropping) {
    EXPECT_NO_THROW(
        {
            createReader("../test/ReaderTests/cropping.json");
        });
}

