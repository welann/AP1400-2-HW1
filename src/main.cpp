
#include "hw1.h"
#include <gtest/gtest.h>
#include <iostream>
#include <vector>

int main(int argc, char** argv)
{
    if (false) // make false to run unit-tests
    {
        // debug section
        Matrix re(5, std::vector<double>(10));
        std::cout << re[0].size();
        re.resize(2, std::vector<double>(3));
        std::cout << re[0].size();

    } else {
        ::testing::InitGoogleTest(&argc, argv);
        std::cout << "RUNNING TESTS ..." << std::endl;
        int ret { RUN_ALL_TESTS() };
        if (!ret)
            std::cout << "<<<SUCCESS>>>" << std::endl;
        else
            std::cout << "FAILED" << std::endl;
    }
    return 0;
}