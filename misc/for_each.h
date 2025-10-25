#ifndef FOR_EACH_H
#define FOR_EACH_H

#include "../main/header.h"
#include "../main/utility.h"

// for_each: call 'F' with integer from range [I, N)
// this version is implemented with 'constexpr', and can be easily upgraded to support 'Step'
template <size_t I, size_t N, typename F>
void for_each_run(F&& f)
{
    // f(I); // this won't compile if 'F' doesn't accept runtime variable I, i.e. __atomic_always_lock_free

    // this works since std::integral_constant<size_t, I>{} instantiates a class template with compile time 'I' and this
    // object is accepted by 'F', i.e. 'auto sz'; later on when using 'sz' as integer inside __atomic_always_lock_free,
    // it implicitly calls 'constexpr operator size_t() const { ... }' which is feasible at compile time.
    // NOTICE: the callback 'F' should declare argument as 'auto' than 'size_t' which becomes integer variable and
    // cannot be accepted by __atomic_always_lock_free!
    //
    // https://stackoverflow.com/questions/56937863/constexpr-lambda-argument
    f(std::integral_constant<size_t, I>{});

    if constexpr (I + 1 < N) {
        for_each_run<I + 1, N>(std::forward<F>(f));
    }
}

// index_sequence_range: deduce std::index_sequence<..> with range
template <size_t StartIdx, size_t EndIdx>
struct index_sequence_range {
    // https://stackoverflow.com/questions/40617854/implement-c-template-for-generating-an-index-sequence-with-a-given-range
    template <size_t... Seq>
    static constexpr auto gen_index_sequence(std::index_sequence<Seq...>)
    {
        return std::index_sequence<StartIdx + Seq...>{};
    }
    using type = decltype(gen_index_sequence(std::make_index_sequence<EndIdx - StartIdx>()));
};
template <size_t StartIdx, size_t EndIdx>
using make_index_sequence_range = typename index_sequence_range<StartIdx, EndIdx>::type;

// index_sequence_step_range: deduce std::index_sequence<..> with range, plus custom 'Step'
template <size_t StartIdx, size_t EndIdx, size_t Step = 1>
struct index_sequence_step_range {
    template <size_t S, size_t E, size_t... Seq>
    static constexpr auto gen_index_sequence()
    {
        // Idea is cumulatively find next index and append it to 'Seq...', form std::index_sequence<Seq...> at the end
        // https://gist.github.com/jjsullivan5196/a83f99263cda755edc257f9c50b53470
        if constexpr (S >= E) {
            return std::index_sequence<Seq...>{};
        } else {
            return gen_index_sequence<S + Step, E, Seq..., S>();
        }
    }
    using type = decltype(gen_index_sequence<StartIdx, EndIdx>());
};
template <size_t StartIdx, size_t EndIdx, size_t Step>
using make_index_sequence_step_range = typename index_sequence_step_range<StartIdx, EndIdx, Step>::type;

// for_each
template <typename F, size_t... I>
inline constexpr void for_each(F&& f, std::index_sequence<I...>)
{
    //(f(I), ...);
    (f(std::integral_constant<size_t, I>{}), ...);
}
template <size_t I, size_t N, typename F>
inline constexpr void for_each(F&& f)
{
    for_each(std::forward<F>(f), make_index_sequence_range<I, N>{});
}

template <size_t I, size_t N, size_t Step, typename F>
inline constexpr void for_each_with_step(F&& f)
{
    for_each(std::forward<F>(f), make_index_sequence_step_range<I, N, Step>{});
}

void run_for_each()
{
    // Rule: sizeof(M) must be power of 2, and not exceed 16
    for_each_run<0, 32>([](auto sz) {
        std::cout << "sizeof(A)=" << sz << ", __atomic_always_lock_free=" << __atomic_always_lock_free(sz, 0)
                  << std::endl;
    });

    auto tt = std::make_tuple(10, 20, 15.1, 'A', std::string{"ABC"});
    for_each_run<0, std::tuple_size_v<decltype(tt)>>([&tt](auto Idx) {
        std::cout << "\t" << std::get<Idx>(tt) << std::endl;
    });

    for_each<0, 32>([](auto sz) {
        std::cout << "sizeof(A)=" << sz << ", __atomic_always_lock_free=" << __atomic_always_lock_free(sz, 0)
                  << std::endl;
    });

    for_each<0, std::tuple_size_v<decltype(tt)>>([&tt](auto Idx) {
        std::cout << "\t" << std::get<Idx>(tt) << std::endl;
    });

    for_each<3, std::tuple_size_v<decltype(tt)>>([&tt](auto Idx) {
        std::cout << "\t" << std::get<Idx>(tt) << std::endl;
    });

    for_each_with_step<0, 32, 1>([](auto sz) {
        std::cout << "sizeof(A)=" << sz << ", __atomic_always_lock_free=" << __atomic_always_lock_free(sz, 0)
                  << std::endl;
    });

    for_each_with_step<0, 32, 3>([](auto sz) {
        std::cout << "sizeof(A)=" << sz << ", __atomic_always_lock_free=" << __atomic_always_lock_free(sz, 0)
                  << std::endl;
    });
}

#endif

