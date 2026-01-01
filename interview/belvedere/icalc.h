#pragma once

#include "header.h"
#include "thread_pool.h"

// generic input data
template <class... Args>
struct input_t {
    using params_list_t = std::tuple<Args...>;
    params_list_t params;
    input_t(Args&&... args) : params(std::forward<Args>(args)...) {}
};

template <class Func, class Params, std::size_t... Is>
auto Run(Func& func, const Params& paras, std::index_sequence<Is...>)
{
    return func(std::get<Is>(paras)...);
}

// generic DoCalc interface
template <class Func, class... Args>
auto DoCalc(Func& func, const std::vector<input_t<Args...>>& inputs)
{
    typename input_t<Args...>::params_list_t params_type;
    using result_t = decltype(Run(func, params_type, std::index_sequence_for<Args...>{}));
    std::vector<result_t> results;
    results.resize(inputs.size());

    thread_pool           pool;
    [[maybe_unused]] auto num_threads = pool.start();

    using namespace std::placeholders;

    auto func_cb     = std::bind(callback<result_t>, std::ref(results), _1, _2);
    using event_type = decltype(make_event(0, func, func_cb, inputs.begin()->params));
    std::vector<event_type> events;
    events.reserve(inputs.size());
    for (unsigned i = 0; i < inputs.size(); ++i) {
        events.push_back(make_event(i, func, func_cb, inputs[i].params));
        pool.push(&events.back());
    }
    pool.wait_until_all_done();
    return results;
}

