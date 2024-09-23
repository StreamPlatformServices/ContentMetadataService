#pragma once
#include "DefaultUnitTests.hpp"

TEST_F(DefaultUnitTests, Random_test)
{

    EXPECT_EQ(2, 2);


    EXPECT_EQ(0, 0);

}


TEST_F(DefaultUnitTests, ShouldThrowExceptionForInvalidInput)
{
    EXPECT_THROW(
        {
            throw std::invalid_argument("");

        }, std::invalid_argument);
}


class DefaultUnitTestsParameterizedTest : public ::testing::TestWithParam<int> {};

TEST_P(DefaultUnitTestsParameterizedTest, ShouldMapCorrectlyForParams) {
    int input = GetParam();

    EXPECT_GT(input, 0); 
}

INSTANTIATE_TEST_SUITE_P(PositiveInputs, DefaultUnitTestsParameterizedTest, ::testing::Values(1, 2, 3, 4));


