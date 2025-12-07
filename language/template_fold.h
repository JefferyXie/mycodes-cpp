#pragma once

#include "../core/header.h"

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

    std::cout << "dump: " << ((std::cout << args), ...) << std::endl;

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

void run_template_fold()
{
    // TODO
}
