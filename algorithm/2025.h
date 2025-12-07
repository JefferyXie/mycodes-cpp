#pragma once

#include "../main/node.h"
#include "../main/utility.h"
#include <queue>
#include <tuple>
#include <utility>
#include <vector>

// Worst Trade Reporter
// https://leetcode.com/discuss/interview-question/3812812/Optiver-or-OA-or-Worst-Trade-Reporter
// 1) hash map: symbol <> (latest true price, [top bid price, top ask price])
// 2) worst trade = max(max(top bid price - true price, 0), max(true price - top ask price, 0));

// Look and Say sequence
// https://www.geeksforgeeks.org/look-and-say-sequence/
// https://www.geeksforgeeks.org/dsa/look-and-say-sequence/
// 1, 11, 21, 1211, 111221, 312211, 13112221, 1113213211, …
// Track two numbers: one represents the repeating times, the other represents current number, write down them and reset
// if the coming number/character changes.

// Given a positive integer N, find the smallest number of steps it will take to reach 1.
// There are two kinds of permitted steps:
// 1) You may decrement N to N - 1.
// 2) If a * b = N, you may decrement N to the larger of a and b.
int find_min_steps_to_1(int N)
{
    std::vector<bool> visited(N + 1, false);

    using record_t = std::pair<int, int>;
    std::queue<record_t> q;
    q.push({N, 0});
    while (!q.empty()) {
        const auto [curr_N, curr_steps] = q.front();
        q.pop();
        if (curr_N == 1) {
            return curr_steps;
        }

        // IDEA: with single step, number N can reach (N - 1) and any number M as long as M * I = N, where M >= I.

        // NOTICE: we are using BFS, so if target value (curr_N - 1) or (curr_N / i) is already visited before, skip it
        // since this route must not be the optimal
        if (!visited[curr_N - 1]) {
            visited[curr_N - 1] = true;
            q.push({curr_N - 1, curr_steps + 1});
        }
        for (int i = 2; i * i <= curr_N; ++i) {
            if (curr_N % i != 0 || visited[curr_N / i]) {
                continue;
            }
            visited[curr_N / i] = true;
            q.push({curr_N / i, curr_steps + 1});
        }
    }
    return -1;
}
void run_find_min_steps_to_1()
{
    using use_case_t = std::pair<int, int>;
    for (auto [N, exp_v] : {
             use_case_t(5, 3),      // 5 > 4 > 2 > 1
             use_case_t(6, 3),      // 6 > 3 > 2 > 1
             use_case_t(19, 5),     // 19 > 18 > 6 > 3 > 2 > 1
             use_case_t(17, 4),     // 17 > 16 > 4 > 2 > 1
             use_case_t(50, 5),     // 50 > 10 > 5 > 4 > 2 > 1
             use_case_t(100, 5),    // 100 > 10 > 9 > 3 > 2 > 1
         }) {
        const auto v = find_min_steps_to_1(N);
        std::cout << "N=" << N << ", steps=" << v << ", find_min_steps_to_1=" << (exp_v == v ? "SUCCESS" : "FAILED")
                  << std::endl;
    }
}

// Given position (a, b), find the min number of steps it will take to reach (x, y) where a, b, x, y are all positive.
// At any position (a, b), next step can be either (a + b, b) or (a, b + a).
int find_min_steps_to_x_y(int x, int y, int a, int b)
{
    if (x == a && y == b)
        return 0;
    if (x < a || y < b)
        return -1;

    int move_b = find_min_steps_to_x_y(x, y, a, b + a);
    int move_a = find_min_steps_to_x_y(x, y, a + b, b);
    if (move_a < 0 && move_b < 0)
        return -1;
    if (move_a < 0)
        return move_b + 1;
    if (move_b < 0)
        return move_a + 1;
    return std::min(move_a, move_b) + 1;
}
int find_min_steps_to_x_y_2(int x, int y, int a, int b)
{
    // in case there is deadloop, and avoid duplicate steps
    std::unordered_set<std::pair<int, int>, decltype([](auto& p) {
                           return p.first ^ (p.second << 1);
                       })>
        visited;

    // <a, b, steps_so_far>
    std::queue<std::tuple<int, int, int>> q;
    q.push({a, b, 0});
    while (!q.empty()) {
        const auto [p1, p2, steps] = q.front();
        q.pop();

        if (p1 == x && p2 == y) {
            return steps;
        }

        if (p1 > x && p2 > y) {
            break;
        }

        if (visited.emplace(p1 + p2, p2).second) {
            q.push({p1 + p2, p2, steps + 1});
        }
        if (visited.emplace(p1, p1 + p2).second) {
            q.push({p1, p1 + p2, steps + 1});
        }
    }
    return -1;
}
void run_find_min_steps_to_x_y()
{
    using use_case_t = std::tuple<std::array<int, 2>, std::array<int, 2>, int>;
    for (auto [target, start, exp_v] : {
             use_case_t({3, 2}, {1, 1}, 2),
             use_case_t({4, 4}, {1, 1}, -1),
             use_case_t({10, 12}, {4, 4}, -1),
         }) {
        const auto v1 = find_min_steps_to_x_y(target[0], target[1], start[0], start[1]);
        std::cout << "target=(" << target[0] << ", " << target[1] << "), start=(" << start[0] << ", " << start[1]
                  << "), find_min_steps_to_x_y=" << v1 << ", " << (exp_v == v1 ? "SUCCESS" : "FAILED") << std::endl;

        const auto v2 = find_min_steps_to_x_y_2(target[0], target[1], start[0], start[1]);
        std::cout << "target=(" << target[0] << ", " << target[1] << "), start=(" << start[0] << ", " << start[1]
                  << "), find_min_steps_to_x_y_2=" << v2 << ", " << (exp_v == v2 ? "SUCCESS" : "FAILED") << std::endl;
    }
}

// https://www.geeksforgeeks.org/find-the-minimum-number-of-steps-to-reach-m-from-n/
//
// Given an integer start, find the min steps it will take to reach target integer.
// Two operations you can do at each step -
// 1) Multiply the number by 2. So, x becomes 2*x.
// 2) Subtract one from the number x. So, x becomes x-1.
//
// The idea is to reverse the problem as follows - get number N starting from M using the operations:
// 1) Divide the number by 2 if it is even.
// 2) Add 1 to the number.
//
// Basically operating reversely is in order to take division as early as it can, this is hard to be achieved in
// non-reverse order.
int find_min_steps_to_N_multi(int start, int target)
{
    int steps = 0;
    while (start < target) {
        if (target % 2 == 0) {
            target /= 2;
        } else {
            target += 1;
        }
        ++steps;
    }
    return steps + start - target;
}
void run_find_min_steps_to_N_multi()
{
    using use_case_t = std::tuple<int, int, int>;
    for (auto [start, target, exp_v] : {
             use_case_t(1, 5, 5),
             use_case_t(4, 6, 2),
             use_case_t(1, 10, 6),
             use_case_t(10, 1, 9),
         }) {
        const auto v = find_min_steps_to_N_multi(start, target);
        std::cout << "start=" << start << ", target=" << target << ", find_min_steps_to_N_multi=" << v << ", "
                  << (exp_v == v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

// Given a positive integer N, find the minimum number of steps that it takes to get reduced to 1.
// You can perform any one of the following 3 steps:
// 1) Subtract 1 from it. (n = n — 1) ,
// 2) If n is divisible by 2, divide by 2.( if n % 2 == 0, then n = n / 2 ) ,
// 3) If n is divisible by 3, divide by 3. (if n % 3 == 0, then n = n / 3 ).
int find_min_steps_to_1_dp(int N)
{
    std::vector<int> dp(N + 1, -1);
    dp[1] = 0;
    dp[2] = 1;
    dp[3] = 1;
    for (int i = 4; i <= N; ++i) {
        auto v = dp[i - 1];
        if (i % 2 == 0) {
            v = std::min(dp[i / 2], v);
        }
        if (i % 3 == 0) {
            v = std::min(dp[i / 3], v);
        }
        dp[i] = 1 + v;
    }
    return dp.back();
}
int find_min_steps_to_1_recursive(int N)
{
    if (N == 1) {
        return 0;
    }

    auto steps_1 = find_min_steps_to_1_recursive(N - 1);
    auto steps_2 = steps_1;
    if (N % 2 == 0) {
        steps_2 = find_min_steps_to_1_recursive(N / 2);
    }
    auto steps_3 = steps_1;
    if (N % 3 == 0) {
        steps_3 = find_min_steps_to_1_recursive(N / 3);
    }
    return 1 + std::min({steps_1, steps_2, steps_3});
}
int find_min_steps_to_1_recursive_2(int N)
{
    std::vector<int>        dp(N + 1, -1);
    std::function<int(int)> run = [&](int n) {
        if (n == 1) {
            return 0;
        }
        if (dp[n] >= 0) {
            return dp[n];
        }

        auto steps_1 = run(n - 1);
        auto steps_2 = steps_1;
        if (n % 2 == 0) {
            steps_2 = run(n / 2);
        }
        auto steps_3 = steps_1;
        if (n % 3 == 0) {
            steps_3 = run(n / 3);
        }
        dp[n] = 1 + std::min({steps_1, steps_2, steps_3});
        return dp[n];
    };
    return run(N);
}
void run_find_min_steps_to_1_dp()
{
    using use_case_t = std::pair<int, int>;
    for (auto [N, exp_v] : {
             use_case_t(4, 2),      // 4 > 2 > 1
             use_case_t(6, 2),      // 6 > 2 > 1
             use_case_t(7, 3),      // 7 > 6 > 2 > 1
             use_case_t(5, 3),      // 5 > 4 > 2 > 1
             use_case_t(19, 4),     // 19 > 18 > 6 > 2 > 1
             use_case_t(17, 5),     // 17 > 16 > 8 > 4 > 2 > 1
             use_case_t(50, 6),     // 50 > 25 > 24 > 8 > 4 > 2 > 1
             use_case_t(100, 7),    // 100 > 50 > 25 > 24 > 8 > 4 > 2 > 1
         }) {
        const auto v1 = find_min_steps_to_1_dp(N);
        std::cout << "N=" << N << ", find_min_steps_to_1_dp=" << v1 << ", " << (exp_v == v1 ? "SUCCESS" : "FAILED")
                  << std::endl;

        const auto v2 = find_min_steps_to_1_recursive(N);
        std::cout << "N=" << N << ", find_min_steps_to_1_recursive=" << v2 << ", "
                  << (exp_v == v2 ? "SUCCESS" : "FAILED") << std::endl;

        const auto v3 = find_min_steps_to_1_recursive_2(N);
        std::cout << "N=" << N << ", find_min_steps_to_1_recursive_2=" << v3 << ", "
                  << (exp_v == v3 ? "SUCCESS" : "FAILED") << std::endl;
    }
}

// https://stackoverflow.com/questions/39588554/minimum-number-of-steps-to-reduce-number-to-1
// Given any number n, and three operations on n:
// 1) Add 1;
// 2) Subtract 1;
// 3) Divide by 2 if the number is even;
//
// There is a pattern which allows you to know the optimal next step in constant time. In fact, there can be cases where
// there are two equally optimal choices -- in that case one of them can be derived in constant time.
//
// If you look at the binary representation of n, and its least significant bits, you can make some conclusions about
// which operation is leading to the solution. In short:
// - if the least significant bit is zero, then divide by 2
// - if n is 3, or the 2 least significant bits are 01, then subtract
// - In all other cases: add.
int find_min_steps_to_1_bitwise(int N)
{
    int steps = 0;
    while (N != 1) {
        if (N % 2 == 0) {
            N /= 2;
        } else if (N == 3 || N % 4 == 1) {
            N -= 1;
        } else {
            N += 1;
        }
        ++steps;
    }
    // bitwise operation
    // while (N != 1) {
    //    if ((N & 0b1) ^ 0b1) { // N % 2 == 0
    //        N /= 2;
    //    } else if ((N & 0b10) && (N != 0b11)) {
    //        N += 1;
    //    } else {
    //        N -= 1;
    //    }
    //    ++steps;
    //}
    return steps;
}
void run_find_min_steps_to_1_bitwise()
{
    using use_case_t = std::pair<int, int>;
    for (auto [N, exp_v] : {
             use_case_t(4, 2),     // 4 > 2 > 1
             use_case_t(5, 3),     // 5 > 4 > 2 > 1
             use_case_t(6, 3),     // 6 > 3 > 2 > 1
             use_case_t(7, 4),     // 7 > 6 > 3 > 2 > 1
             use_case_t(15, 5),    // 15 > 16 > 8 > 4 > 2 > 1
             use_case_t(17, 5),    // 17 > 16 > 8 > 4 > 2 > 1
             use_case_t(19, 6),    // 19 > 18 > 9 > 8 > 4 > 2 > 1
             use_case_t(62, 7),    // 62 > 31 > 32 > 16 > 8 > 4 > 2 > 1
         }) {
        const auto v = find_min_steps_to_1_bitwise(N);
        std::cout << "N=" << N << ", find_min_steps_to_1_bitwise=" << v << ", " << (exp_v == v ? "SUCCESS" : "FAILED")
                  << std::endl;
    }
}

// find all paths from root to each individual leaf nodes
void tree_all_paths(tree_node_int_t* root)
{
    if (!root) {
        return;
    }

    std::vector<tree_node_int_t*> path;
    path.push_back(root);

    auto print = [&]() {
        std::cout << "[ ";
        for (size_t i = 0; i < path.size(); ++i) {
            if (i == path.size() - 1) {
                std::cout << path[i]->data;
            } else {
                std::cout << path[i]->data << "->";
            }
        }
        std::cout << " ]" << std::endl;
    };

    std::function<void(tree_node_int_t*)> traverse = [&](tree_node_int_t* node) {
        if (!node->left && !node->right) {
            // dump path
            print();
            path.pop_back();
            return;
        }

        if (node->left) {
            path.push_back(node->left);
            traverse(node->left);
        }

        if (node->right) {
            path.push_back(node->right);
            traverse(node->right);
        }

        path.pop_back();
    };

    traverse(root);
}

void tree_all_paths_v2(tree_node_int_t* root)
{
    std::vector<tree_node_int_t*> path;
    auto                          print = [&]() {
        std::cout << "[ ";
        for (size_t i = 0; i < path.size(); ++i) {
            if (i == path.size() - 1) {
                std::cout << path[i]->data;
            } else {
                std::cout << path[i]->data << "->";
            }
        }
        std::cout << " ]" << std::endl;
    };

    std::function<void(tree_node_int_t*)> traverse = [&](tree_node_int_t* node) {
        if (!node) {
            return;
        }

        path.push_back(node);
        traverse(node->left);
        traverse(node->right);

        if (!node->left && !node->right) {
            // dump
            print();
        }
        path.pop_back();
    };

    traverse(root);
}

void run_tree_all_paths()
{
    tree_node_int_t root(100);
    tree_node_int_t n1(1);
    tree_node_int_t n2(2);
    tree_node_int_t n3(3);
    tree_node_int_t n4(4);
    tree_node_int_t n5(5);
    tree_node_int_t n6(6);
    tree_node_int_t n7(7);
    tree_node_int_t n8(8);
    tree_node_int_t n9(9);
    tree_node_int_t n0(0);

    root.left  = &n1;
    root.right = &n2;
    n1.left    = &n3;
    n1.right   = &n4;
    n2.left    = &n5;
    n2.right   = &n6;
    n3.left    = &n7;
    n3.right   = &n8;
    n4.left    = &n9;
    n4.right   = &n0;

    std::cout << "---------------------------" << std::endl;
    tree_all_paths(&root);

    std::cout << "---------------------------" << std::endl;
    tree_all_paths_v2(&root);
}

// leetcode 114: given a binary tree, change it to list
// - left child is null, right child is 'next'
// - follow pre-order traversal, left-root-right
tree_node_int_t* tree_to_list(tree_node_int_t* root)
{
    std::cout << root->data << ",";

    auto l     = root->left;
    auto r     = root->right;
    auto tail  = root;
    tail->left = nullptr;
    if (l) {
        tail->right = l;
        tail        = tree_to_list(l);
    }
    if (r) {
        tail->right = r;
        tail        = tree_to_list(r);
    }
    return tail;
}

// this is kind of BFS, each cycle put the right child to right-bottom from left branch, and put left child on right
void tree_to_list_2(tree_node_int_t* root)
{
    while (root) {
        std::cout << root->data << ",";
        if (root->left) {
            auto pre = root->left;
            while (pre->right) {
                pre = pre->right;
            }
            pre->right  = root->right;
            root->right = root->left;
            root->left  = nullptr;
        }
        root = root->right;
    }
}

void run_tree_to_list()
{
    auto create_tree = []() {
        auto root = new tree_node_int_t(100);
        auto n1   = new tree_node_int_t(1);
        auto n2   = new tree_node_int_t(2);
        auto n3   = new tree_node_int_t(3);
        auto n4   = new tree_node_int_t(4);
        auto n5   = new tree_node_int_t(5);
        auto n6   = new tree_node_int_t(6);
        auto n7   = new tree_node_int_t(7);
        auto n8   = new tree_node_int_t(8);
        auto n9   = new tree_node_int_t(9);
        auto n0   = new tree_node_int_t(0);

        root->left  = n1;
        root->right = n2;
        n1->left    = n3;
        n1->right   = n4;
        n2->left    = n5;
        n2->right   = n6;
        n3->left    = n7;
        n3->right   = n8;
        n4->left    = n9;
        n4->right   = n0;
        return root;
    };

    using use_case_t = std::tuple<tree_node_int_t*, tree_node_int_t*, std::string>;
    for (auto [root1, root2, exp_v] : {
             use_case_t(create_tree(), create_tree(), "[100->1->3->7->8->4->9->0->2->5->6]"),
         }) {
        tree_to_list(root1);
        tree_to_list_2(root2);
    }
}

// Find next greater number with same set of digits
// https://www.geeksforgeeks.org/find-next-greater-number-set-digits/
//
// Given a number N as string, find the smallest number that has same set of digits as N and is greater than N. If N is
// the greatest possible number with its set of digits, then print 'Not Possible'
//
// Examples:
// Input: 218765 Output: 251678
// Input: 1234 Output: 1243
// Input: 4321 Output: Not Possible
std::string find_next_greater_with_same_set_digits(std::string str)
{
    const auto length = str.size();
    if (length < 2) {
        return "Not Possible";
    }

    auto idx = length;
    while (--idx) {
        const auto prev = str[idx];
        const auto curr = str[idx - 1];
        if (curr >= prev)
            continue;

        for (auto i = length - 1; i >= idx; --i) {
            if (str[i] > curr) {
                str[idx - 1] = str[i];
                str[i]       = curr;
                break;
            }
        }
        size_t i = 0;
        while (idx + i < length - i - 1) {
            const auto t        = str[length - i - 1];
            str[length - i - 1] = str[idx + i];
            str[idx + i]        = t;
            ++i;
        }
        return str;
    }
    return "Not Possible";
}
void run_find_next_greater_with_same_set_digits()
{
    using use_case_t = std::pair<std::string, std::string>;
    for (auto [input, exp_v] : {
             use_case_t("2", "Not Possible"),
             use_case_t("21", "Not Possible"),
             use_case_t("4321", "Not Possible"),
             use_case_t("23", "32"),
             use_case_t("1234", "1243"),
             use_case_t("218765", "251678"),
             use_case_t("21321", "22113"),
             use_case_t("21310", "23011"),
             use_case_t("534976", "536479"),
         }) {
        const auto v = find_next_greater_with_same_set_digits(input);
        std::cout << "Input=" << input << ", find_next_greater_with_same_set_digits=" << v << ", "
                  << (exp_v == v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

template <int EXP>
constexpr double my_pow(int x)
{
    if constexpr (EXP < 0) {
        return 1 / my_pow<-EXP>(x);
    } else if constexpr (EXP == 0) {
        return 1;
    } else {
        const auto v = my_pow<EXP / 2>(x);
        if constexpr (EXP % 2 == 0) {
            return v * v;
        } else {
            return v * v * x;
        }
    }
}

constexpr double my_pow_bitwise(int x, int y)
{
    if (y < 0) {
        return 1 / my_pow_bitwise(x, -y);
    }

    double result = 1;
    while (y) {
        if (y & 0x01) {
            result *= x;
        }
        x *= x;
        y >>= 1;
    }
    return result;
}

void run_my_pow()
{
    constexpr int BASE = 2;
    static_assert(my_pow<0>(BASE) == 1);
    static_assert(my_pow<1>(BASE) == 2);
    static_assert(my_pow<2>(BASE) == 4);
    static_assert(my_pow<3>(BASE) == 8);
    static_assert(my_pow<4>(BASE) == 16);
    static_assert(my_pow<5>(BASE) == 32);
    static_assert(my_pow<-1>(BASE) == 0.5);
    static_assert(my_pow<-2>(BASE) == 0.25);
    static_assert(my_pow<-3>(BASE) == 0.125);

    static_assert(my_pow_bitwise(BASE, 0) == 1);
    static_assert(my_pow_bitwise(BASE, 1) == 2);
    static_assert(my_pow_bitwise(BASE, 2) == 4);
    static_assert(my_pow_bitwise(BASE, 3) == 8);
    static_assert(my_pow_bitwise(BASE, 4) == 16);
    static_assert(my_pow_bitwise(BASE, 5) == 32);
    static_assert(my_pow_bitwise(BASE, -1) == 0.5);
    static_assert(my_pow_bitwise(BASE, -2) == 0.25);
    static_assert(my_pow_bitwise(BASE, -3) == 0.125);
}

// Minimum time taken by each job to be completed given by a directed graph
// https://www.geeksforgeeks.org/minimum-time-taken-by-each-job-to-be-completed-given-by-a-directed-acyclic-graph/
//
// Assume jobs tagged by [0, N)
std::vector<int> job_compelte_time(int N, const std::vector<std::array<int, 2>>& edges)
{
    std::vector<int> result(N, 0);

    // for specific job i, the number of jobs it depends on
    std::vector<int> deps(N, 0);

    // for specific job i, all jobs that depend on i
    std::vector<std::vector<int>> next_jobs(N, std::vector<int>{});

    for (const auto edge : edges) {
        const auto from = edge[0];
        const auto to   = edge[1];
        next_jobs[from].push_back(to);
        ++deps[to];
    }

    // store jobs that are ready to run
    std::queue<int> q;
    for (int i = 0; i < N; ++i) {
        if (deps[i] == 0) {
            q.push(i);
            result[i] = 1;    // takes 1 unit time to finish jobs that depend on none others
        }
    }

    while (!q.empty()) {
        const auto job = q.front();
        q.pop();

        for (auto next_job : next_jobs[job]) {
            if (--deps[next_job] == 0) {
                // push 'next_job' that has all its dependencies done at this point
                q.push(next_job);

                // this 'next_job' takes extra 1 unit time compare to latest dependency
                result[next_job] = result[job] + 1;
            }
        }
    }
    return result;
}
void run_job_compelte_time()
{
    using use_case_t = std::tuple<int, std::vector<std::array<int, 2>>, std::string>;
    for (const auto& [jobs, edges, exp_v] : {
             use_case_t(
                 10,
                 {
                     {0, 2},
                     {0, 3},
                     {0, 4},
                     {1, 2},
                     {1, 7},
                     {1, 8},
                     {2, 5},
                     {3, 5},
                     {3, 7},
                     {4, 7},
                     {5, 6},
                     {6, 7},
                     {7, 9},
                 },
                 "[1,1,2,2,2,3,4,5,2,6]"),
             use_case_t(
                 7,
                 {
                     {0, 1},
                     {1, 2},
                     {1, 3},
                     {1, 4},
                     {2, 5},
                     {3, 5},
                     {4, 6},
                 },
                 "[1,2,3,3,3,4,4]"),
         }) {
        const auto v = dump_array(job_compelte_time(jobs, edges));
        std::cout << "jobs=" << jobs << ", job_compelte_time=" << v << ", " << (exp_v == v ? "SUCCESS" : "FAILED")
                  << std::endl;
    }
}

// Enhanced version: each jobs finish at different time cost
std::vector<int>
enhanced_job_compelte_time(int N, const std::vector<int>& jobs_time_cost, const std::vector<std::array<int, 2>>& edges)
{
    if (N != static_cast<int>(jobs_time_cost.size()))
        return {};

    std::vector<int> result(N, 0);

    // for specific job i, the number of jobs it depends on
    std::vector<int> deps(N, 0);

    // for specific job i, all jobs that depend on i
    std::vector<std::vector<int>> next_jobs(N, std::vector<int>{});

    for (const auto edge : edges) {
        const auto from = edge[0];
        const auto to   = edge[1];
        next_jobs[from].push_back(to);
        ++deps[to];
    }

    // store jobs that are running and are ready to run
    std::queue<int> q;
    for (int i = 0; i < N; ++i) {
        if (deps[i] == 0) {
            q.push(i);
            result[i] = jobs_time_cost[i];    // takes x unit time to finish this job
        }
    }

    // job time cost <> job, use multimap since jobs may have same time cost
    //
    // at some specific time point, we shall collect all ready-to-run jobs ordered by time cost before adding them to
    // the queue; this 'adding' action happens when either
    // 1) queue is empty; or
    // 2) top job in the queue has different ending time from current finished job
    std::multimap<int, int> jobs_ordered_by_cost;
    while (!q.empty()) {

        std::cout << "top job=" << q.front() << ", size=" << q.size() << ", result=" << dump_array(result) << std::endl;

        const auto job = q.front();
        q.pop();

        for (auto next_job : next_jobs[job]) {
            if (--deps[next_job] == 0) {
                // keep ready-to-run jobs ordered by cost to finish, as in turn they are in desired order in queue
                jobs_ordered_by_cost.emplace(jobs_time_cost[next_job], next_job);

                // we now know when this 'next_job' will finish
                result[next_job] = result[job] + jobs_time_cost[next_job];
            }
        }

        if (q.empty() || result[job] != result[q.front()]) {
            // push 'next_job' that has all its dependencies done at this point
            for (auto [_, next_job] : jobs_ordered_by_cost) {
                q.push(next_job);
            }
            jobs_ordered_by_cost.clear();
        }
    }

    return result;
}
void run_enhanced_job_compelte_time()
{
    // [N, jobs_time_cost, jobs_deps]
    using use_case_t = std::tuple<int, std::vector<int>, std::vector<std::array<int, 2>>, std::string>;

    for (const auto& [N, jobs_time_cost, jobs_deps, exp_v] : {
             use_case_t{
                 10,
                 {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                 {
                     {0, 2},
                     {0, 3},
                     {0, 4},
                     {1, 2},
                     {1, 7},
                     {1, 8},
                     {2, 5},
                     {3, 5},
                     {3, 7},
                     {4, 7},
                     {5, 6},
                     {6, 7},
                     {7, 9},
                 },
                 "[1,1,2,2,2,3,4,5,2,6]"},
             use_case_t{
                 7,
                 {1, 1, 1, 1, 1, 1, 1},
                 {
                     {0, 1},
                     {1, 2},
                     {1, 3},
                     {1, 4},
                     {2, 5},
                     {3, 5},
                     {4, 6},
                 },
                 "[1,2,3,3,3,4,4]"},
             use_case_t{
                 7,
                 {2, 1, 3, 2, 2, 5, 1},
                 {
                     {0, 1},
                     {1, 2},
                     {1, 3},
                     {1, 4},
                     {2, 5},
                     {3, 5},
                     {4, 6},
                 },
                 "[2,3,6,5,5,11,6]"},
             use_case_t{
                 5,
                 {1, 1, 3, 3, 2},
                 {
                     {0, 1},
                     {0, 2},
                     {1, 3},
                     {3, 4},
                     {2, 4},
                 },
                 "[1,2,4,5,7]"},
         }) {
        const auto v = dump_array(enhanced_job_compelte_time(N, jobs_time_cost, jobs_deps));
        std::cout << "N=" << N << ", enhanced_job_compelte_time=" << v << ", " << (exp_v == v ? "SUCCESS" : "FAILED")
                  << std::endl;
    }
}

// Largest Rectangular Area in a Histogram
//
// https://www.geeksforgeeks.org/largest-rectangular-area-in-a-histogram-using-stack/
//
// Find the largest rectangular area possible in a given histogram where the largest rectangle can be made of a number
// of contiguous bars whose heights are given in an array. For simplicity, assume that all bars have the same width and
// the width is 1 unit.
//
// Simple approach: O(n^2), loop the array, for specific height, traverse left & right and find max possible width
//
// Optimal approach: O(n), use stack to track the height, as below
int max_rectangle(const std::vector<int>& heights)
{
    // this stack tracks the index whose heights keep increasing
    std::stack<int> stk;
    stk.push(0);

    int result = 0;
    for (int i = 1; i < static_cast<int>(heights.size()); ++i) {
        while (!stk.empty() && heights[stk.top()] > heights[i]) {
            const auto idx = stk.top();
            stk.pop();

            // given height at 'idx', find the max width which spans from 'i' and 'stk.top()'
            const auto width = (stk.empty() ? i : (i - stk.top() - 1));
            result           = std::max(result, heights[idx] * width);
        }

        stk.push(i);
    }

    // this step is to cover the leftover items like the right-most ones
    const int idx_last = stk.empty() ? -1 : stk.top();
    while (!stk.empty()) {
        const auto idx = stk.top();
        stk.pop();

        const auto width = (stk.empty() ? (idx_last + 1) : (idx_last - stk.top()));
        result           = std::max(result, heights[idx] * width);
    }
    return result;
}
void run_max_rectangle()
{
    using use_case_t = std::pair<std::vector<int>, int>;
    for (auto& [heights, exp_v] : {
             use_case_t({60, 20, 50, 40, 10, 50, 60}, 100),
             use_case_t({3, 5, 1, 7, 5, 9}, 15),
             use_case_t({2, 3, 3, 2}, 8),
         }) {
        const auto v = max_rectangle(heights);
        std::cout << "Heights: " << dump_array(heights) << ", max_rectangle=" << v << ", "
                  << (exp_v == v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

// Maximum size rectangle binary sub-matrix with all 1s
// https://www.geeksforgeeks.org/maximum-size-rectangle-binary-sub-matrix-1s/
//
// Given a 2d binary matrix mat[][], the task is to find the maximum size rectangle binary-sub-matrix with all 1’s.
//
// Simple approach: loop row by row, then find consective width at matrix[row][col] per row; at specific
// matrix[row][col], loop from row to 0, find the max width and get the area. O((m^2) * n)
//
// Optimal approach: the idea is to consider each row as the base of histogram, and then use the same approach to find
// the maximum size rectangle in the matrix. O(m * n)
int max_rectangle_sub_matrix(const std::vector<std::vector<int>>& matrix)
{
    int              result = 0;
    std::vector<int> heights(matrix[0].size(), 0);
    for (size_t idx_row = 0; idx_row < matrix.size(); ++idx_row) {
        for (size_t idx_col = 0; idx_col < matrix[0].size(); ++idx_col) {
            if (matrix[idx_row][idx_col]) {
                ++heights[idx_col];
            } else {
                heights[idx_col] = 0;
            }
        }
        result = std::max(result, max_rectangle(heights));
    }
    return result;
}
void run_max_rectangle_sub_matrix()
{
    using use_case_t = std::pair<std::vector<std::vector<int>>, int>;
    for (auto& [matrix, exp_v] : {
             use_case_t(
                 {
                     {0, 1, 1, 0},
                     {1, 1, 1, 1},
                     {1, 1, 1, 1},
                     {1, 1, 0, 0},
                 },
                 8),

             use_case_t(
                 {
                     {0, 1, 1},
                     {1, 1, 1},
                     {0, 1, 1},
                 },
                 6),
         }) {
        const auto v = max_rectangle_sub_matrix(matrix);
        std::cout << "matrix=" << dump_matrix(matrix) << ", max_rectangle_sub_matrix=" << v << ", "
                  << (exp_v == v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

// Maximum size square sub-matrix with all 1s
// https://www.geeksforgeeks.org/maximum-size-sub-matrix-with-all-1s-in-a-binary-matrix/
//
// Given a binary matrix mat of size n * m, the task is to find out the maximum length of a side of a square sub-matrix
// with all 1s.
//
// Idea: at any [i, j], the max square is min(square[i-1][j], square[i][j-1], square[i-1][j-1])
// O(m * n)
//
int max_square_sub_matrix(std::vector<std::vector<int>> matrix)
{
    int result = 0;
    for (size_t idx_row = 1; idx_row < matrix.size(); ++idx_row) {
        for (size_t idx_col = 1; idx_col < matrix[0].size(); ++idx_col) {
            if (matrix[idx_row][idx_col]) {
                matrix[idx_row][idx_col] = 1 + std::min(
                                                   {matrix[idx_row - 1][idx_col], matrix[idx_row][idx_col - 1],
                                                    matrix[idx_row - 1][idx_col - 1]});
                result = std::max(result, matrix[idx_row][idx_col]);
            }
        }
    }
    return result;
}
void run_max_square_sub_matrix()
{
    using Matrix = std::vector<std::vector<int>>;
    for (auto& matrix : {
             Matrix{
                 {1, 1},
                 {1, 1},
             },    // 2

             Matrix{
                 {0, 1, 1, 0},
                 {1, 1, 1, 1},
                 {1, 1, 1, 1},
                 {1, 1, 0, 0},
             },    // 2

             Matrix{
                 {0, 1, 1},
                 {1, 1, 1},
                 {0, 1, 1},
             },    // 2

             Matrix{
                 {0, 1, 1, 0, 1},
                 {1, 1, 0, 1, 0},
                 {0, 1, 1, 1, 0},
                 {1, 1, 1, 1, 0},
                 {1, 1, 1, 1, 1},
                 {0, 0, 0, 0, 0},
             },    // 3
         }) {
        std::cout << "Max-square-sub-matrix=" << max_square_sub_matrix(matrix) << std::endl;
    }
}

// Very good explaination & code:
// https://www.w3schools.com/dsa/dsa_algo_graphs_cycledetection.php
//
// Below code can be generated by google: c++ graph check cycle
bool graph_undirected_cycle(int N, const std::vector<std::array<int, 2>>& edges)
{
    std::vector<bool> visited(N, false);

    std::vector<std::vector<int>> children(N, std::vector<int>{});
    for (const auto edge : edges) {
        const auto from = edge[0];
        const auto to   = edge[1];
        // undirected graph
        children[from].push_back(to);
        children[to].push_back(from);
    }

    using lambda_t         = std::function<bool(int, int)>;
    lambda_t dfs_has_cycle = [&](int idx, int idx_parent) {
        if (visited[idx])
            return false;

        visited[idx] = true;

        for (auto child : children[idx]) {
            if (!visited[child]) {
                if (dfs_has_cycle(child, idx)) {
                    return true;
                }
            }
            // TODO: not sure why need to check (child != idx_parent)??
            else if (child != idx_parent) {
                return true;
            }
        }
        return false;
    };

    for (int i = 0; i < N; ++i) {
        if (dfs_has_cycle(i, -1))
            return true;
    }
    return false;
}

// Detect cycle in directed graph
// https://www.geeksforgeeks.org/detect-cycle-in-directed-graph-using-topological-sort/
//
// Approach 1: start from 0-indegree vertices, traverse the graph by dfs and keep the visited vertices in stack, ignore
// those visited ones; loop the stack and compare to indegree list, the parent vertex should be traversed before its
// children, otherwise cycle exists
//
// Approach 2: detect the cycle during dfs, the key is we need a 'on_route' array that tracks vertices in current
// recursive path O(N + M), where N is number of vertices, M is number of edges
//
bool graph_directed_cycle(int N, const std::vector<std::array<int, 2>>& edges)
{
    std::vector<bool> visited(N, false);
    std::vector<bool> on_route(N, false);

    std::vector<std::vector<int>> children(N, std::vector<int>{});
    for (const auto edge : edges) {
        const auto from = edge[0];
        const auto to   = edge[1];
        children[from].push_back(to);
    }

    using lambda_t         = std::function<bool(int)>;
    lambda_t dfs_has_cycle = [&](int idx) {
        if (visited[idx])
            return false;

        visited[idx]  = true;
        on_route[idx] = true;

        for (auto child : children[idx]) {
            // if child vertex is not yet visited, do depth-first-search with it
            if (!visited[child]) {
                if (dfs_has_cycle(child)) {
                    return true;
                }
            }
            // if child is already visited and it is in current recrusive path, a cycle exists
            else if (on_route[child]) {
                return true;
            }
        }

        // this on_route[..] tracks vertices in current recursive path, since same vertex may exist in multiple paths
        // (i.e. multi 0-degree vertices), we should reset here before checking another route; however the visited[..]
        // doesn't need reset
        on_route[idx] = false;
        return false;
    };

    for (int i = 0; i < N; ++i) {
        if (dfs_has_cycle(i))
            return true;
    }
    return false;
}
void run_graph_directed_cycle()
{
    using use_case_t = std::tuple<int, std::vector<std::array<int, 2>>, bool>;
    for (const auto& [vertices, edges, exp_v] : {
             use_case_t{
                 2,
                 {
                     {0, 1},
                     {1, 0},
                 },
                 true},
             use_case_t{
                 3,
                 {
                     {0, 1},
                     {1, 2},
                     {2, 0},
                 },
                 true},
             use_case_t{
                 5,
                 {
                     {0, 1},
                     {1, 2},
                     {2, 3},
                     {2, 4},
                     {3, 1},
                 },
                 true},
             use_case_t{
                 4,
                 {
                     {0, 1},
                     {1, 2},
                     {2, 0},
                     {0, 2},
                     {2, 3},
                     {3, 3},
                 },
                 true},
             use_case_t{
                 4,
                 {
                     {0, 1},
                     {1, 2},
                     {2, 3},
                     {0, 2},
                 },
                 false},
             use_case_t{
                 10,
                 {
                     {0, 2},
                     {0, 3},
                     {0, 4},
                     {1, 2},
                     {1, 7},
                     {1, 8},
                     {2, 5},
                     {3, 5},
                     {3, 7},
                     {4, 7},
                     {5, 6},
                     {6, 7},
                     {7, 9},
                 },
                 false},
             use_case_t{
                 7,
                 {
                     {0, 1},
                     {1, 2},
                     {1, 3},
                     {1, 4},
                     {2, 5},
                     {3, 5},
                     {4, 6},
                 },
                 false},
         }) {
        const auto v = graph_directed_cycle(vertices, edges);
        std::cout << "vertices=" << vertices << ", graph_directed_cycle=" << std::boolalpha << v << ", "
                  << (exp_v == v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

// Given an integer n, the task is to find the first n rows of Pascal's triangle.
//
// https://www.geeksforgeeks.org/dsa/pascal-triangle/
std::vector<int> find_pascal_triangle_row(int rowIdx)
{
    std::vector<int> result(rowIdx + 1, 1);
    for (int i = 1; i < rowIdx; ++i) {
        for (int j = i; j > 0; --j) {
            result[j] = result[j - 1] + result[j];
        }
    }
    return result;
}
std::vector<int> find_pascal_triangle_row_2(int rowIdx)
{
    std::vector<int> result(rowIdx + 1, 1);
    for (int i = 1; i < rowIdx; ++i) {
        // NOTICE: nice formula!!!
        result[i] = result[i - 1] * (rowIdx + 1 - i) / i;
    }
    return result;
}

void run_find_pascal_triangle_row()
{
    for (auto rowIdx : {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}) {
        std::cout << "rowIdx=" << rowIdx << ", row=" << dump_array(find_pascal_triangle_row(rowIdx)) << ", "
                  << dump_array(find_pascal_triangle_row_2(rowIdx)) << std::endl;
    }
}

// Given an integer n, return all the numbers in the range [1, n] sorted in lexicographical order.
//
// n = 13,  [1, 10, 11, 12, 13, 2, 3, 4, 5, 6, 7, 8, 9]
// n = 200, [1, 10, 100, 101, ..., 109, 11, 110, 111, ..., 119, 12, ...]

// TODO: revisit this code...
std::vector<int> find_nums_in_lexical_order(int n)
{
    std::vector<int> ans;
    int              v = 1;
    for (int i = 0; i < n; ++i) {
        ans.push_back(v);
        // 1) for the (1, 10, 100) alike part
        // TODO: not loop??
        if (v * 10 <= n) {
            v *= 10;
        } else {
            // 2) for (109, 11, 110) alike part; and
            // 3) boundary check;
            while (v % 10 == 9 || v + 1 > n) {
                v /= 10;
            }
            // 4) regular forward part (101, 102, ..., 109)
            ++v;
        }
    }
    return ans;
}

// Given a matrix with n rows and m columns, find the length of the longest increasing path in the matrix.
// From each cell, we can either move in four directions: left, right, up, or down.
//
// https://www.geeksforgeeks.org/dsa/longest-increasing-path-matrix/
int find_longest_increasing_path(const std::vector<std::vector<int>>& matrix)
{
    const std::vector<int>        dirs = {-1, 0, 1, 0, -1};
    const int                     m = matrix.size(), n = matrix[0].size();
    std::vector<std::vector<int>> cache(m, std::vector<int>(n, 0));

    std::function<int(int, int)> dfs = [&](int i, int j) -> int {
        if (cache[i][j]) {
            return cache[i][j];
        }
        for (int k = 0; k < 4; ++k) {
            int x = i + dirs[k], y = j + dirs[k + 1];
            // NOTICE: by using cache, we don't worry about duplicate visit and/or dead-loop in current route (which is
            // typically avoided with 'visited' alike array), because of condition matrix[x][y] > matrix[i][j]
            if (x >= 0 && x < m && y >= 0 && y < n && matrix[x][y] > matrix[i][j]) {
                cache[i][j] = std::max(cache[i][j], dfs(x, y));
            }
        }
        return ++cache[i][j];
    };

    int ans = 0;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            ans = std::max(ans, dfs(i, j));
        }
    }
    return ans;
}

int find_longest_increasing_path_2(const std::vector<std::vector<int>>& matrix)
{
    const int                        n   = matrix.size();
    const int                        m   = matrix[0].size();
    std::vector<std::pair<int, int>> dir = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    std::vector<std::vector<int>>    degree(n + 1, std::vector<int>(m + 1, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (auto it : dir) {
                int x = i + it.first;
                int y = j + it.second;
                // If neighboring cell is within bounds and has a smaller value
                if (x >= 0 && x < n && y >= 0 && y < m && matrix[x][y] < matrix[i][j]) {
                    degree[i][j]++;
                }
            }
        }
    }

    // Add all cells with in-degree 0 to the queue (they are starting points)
    std::queue<std::pair<int, int>> q;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (degree[i][j] == 0)
                q.push({i, j});

    // This will count the number of levels processed (i.e., length of path)
    int ans = 0;

    // Perform BFS level-by-level
    while (!q.empty()) {
        ans++;

        const int len = q.size();
        for (int i = 0; i < len; i++) {
            int x1 = q.front().first;
            int y1 = q.front().second;
            q.pop();

            // Explore all four directions
            for (auto it : dir) {
                int x = x1 + it.first;
                int y = y1 + it.second;

                // If neighboring cell has greater value (valid increasing move)
                if (x >= 0 && x < n && y >= 0 && y < m && matrix[x][y] > matrix[x1][y1]) {
                    if (--degree[x][y] == 0) {
                        q.push({x, y});
                    }
                }
            }
        }
    }
    // Return the total number of BFS levels = length of the longest increasing path
    return ans;
}

int max_sub_array(const std::vector<int>& arr)
{
    if (arr.empty()) {
        return 0;
    }

    int max_excl = 0;
    int max_incl = arr[0];
    for (size_t i = 1; i < arr.size(); ++i) {
        max_excl = std::max(max_incl, max_excl);
        max_incl = std::max(arr[i], max_incl + arr[i]);
    }
    return std::max(max_incl, max_excl);
}
void run_max_sub_array()
{
    using use_case_t = std::tuple<std::vector<int>, int>;
    for (const auto& [arr, exp_v] : {
             use_case_t{{2, 3, -8, 7, -1, 2, 3}, 11},
             use_case_t{{-2, -4}, -2},
             use_case_t{{5, 4, 1, 7, 8}, 25},
         }) {
        const auto v = max_sub_array(arr);
        std::cout << "array=" << dump_array(arr) << ", max_sub_array=" << v << ", "
                  << (exp_v == v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

// https://www.geeksforgeeks.org/dsa/iterative-letter-combinations-of-a-phone-number/
//
// Given an integer array arr[] containing digits from [0, 9], the task is to print all possible letter combinations
// that the numbers could represent. A mapping of digits to letters (just like on the telephone buttons) is being
// followed. Note that 0 and 1 do not map to any letters.
//
// Time O(4^n), space O(4^n)
std::vector<std::string> possible_words(const std::vector<int>& arr)
{
    std::vector<std::string> mapping = {
        "", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz",
    };

    std::vector<std::string>    result;
    std::string                 word;
    std::function<void(size_t)> impl = [&](auto idx) {
        while (idx < arr.size() && mapping[arr[idx]].empty()) {
            ++idx;
        }

        if (idx >= arr.size()) {
            result.push_back(word);
            return;
        }

        for (auto ch : mapping[arr[idx]]) {
            word.push_back(ch);
            impl(idx + 1);
            word.pop_back();
        }
    };

    impl(0);

    return result;
}
void run_possible_words()
{
    for (const auto& arr : {
             std::vector{2, 3},
             std::vector{5},
             std::vector{1, 2, 8, 9, 0},
         }) {
        std::cout << "array=" << dump_array(arr) << ", possible_words=" << dump_array(possible_words(arr)) << std::endl;
    }
}

// Given an absolute path for a file (Unix-style), simplify it. Note that absolute path always begin with ‘/’ ( root
// directory ), a dot in path represent current directory and double dot represents parent directory.
std::string simplify_dir_path(const std::string& path)
{
    std::stack<std::string> result;
    auto                    lvl = 0;
    auto                    idx = path.size();
    while (idx-- > 0) {
        auto pos = path.rfind('/', idx);
        if (pos == std::string::npos) {
            break;
        }

        const auto v = path.substr(pos + 1, idx - pos);
        if (v.empty()) {
        } else if (v == ".") {
            // ignore
        } else if (v == "..") {
            ++lvl;
        } else if (lvl > 0) {
            --lvl;
        } else {
            result.push(v);
        }
        idx = pos;
    }
    if (lvl > 0 || result.empty()) {
        return "/";
    }

    std::string ans;
    while (!result.empty()) {
        ans += "/" + result.top();
        result.pop();
    }
    return ans;
}

void run_simplify_dir_path()
{
    using use_case_t = std::tuple<std::string, std::string>;
    for (auto [p, exp_v] : {
             use_case_t("/home/", "/home"),
             use_case_t("/a/./b/../../c/", "/c"),
             use_case_t("/a/..", "/"),
             use_case_t("/a/../", "/"),
             use_case_t("/../../../../../a", "/a"),
             use_case_t("/a/./b/./c/./d/", "/a/b/c/d"),
             use_case_t("/a/../.././../../.", "/"),
             use_case_t("/a//b//c//////d", "/a/b/c/d"),
         }) {
        const auto v = simplify_dir_path(p);
        std::cout << "raw_path=" << p << ", simplify_dir_path=" << v << ", " << (exp_v == v ? "SUCCESS" : "FAILED")
                  << std::endl;
    }
}

std::vector<int> merge_arrays(const std::vector<int>& arr1, const std::vector<int>& arr2, const std::vector<int>& arr3)
{
    size_t           idx_1 = 0;
    size_t           idx_2 = 0;
    size_t           idx_3 = 0;
    std::vector<int> result;
    constexpr auto   Max = std::numeric_limits<int>::max();
    while (idx_1 < arr1.size() || idx_2 < arr2.size() || idx_3 < arr3.size()) {
        const auto v1 = idx_1 >= arr1.size() ? Max : arr1[idx_1];
        const auto v2 = idx_2 >= arr2.size() ? Max : arr2[idx_2];
        const auto v3 = idx_3 >= arr3.size() ? Max : arr3[idx_3];
        const auto v  = std::min({v1, v2, v3});
        if (result.empty() || v != result.back()) {
            result.push_back(v);
        }
        idx_1 += (v1 == result.back());
        idx_2 += (v2 == result.back());
        idx_3 += (v3 == result.back());
    }
    return result;
}
std::vector<int> merge_arrays_k(const std::vector<std::vector<int>>& arrs)
{
    struct element_t {
        size_t                  idx;
        const std::vector<int>* arr;
    };
    auto comp = [](const element_t& lhs, const element_t& rhs) {
        return lhs.arr->at(lhs.idx) > rhs.arr->at(rhs.idx);
    };
    std::priority_queue<element_t, std::vector<element_t>, decltype(comp)> pq;
    for (auto& arr : arrs) {
        pq.push({0, &arr});
    }

    std::vector<int> result;
    while (!pq.empty()) {
        auto& p = pq.top();
        auto& v = p.arr->at(p.idx);
        if (result.empty() || result.back() != v) {
            result.push_back(v);
        }
        pq.pop();
        if (p.idx + 1 < p.arr->size()) {
            pq.push({p.idx + 1, p.arr});
        }
    }
    return result;
}
void run_merge_arrays()
{
    auto arr1 = std::vector<int>{1, 2, 3, 3, 4};
    auto arr2 = std::vector<int>{-1, 2, 3, 5};
    auto arr3 = std::vector<int>{4, 5, 6, 7, 8, 9};
    auto r1   = merge_arrays(arr1, arr2, arr3);
    auto r2   = merge_arrays_k({std::vector<int>{arr1}, std::vector<int>{arr2}, std::vector<int>{arr3}});
    std::cout << "arr1=" << dump_array(arr1) << ", arr2=" << dump_array(arr2) << ", arr3=" << dump_array(arr3)
              << ", merge_arrays=" << dump_array(r1) << ", merge_arrays_k=" << dump_array(r2) << std::endl;
}

// https://www.geeksforgeeks.org/dsa/convert-binary-tree-to-doubly-linked-list-by-keeping-track-of-visited-node/
//
// Convert binary tree into double linked list so that node's left child is previous node and right child is next node
// The list is inorder: left > root > right
tree_node_int_t* binary_tree_2_double_list(tree_node_int_t* root)
{
    using return_t                                 = std::pair<tree_node_int_t*, tree_node_int_t*>;
    std::function<return_t(tree_node_int_t*)> impl = [&](tree_node_int_t* node) {
        if (!node) {
            return return_t{nullptr, nullptr};
        }
        auto [l_head, l_tail] = impl(node->left);
        node->left            = l_tail;
        if (l_tail) {
            l_tail->right = node;
        }

        auto [r_head, r_tail] = impl(node->right);
        node->right           = r_head;
        if (r_head) {
            r_head->left = node;
        }

        auto head = l_head ? l_head : node;
        auto tail = r_tail ? r_tail : node;
        return return_t{head, tail};
    };
    auto [head, _] = impl(root);
    return head;
}
void run_binary_tree_2_double_list()
{
    auto root         = new tree_node_int_t(10);
    root->left        = new tree_node_int_t(12);
    root->right       = new tree_node_int_t(15);
    root->left->left  = new tree_node_int_t(25);
    root->left->right = new tree_node_int_t(30);
    root->right->left = new tree_node_int_t(36);

    auto result = binary_tree_2_double_list(root);
    while (result) {
        if (result->left) {
            std::cout << "(" << result->left->data << ")";
        }
        std::cout << result->data;
        result = result->right;
        if (result) {
            std::cout << "(" << result->data << ")" << " -> ";
        }
    }
    std::cout << std::endl;
}

// https://www.geeksforgeeks.org/dsa/check-if-a-given-graph-is-bipartite-using-dfs/
//
// Given a graph with V vertices numbered from 0 to V-1 and a list of edges, determine whether the graph is bipartite or
// not.
//
// Note: A bipartite graph is a type of graph where the set of vertices can be divided into two disjoint sets, say
// U and V, such that every edge connects a vertex in U to a vertex in V, there are no edges between vertices within the
// same set.
//
// For given adj_list, adj_list[i] tells the neighbors of vertex i.
bool graph_biparties_check(const std::vector<std::vector<int>>& adj_list)
{
    const int        N = adj_list.size();
    std::vector<int> groups(N, -1);

    std::function<bool(int)> dfs = [&](int this_vertex) {
        const auto this_group = groups[this_vertex];
        for (auto neighbor : adj_list[this_vertex]) {
            if (groups[neighbor] == this_group) {
                return false;
            }
            if (groups[neighbor] == -1) {
                groups[neighbor] = !this_group;
                if (!dfs(neighbor)) {
                    return false;
                }
            }
        }
        return true;
    };

    const int default_group = 0;

    for (int vertex = 0; vertex < N; ++vertex) {
        if (groups[vertex] == -1) {
            groups[vertex] = default_group;
            if (!dfs(vertex)) {
                return false;
            }
        }
    }
    return true;
}
void run_graph_biparties_check()
{
    using use_case_t = std::tuple<std::vector<std::vector<int>>, bool>;
    for (const auto& [adj_list, exp_v] : {
             use_case_t({{1, 2}, {0, 2}, {0, 1}, {2}}, false),
             use_case_t({{1}, {0, 2}, {1, 3}, {2}}, true),
         }) {
        const auto v = graph_biparties_check(adj_list);
        std::cout << "graph=" << dump_matrix(adj_list) << ", graph_biparties_check=" << std::boolalpha << v << ", "
                  << (exp_v == v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

//
// https://leetcode.com/discuss/post/742523/facebook-prep-question-contiguous-subarr-uzkf/
//
// You are given an array arr of N integers. For each index i, you are required to determine the number of contiguous
// subarrays that fulfill the following conditions:
// 1) The value at index i must be the maximum element in the contiguous subarrays, and
// 2) These contiguous subarrays must either start from or end on index i.
//
// Example:
// Input = [3, 4, 1, 6, 2]
// Output = [1, 3, 1, 5, 1]
// Explanation:
// index 0 - [3] is the only contiguous subarray
// index 1 - [4], [3, 4], [4, 1]
// index 2 - [1]
// index 3 - [6], [6, 2], [1, 6], [4, 1, 6], [3, 4, 1, 6]
// index 4 - [2]
// So, the answer for the above input is [1, 3, 1, 5, 1]
//
// O(n), O(n)
std::vector<int> contiguous_sub_array(const std::vector<int>& arr)
{
    const int        N = arr.size();
    std::vector<int> result(N, 1);

    // from left to right, stack tracks all elements bigger than current arr[i], and the stack is sorted
    std::stack<int> st;
    for (int i = 0; i < N; ++i) {
        // pop top from stack if it is smaller than current arr[i]
        while (!st.empty() && arr[i] > arr[st.top()]) {
            st.pop();
        }
        // add to result[i] the possible number of subarrays on the left side
        result[i] += i - (st.empty() ? 0 : (st.top() + 1));
        st.push(i);
    }

    // from right to left, stack tracks all elements bigger than current arr[i], and the stack is sorted
    std::stack<int> dummy;
    st.swap(dummy);
    for (int i = N - 1; i >= 0; --i) {
        while (!st.empty() && arr[i] > arr[st.top()]) {
            st.pop();
        }
        // add to result[i] the possible number of subarrays on the right side
        result[i] += (st.empty() ? N : st.top()) - i - 1;
        st.push(i);
    }

    return result;
}
void run_contiguous_sub_array()
{
    using use_case_t = std::tuple<std::vector<int>, std::string>;
    for (const auto& [arr, exp_v] : {
             use_case_t({1, 2, 3, 4}, "[1,2,3,4]"),
             use_case_t({1, 4, 2, 3}, "[1,4,1,2]"),
             use_case_t({3, 4, 1, 6, 2}, "[1,3,1,5,1]"),
             use_case_t({2, 4, 7, 1, 5, 3}, "[1,2,6,1,3,1]"),
         }) {
        const auto v = dump_array(contiguous_sub_array(arr));
        std::cout << "Input: " << dump_array(arr) << ", contiguous_sub_array=" << v << ", "
                  << (exp_v == v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

// Given a list of n integers, determine the number of different pairs of elements within it which sum to k.
//
// If an integer appears in the list multiple times, each copy is considered to be different; that is, two pairs are
// considered different if one pair includes at least one array index which the other doesn't, even if they include the
// same values.
//
int number_sum_k(const std::vector<int>& arr, int k)
{
    std::unordered_map<int, int> number_freq;
    for (int i = 0; i < (int)arr.size(); ++i) {
        ++number_freq[arr[i]];
    }
    int count = 0;
    for (auto v : arr) {
        if (auto iter = number_freq.find(k - v); iter != number_freq.end()) {
            count += iter->second;
            // this is the fun part, take out of self...
            if (iter->first == v) {
                --count;
            }
        }
    }
    // the other fun part, each valid pair was counted twice during above FULL loop
    count /= 2;
    return count;
}
void run_number_sum_k()
{
    using use_case_t = std::tuple<std::vector<int>, int, int>;
    for (const auto& [arr, k, exp_v] : {
             use_case_t({1, 2, 3, 4, 3}, 6, 2),
             use_case_t({1, 5, 3, 3, 3}, 6, 4),
             use_case_t({1, 1, 2, 2, 2, 3, 3, 3, 3}, 1, 0),
             use_case_t({1, 1, 2, 2, 2, 3, 3, 3, 3}, 2, 1),
             use_case_t({1, 1, 2, 2, 2, 3, 3, 3, 3}, 3, 6),
             use_case_t({1, 1, 2, 2, 2, 3, 3, 3, 3}, 4, 11),
             use_case_t({1, 1, 2, 2, 2, 3, 3, 3, 3}, 5, 12),
             use_case_t({1, 1, 1, 1, 1, 1}, 2, 15),
         }) {
        const auto ans = number_sum_k(arr, k);
        std::cout << "Array=" << dump_array(arr) << ", k=" << k << ", number_sum_k=" << ans << ", "
                  << (ans == exp_v ? "SUCCESS" : "FAILED") << std::endl;
    }
}

