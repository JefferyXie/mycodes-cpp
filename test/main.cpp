#include <benchmark/benchmark.h>
#include <gtest/gtest.h>

#include "test_algorithm.h"
#include "test_fixture.h"
#include "test_language.h"
#include "test_utility.h"
#include "test_benchmark.h"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    // Run Google Tests
    int gtest_result = RUN_ALL_TESTS();

    // Run Google Benchmarks only if tests passed or desired
    if (gtest_result == 0) {
        ::benchmark::Initialize(&argc, argv);
        ::benchmark::RunSpecifiedBenchmarks();
    }

    return gtest_result;
}

