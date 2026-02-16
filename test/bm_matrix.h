#pragma once

#include <benchmark/benchmark.h>

#include "../core/header.h"
#include "../data_structure/matrix.h"

static void BM_matrix_multiply(benchmark::State& state)
{
    auto gen_matrix = [](size_t R, size_t C) {
        matrix_t<int>                      m{R, C};
        std::mt19937                       gen(42);
        std::uniform_int_distribution<int> dist{-100, 100};
        for (size_t r = 0; r < R; ++r) {
            for (size_t c = 0; c < C; ++c) {
                m(r, c) = dist(gen);
            }
        }
        return m;
    };

    const auto R = state.range(0);
    const auto C = state.range(1);

    auto m1 = gen_matrix(R, C);
    auto m2 = gen_matrix(C, R);
    for (auto _ : state) {
        auto start = std::chrono::high_resolution_clock::now();

        [[maybe_unused]] auto v = m1.multiply(m2);
        // benchmark::DoNotOptimize(v);

        auto stop = std::chrono::high_resolution_clock::now();
        auto secs = std::chrono::duration<double>(stop - start).count();
        state.SetIterationTime(secs);
    }

    state.SetComplexityN(R * C);

    state.SetItemsProcessed(state.iterations());
}

static void BM_matrix_multiply_2(benchmark::State& state)
{
    auto gen_matrix = [](size_t R, size_t C) {
        matrix_t<int>                      m{R, C};
        std::mt19937                       gen(42);
        std::uniform_int_distribution<int> dist{-100, 100};
        for (size_t r = 0; r < R; ++r) {
            for (size_t c = 0; c < C; ++c) {
                m(r, c) = dist(gen);
            }
        }
        return m;
    };

    const auto R = state.range(0);
    const auto C = state.range(1);

    auto m1 = gen_matrix(R, C);
    auto m2 = gen_matrix(C, R);
    for (auto _ : state) {
        auto start = std::chrono::high_resolution_clock::now();

        [[maybe_unused]] auto v = m1.multiply_2(m2);
        // benchmark::DoNotOptimize(v);

        auto stop = std::chrono::high_resolution_clock::now();
        auto secs = std::chrono::duration<double>(stop - start).count();
        state.SetIterationTime(secs);
    }

    state.SetComplexityN(R * C);

    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_matrix_multiply)
    ->RangeMultiplier(8)
    ->Ranges({
        {2 << 3, 2 << 10},
        {2 << 3, 2 << 10},
    })
    ->Complexity(benchmark::oN)
    ->UseManualTime()
    ->Unit(benchmark::kMillisecond);

BENCHMARK(BM_matrix_multiply_2)
    ->RangeMultiplier(8)
    ->Ranges({
        {2 << 3, 2 << 10},
        {2 << 3, 2 << 10},
    })
    ->Complexity(benchmark::oNSquared)
    ->UseManualTime()
    ->Unit(benchmark::kMillisecond);

/*

2026-02-15T11:35:06-06:00
Running ./test/main_exe
Run on (16 X 3096.01 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x8)
  L1 Instruction 32 KiB (x8)
  L2 Unified 256 KiB (x8)
  L3 Unified 16384 KiB (x1)
Load Average: 0.47, 0.49, 0.32
***WARNING*** Library was built as DEBUG. Timings may be affected.
-----------------------------------------------------------------------------------------------------
Benchmark                                           Time             CPU   Iterations UserCounters...
-----------------------------------------------------------------------------------------------------
BM_matrix_multiply/16/16/manual_time            0.001 ms        0.001 ms       680013 items_per_second=1.03231M/s
BM_matrix_multiply/64/16/manual_time            0.013 ms        0.014 ms        51630 items_per_second=74.2194k/s
BM_matrix_multiply/512/16/manual_time           0.847 ms        0.847 ms          825 items_per_second=1.18082k/s
BM_matrix_multiply/2048/16/manual_time           14.0 ms         14.0 ms           48 items_per_second=71.3639/s
BM_matrix_multiply/16/64/manual_time            0.004 ms        0.004 ms       178547 items_per_second=253.631k/s
BM_matrix_multiply/64/64/manual_time            0.052 ms        0.052 ms        13270 items_per_second=19.2306k/s
BM_matrix_multiply/512/64/manual_time            3.29 ms         3.29 ms          215 items_per_second=304.237/s
BM_matrix_multiply/2048/64/manual_time           54.8 ms         54.8 ms           12 items_per_second=18.2434/s
BM_matrix_multiply/16/512/manual_time           0.030 ms        0.030 ms        23056 items_per_second=33.6491k/s
BM_matrix_multiply/64/512/manual_time           0.412 ms        0.412 ms         1710 items_per_second=2.42558k/s
BM_matrix_multiply/512/512/manual_time           27.2 ms         27.2 ms           26 items_per_second=36.7754/s
BM_matrix_multiply/2048/512/manual_time           428 ms          428 ms            2 items_per_second=2.33479/s
BM_matrix_multiply/16/2048/manual_time          0.119 ms        0.119 ms         5832 items_per_second=8.4099k/s
BM_matrix_multiply/64/2048/manual_time           1.68 ms         1.68 ms          417 items_per_second=594.427/s
BM_matrix_multiply/512/2048/manual_time           110 ms          110 ms            6 items_per_second=9.10053/s
BM_matrix_multiply/2048/2048/manual_time         2034 ms         2034 ms            1 items_per_second=0.491665/s
BM_matrix_multiply/manual_time_BigO            457.83 N        457.83 N    
BM_matrix_multiply/manual_time_RMS                 61 %            61 %    

BM_matrix_multiply_2/16/16/manual_time          0.002 ms        0.002 ms       346231 items_per_second=498.957k/s
BM_matrix_multiply_2/64/16/manual_time          0.030 ms        0.030 ms        23103 items_per_second=32.9359k/s
BM_matrix_multiply_2/512/16/manual_time          1.91 ms         1.91 ms          361 items_per_second=524.329/s
BM_matrix_multiply_2/2048/16/manual_time         32.6 ms         32.6 ms           22 items_per_second=30.6324/s
BM_matrix_multiply_2/16/64/manual_time          0.007 ms        0.008 ms        96638 items_per_second=133.378k/s
BM_matrix_multiply_2/64/64/manual_time          0.118 ms        0.118 ms         5834 items_per_second=8.47006k/s
BM_matrix_multiply_2/512/64/manual_time          9.95 ms         9.95 ms           73 items_per_second=100.54/s
BM_matrix_multiply_2/2048/64/manual_time          293 ms          293 ms            2 items_per_second=3.41379/s
BM_matrix_multiply_2/16/512/manual_time         0.058 ms        0.058 ms        12236 items_per_second=17.3859k/s
BM_matrix_multiply_2/64/512/manual_time          1.23 ms         1.23 ms          573 items_per_second=814.137/s
BM_matrix_multiply_2/512/512/manual_time          174 ms          174 ms            4 items_per_second=5.75503/s
BM_matrix_multiply_2/2048/512/manual_time        2635 ms         2635 ms            1 items_per_second=0.379479/s
BM_matrix_multiply_2/16/2048/manual_time        0.252 ms        0.253 ms         2795 items_per_second=3.96047k/s
BM_matrix_multiply_2/64/2048/manual_time         12.4 ms         12.4 ms           57 items_per_second=80.8613/s
BM_matrix_multiply_2/512/2048/manual_time         849 ms          849 ms            1 items_per_second=1.17784/s
BM_matrix_multiply_2/2048/2048/manual_time      49435 ms        49434 ms            1 items_per_second=0.0202285/s
BM_matrix_multiply_2/manual_time_BigO            0.00 N^2        0.00 N^2  
BM_matrix_multiply_2/manual_time_RMS               17 %            17 %    

*/
