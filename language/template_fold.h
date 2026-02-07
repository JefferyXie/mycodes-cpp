#pragma once

#include "../core/header.h"

// A very nice macro!!
#define RUN_N_DUMP(...) std::cout << #__VA_ARGS__ << " = " << __VA_ARGS__ << '\n'

template <typename... Args>
void try_fold(Args... args)
{
    // type 1, unary left fold:   ( ... op pack )           -> (((E1 op E2) op E3 ...) op En)
    // type 2, unary right fold:  ( pack op ... )           -> (E1 op (E2 op (E3 op (... op En))))
    // type 3, binary left fold:  ( init op ... op pack )   -> (((init op E1) op E2) ...) op En)
    // type 4, binary right fold: ( pack op ... op init )   -> (E1 op (E2 op (E3 op (E4 op (... op (En op init))))))
    std::cout << "dump: " << (std::cout << ... << args) << std::endl;

    std::cout << "dump: " << (std::cout << ... << args) << std::endl;

    std::cout << "dump: " << ((std::cout << args) << ...) << std::endl;

    std::cout << "dump: " << ((std::cout << args), ...) << std::endl; // type 2: (std::cout << arg1), ((std::cout << arg2), ((std::cout << arg3), (...)))

    std::cout << "sum: " << (args + ...) << std::endl;
    std::cout << "sum: " << (... + args) << std::endl;

    std::cout << "sum: " << (args + ... + (100 * 2)) << std::endl;
    std::cout << "sum: " << ((100 * 2) + ... + args) << std::endl;

    std::cout << "&&: " << (args && ...) << std::endl;
    std::cout << "&&: " << (... && args) << std::endl;

    // clang-format off
    ([&] { args += 1; }(args), ...);
    std::cout << "dump: " << (std::cout << ... << args) << std::endl;

    ([](auto& v) { v += 1; }(args), ...);
    std::cout << "dump: " << (std::cout << ... << args) << std::endl;
    // clang-format on
}

void try_fold_2(auto... args) {
    // same as above try_fold(..) function template
}

constexpr auto try_lcm(auto x, auto... xs)
{
    return ((x = std::lcm(x, xs)), ...);
}

void run_template_fold()
{
    constexpr int p{2 * 2 * 3};
    constexpr int q{2 * 3 * 3};
    static_assert(2 * 2 * 3 * 3 == std::lcm(p, q));
    static_assert(225 == std::lcm(45, 75));

    // try_lcm(2 * 3, 3 * 4, 4 * 5) = 60
    // try_lcm(2 * 3 * 4, 3 * 4 * 5, 4 * 5 * 6) = 120
    // try_lcm(2 * 3 * 4, 3 * 4 * 5, 4 * 5 * 6, 5 * 6 * 7) = 840
    RUN_N_DUMP(try_lcm(2 * 3, 3 * 4, 4 * 5));
    RUN_N_DUMP(try_lcm(2 * 3 * 4, 3 * 4 * 5, 4 * 5 * 6));
    RUN_N_DUMP(try_lcm(2 * 3 * 4, 3 * 4 * 5, 4 * 5 * 6, 5 * 6 * 7));
}
