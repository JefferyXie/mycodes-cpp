#include <gtest/gtest.h>
#include "header.h"
#include "../test/Test_Fixture.h"
#include "../test/Test_language.h"
#include "../test/Test_algorithm.h"
#include "../test/Test_multithreading.h"
#include "../language/C++11.h"

int main(int argc, char **argv) {
    //C11::Run_thread();
    //return 0;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


