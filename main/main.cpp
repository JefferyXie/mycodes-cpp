#include <gtest/gtest.h>
#include "header.h"
#include "../test/Test_Fixture.h"
#include "../test/Test_language.h"
#include "../test/Test_algorithm.h"
#include "../test/Test_multithreading.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

