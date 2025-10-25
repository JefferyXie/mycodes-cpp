#ifndef BOOST_H
#define BOOST_H

#include "../main/header.h"

// https://github.com/boostorg/accumulators/blob/develop/example/main.cpp
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>

void boost_accumulators()
{
    constexpr uint32_t    SamplesCount = 100000;
    std::vector<uint64_t> samples(SamplesCount, 0);
    for (auto& s : samples) {
        // s = rdtsc();
        s = std::rand();
    }

    namespace bacc = boost::accumulators;
    bacc::accumulator_set<double, bacc::stats<bacc::tag::mean, bacc::tag::variance>> c;
    for (std::size_t i = 1; i < samples.size(); ++i) {
        c(samples[i] - samples[i - 1]);
    }

    [[maybe_unused]] const double m      = bacc::mean(c);
    [[maybe_unused]] const double stddev = std::sqrt(bacc::variance(c) * samples.size() / double(samples.size() - 1));

    std::cout << "m=" << m << ", stddev=" << stddev << std::endl;
}

void run_boost()
{
    boost_accumulators();
}

#endif

