#include "../algorithm/2023.h"
#include "../algorithm/2024.h"
#include "../algorithm/2025.h"
#include "../algorithm/2026.h"

#include "../interview/jump/repo.h"
#include "../interview/jump/task_parser.h"

#include "../language/c++11.h"
#include "../language/meta.h"
#include "../language/movement.h"
#include "../language/template_SFINAE.h"
#include "../language/template_concept.h"
#include "../language/template_constraints.h"
#include "../language/template_crtp_mixin.h"
#include "../language/template_fold.h"
#include "../language/template_generic_factory.h"
#include "../language/template_special.h"
#include "../language/template_special2.h"
#include "../language/type_deduction.h"

#include "../misc/__draft.h"

#include "../thread/mutex_spinlock.h"
#include "../thread/producer_consumer.h"

/*
void bm_memory_access(benchmark::State& state)
{
    std::default_random_engine engine;
    auto bytes = state.range(0) * 1024;
    std::vector<unsigned> data(bytes / sizeof(unsigned));
    stdr::generate(data, engine);
    std::uniform_int_distribution<std::size_t> dist(0, data.size() – 1);
    for (auto _ : state) {
        benchmark::DoNotOptimize(++data[dist(engine)]);
    }
    state.SetBytesProcessed(state.iterations() * data.size());
}



// Random access:
benchmark::DoNotOptimize(++data[dist(engine)]);

// Sequential forward access:
for (std::size_t idx = 0; idx < data.size(); ++idx)
{
    benchmark::DoNotOptimize(dist(engine));
    benchmark::DoNotOptimize(++data[idx]);
}

// Sequential backward access:
for (std::size_t idx = data.size(); idx > 0; --idx)
{
    benchmark::DoNotOptimize(dist(engine));
    benchmark::DoNotOptimize(++data[idx - 1]);
}



std::vector<std::size_t> indices(chunk_size);
std::iota(indices.begin(), indices.end(), 0);
std::uniform_int_distribution<std::size_t> dist(0, N - chunk_size - 1);
for (auto _ : state)
{
    state.PauseTiming();
    std::shuffle(indices.begin(), indices.end(), engine);
    state.ResumeTiming();
    auto start = dist(engine);
    for (auto i : indices) {
        sum += data[start + i];
    }
    benchmark::DoNotOptimize(sum);
}

*/

int main(int argc, char** argv)
{
    run_triangle_min_sum_path();
    return 0;

    run_merge_arrays();
    return 0;

    run_is_palindrome();
    return 0;

    __event_queue::run_event_queue();
    return 0;

    run_tree_to_list();
    return 0;

    run_max_nodes_exclu_neighbors();
    return 0;

    run_task_parser();
    return 0;

    run_mutex_spinlock();
    return 0;

    run_type_deduction();

    run_meta();
    run_move();
    run_template_concept();
    run_template_constraints_1();
    run_template_constraints_2();
    run_template_fold();
    run_template_generic_factory();
    run_template_SFINAE_1();
    run_template_SFINAE_2();
    run_template_SFINAE_3();
    run_template_specialization();
    run_template_specialization_2();
    return 0;

    struct AA {
        AA(int a) { std::cout << "AA(int): " << a << std::endl; }

        AA(const AA&) { std::cout << "AA(const AA&)" << std::endl; }
        AA& operator=(const AA&)
        {
            std::cout << "operator=(const AA&)" << std::endl;
            return *this;
        }

        AA(AA&& o)
        {
            a   = o.a;
            b   = o.b;
            c   = std::move(o.c);
            o.a = 0;
            o.b = 0;
            o.c = "";
            std::cout << "AA(AA&&)" << std::endl;
        }
        AA& operator=(AA&& o)
        {
            a   = o.a;
            b   = o.b;
            c   = std::move(o.c);
            o.a = 0;
            o.b = 0;
            o.c = "";
            std::cout << "operator=(AA&&)" << std::endl;
            return *this;
        }

        ~AA() { std::cout << "~AA()" << std::endl; }

        int         a;
        double      b;
        std::string c;
    };
    struct BB {
        AA aa;
        BB() : aa{10} { std::cout << "BB()" << std::endl; }
        ~BB() { std::cout << "~BB()" << std::endl; }
    };

    int a = 10;
    /*
        int b = 20;

        int& ar    = a;
        int& br    = b;

        auto test2 = std::forward_as_tuple(ar, br);
        static_assert(std::is_same_v<decltype(test2), std::tuple<int&, int&>>);

        [[maybe_unused]] auto test3 = std::make_tuple(ar, br);
        static_assert(std::is_same_v<decltype(test3), std::tuple<int, int>>);

        // auto test3 = std::make_tuple(ar,br); // this cannot make tuple of int& reference
        std::get<0>(test2) = -1;
        std::get<1>(test2) = -2;
        std::cout << a << ":" << b << std::endl;
    */

    AA   ss(10);
    auto test1 = std::forward_as_tuple(a, std::move(ss));
    static_assert(std::is_same_v<decltype(test1), std::tuple<int&, AA&&>>);
    std::cout << ss.c << ", " << std::get<1>(test1).c << std::endl;

    std::get<1>(test1).c = "fff";
    std::cout << ss.c << ", " << std::get<1>(test1).c << std::endl;

    // auto yy = BB();

    // AA a = 10;

    // AA b(100);
    // AA c = AA(20);

    return 0;

    std::tuple<int, long, std::string> tp(10, 20, "abc");
    std::apply(
        []<typename... Args>(Args&&... v) {
            std::cout << sizeof...(Args) << std::endl;
            (
                [&]() {
                    std::cout << v << std::endl;
                }(),
                ...);
        },
        tp);

    return 0;
}
