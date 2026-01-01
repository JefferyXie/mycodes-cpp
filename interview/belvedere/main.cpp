/*
 * Author: Jeffery Xie (jeffery.xie@gmail.com)
 * Date: 05/02/2019
 *
 * Highlights:
 *
 * 1) This is a really open question/project so the imeplementation
 *    is just in terms of my understanding, further details and discussion
 *    will be much helpful.
 *
 * 2) For speed, I use multiple threads to maximize hardware usage. The
 *    thread_pool and event_t are quiet generic and can handle any types of
 *    calculation interface. Also, the thread_pool is sharable, meaning
 *    multiple different types of calculation can run at the same time.
 *
 * 3) For generic, besides of thread_pool and event_t, the input_t and
 *    DoCalc are made generialize. Also helper functions are made to faciliate
 *    the usage, such like make_event, callback, etc.
 *
 * 4) Another point needs to be considered: program speed may be impacted by
 *    various reason. Using multiple thread won't guaranttee speed improvement
 *    if calculation itself is fast enough, but it'll be much useful if calc
 *    is very heavy.
 *
 */

/* Our trading system contains a number of calculation engines that perform calcs on a request basis.
 * The goal of this assignment is to come up with a generic framework for accelerating the calculations
 * in these engines. We have given you a benchmark test that provides a workload that is representative
 * of the types of calculations we are trying to accelerate.
 *
 * The goal of this assignment is to get the provided test to run as quickly as possible.
 *
 * Your solution should be generic. It should be easily extendible to methods other than IsPrime. It should
 * also be extendible to calculations with various function signatures. You should not modify IsPrime or Math.h.
 */

#include "thread_pool.h"
#include "calclib.h"
#include "icalc.h"

int main(int argc, char** argv)
{
    // case 1: MathLibrary::Math::IsPrime
    {
        std::default_random_engine             generator;
        std::uniform_int_distribution<int64_t> distribution(1, 1000);
        std::vector<input_t<int64_t>>          inputs;
        for (int32_t i = 0; i < 500; ++i) {
            inputs.push_back(distribution(generator));
        }
        auto tick    = std::chrono::high_resolution_clock::now();
        auto outputs = DoCalc(MathLibrary::Math::IsPrime, inputs);
        auto tock    = std::chrono::high_resolution_clock::now();

        for (unsigned i = 0; i < inputs.size(); ++i) {
            std::cout << std::get<0>(inputs[i].params) << "->" << outputs[i] << std::endl;
        }

        std::cout << "finished: " << (std::chrono::duration_cast<std::chrono::duration<double>>(tock - tick)).count()
                  << std::endl;
    }

    // case 2: calc_lib::my_calc
    {
        using result_t = std::pair<int, std::string>;
        std::vector<result_t> results;

        std::vector<input_t<std::string, int, calc_lib::extra_data>> inputs;
        for (unsigned i = 0; i < 100; ++i) {
            inputs.emplace_back("abc", i, calc_lib::extra_data{"xyz"});
        }

        auto tick    = std::chrono::high_resolution_clock::now();
        auto outputs = DoCalc(calc_lib::my_calc, inputs);
        auto tock    = std::chrono::high_resolution_clock::now();

        for (unsigned i = 0; i < inputs.size(); ++i) {
            std::cout << "(" << std::get<0>(inputs[i].params) << "," << std::get<1>(inputs[i].params) << ","
                      << std::get<2>(inputs[i].params).arr << ")->(" << outputs[i].first << "," << outputs[i].second
                      << ")" << std::endl;
        }

        std::cout << "finished: " << (std::chrono::duration_cast<std::chrono::duration<double>>(tock - tick)).count()
                  << std::endl;
    }

    return 0;
}
