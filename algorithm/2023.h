#ifndef ALGO_2023_H
#define ALGO_2023_H

#include "../main/header.h"
#include "../main/utility.h"
#include "../main/node.h"
#include <utility>

///////////////////////////////////////////////////////////////////////////////
//
// given two binary arrays, calculate the sum in binary format
//
void sum_binary(const char* arr1, uint32_t len1, const char* arr2, uint32_t len2)
{
    uint32_t len    = (len1 > len2 ? len1 : len2) + 1;
    char*    result = new char[len];
    bool     carry  = false;
    for (uint32_t n = 0; n < len; ++n) {
        char ch1 = '0';
        char ch2 = '0';
        if (n < len1) {
            ch1 = *(arr1 + len1 - n - 1);
        }
        if (n < len2) {
            ch2 = *(arr2 + len2 - n - 1);
        }

        char& ch = *(result + len - n - 1);
        if (ch1 != ch2) {
            if (carry) {
                ch = '0';
            } else {
                ch = '1';
            }
        } else if (ch1 == '0') {
            if (carry) {
                ch    = '1';
                carry = false;
            } else {
                ch = '0';
            }
        } else {
            if (carry) {
                ch = '1';
            } else {
                ch    = '0';
                carry = true;
            }
        }
    }
    if (*result == '0')
        std::cout << result + 1 << std::endl;
    else
        std::cout << result << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
//
// say we have a mapping between letters (a,b,c,...,z) and numbers as below,
// a --> 1
// b --> 2
// ...
// z --> 26
// given a number string, find the number of ways to decode it into letters
//
int decode_without_cache(const std::string& code)
{
    const int max_len = code.size();
    using lambda_t    = std::function<int(int)>;
    lambda_t impl     = [&](int idx_start) {
        if (idx_start >= max_len) {
            return 1;
        }

        int count1 = impl(idx_start + 1);
        int count2 = 0;
        if (max_len > idx_start + 1 &&
            (code[idx_start] == '1' || (code[idx_start] == '2' && code[idx_start + 1] <= '6'))) {
            count2 = impl(idx_start + 2);
        }
        return count1 + count2;
    };

    return impl(0);
}

int decode_with_cache(const std::string& code)
{
    const int        max_len = code.size();
    std::vector<int> cache(max_len, 0);
    using lambda_t = std::function<int(int)>;
    lambda_t impl  = [&](int idx_start) {
        if (idx_start >= max_len) {
            return 1;
        }
        if (cache[idx_start] > 0) {
            return cache[idx_start];
        }

        int count1 = impl(idx_start + 1);
        int count2 = 0;
        if (max_len > idx_start + 1 &&
            (code[idx_start] == '1' || (code[idx_start] == '2' && code[idx_start + 1] <= '6'))) {
            count2 = impl(idx_start + 2);
        }
        cache[idx_start] = count1 + count2;
        return cache[idx_start];
    };

    return impl(0);
}

void run_decode_num_to_letters()
{
    using code_pair_t = std::pair<std::string, int>;
    for (auto [code, result] : {
             code_pair_t{"3", 1},
             code_pair_t{"12", 2},
             code_pair_t{"1", 1},
             code_pair_t{"11", 2},
             code_pair_t{"111", 3},
             code_pair_t{"1111", 5},
             code_pair_t{"11111", 8},
             code_pair_t{"12345", 3},
             code_pair_t{"27345", 1},
         }) {
        std::cout << "decode_num_to_letters: " << code << " -> " << decode_without_cache(code) << std::endl;
        if (auto v = decode_with_cache(code); v != result) {
            std::cout << "\terror: expected " << result << " but got " << v << std::endl;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// given the steps you can go each time, find the number of ways to get up the top of stairs
//
// number of stairs you can go once a time: [1, 2]
//
// basic version, do recursively
int num_ways_stairs(int stairs)
{
    if (stairs <= 0)
        return 0;
    else if (stairs == 1)
        return 1;
    else if (stairs == 2)
        return num_ways_stairs(1) + 1;

    return num_ways_stairs(stairs - 1) + num_ways_stairs(stairs - 2);
}

int num_ways_stairs(int stairs, int* cache)
{
    // TODO: code will be easier if we specify 1 when stairs == 0
    if (stairs <= 0)
        return 0;
    else if (stairs == 1)
        return 1;
    else if (stairs == 2)
        return 2;

    int idx_cache = stairs - 1;
    if (cache[idx_cache] > 0)
        return cache[idx_cache];

    cache[idx_cache] = num_ways_stairs(stairs - 1, cache) + num_ways_stairs(stairs - 2, cache);
    return cache[idx_cache];
}

// optimize way without recursive, by using least cache
int num_ways_stairs_2(int stairs)
{
    int num_ways[] = {1, 0};
    for (int st = 1; st <= stairs; ++st) {
        int ways    = num_ways[0] + num_ways[1];
        num_ways[0] = num_ways[1] > 0 ? num_ways[1] : 1;
        num_ways[1] = ways;
    }
    return num_ways[1];
}

//
// generic solution that takes any steps as input
//
// Compare to coin_change, num_ways_stairs is doing permutation P instead of C!
//
// Is that possible coin_change use one dimentional array rather than two dimentions??
//
int generic_num_ways_stairs(int* steps_per_time, int len, int stairs)
{
    int* num_ways = new int[stairs]{0};
    for (int i = 1; i <= stairs; ++i) {
        int ways = 0;
        for (int j = 0; j < len; ++j) {
            int steps = *(steps_per_time + j);
            if (i < steps)
                continue;
            else if (i == steps) {
                ways += 1;
            } else {
                ways += num_ways[i - steps - 1];
            }
        }

        num_ways[i - 1] = ways;
        // std::cout << "\nstairs=" << i << ", ways=" << ways << std::endl;
    }

    int solutions = num_ways[stairs - 1];
    delete[] num_ways;
    return solutions;
}

int generic_num_ways_stairs(const std::vector<int>& steps_per_time, int stairs)
{
    const int        steps_types = steps_per_time.size();
    std::vector<int> num_ways(stairs + 1, 0);
    for (int i = 1; i <= stairs; ++i) {
        int ways = 0;
        for (int j = 0; j < steps_types; ++j) {
            const int steps = steps_per_time[j];
            if (i < steps)
                continue;
            else if (i == steps) {
                ways += 1;
            } else {
                ways += num_ways[i - steps];
            }
        }
        num_ways[i] = ways;
    }
    return num_ways.back();
}

// https://www.youtube.com/watch?v=Mjy4hd2xgrs&ab_channel=NeetCode
// https://github.com/doocs/leetcode/tree/main/solution/0500-0599/0518.Coin%20Change%20II/
//
// Given a set of coins with different face value, find the number of combinations that sum of coins equal to amount
//
// TODO: revisit this question!!
//
int coin_change(const std::vector<int>& coins, int amount)
{
    const int                     num_coins = coins.size();
    std::vector<std::vector<int>> matrix(num_coins, std::vector<int>(amount + 1, 0));
    for (int r = 0; r < num_coins; ++r) {
        for (int c = 1; c < amount + 1; ++c) {
            auto& combinations = matrix[r][c];
            if (r == 0) {
                combinations = (c % coins[r] == 0);
                continue;
            }

            // num of combinations without using coin 'r'
            combinations = matrix[r - 1][c];
            if (c > coins[r]) {
                // num of combinations by using coin 'r'
                combinations += matrix[r][c - coins[r]];
            } else {
                // ??
                combinations += (c % coins[r] == 0);
            }
        }
        /*
        std::cout << "r=" << r << ": ";
        print_array(matrix[r - 1], std::numeric_limits<int>::max());
        print_array(matrix[r], std::numeric_limits<int>::max());
        std::cout << std::endl;
        */
    }
    return matrix.back().back();
}

void run_num_way_stairs()
{
    for (int stairs : {1, 2, 3, 4, 5, 6, 7}) {
        {
            int steps_per_time[] = {1, 2};
            std::cout << "steps_per_time={1, 2}:" << std::endl;
            std::cout << "\tstairs=" << stairs
                      << ", generic_num_ways_stairs=" << generic_num_ways_stairs(steps_per_time, 2, stairs)
                      << std::endl;

            std::cout << "\tstairs=" << stairs
                      << ", generic_num_ways_stairs=" << generic_num_ways_stairs(std::vector<int>{1, 2}, stairs)
                      << std::endl;

            std::cout << "\tcoin_amount=" << stairs << ", coin_change=" << coin_change(std::vector<int>{1, 2}, stairs)
                      << std::endl;
        }

        {
            int steps_per_time[] = {1, 3, 5};
            std::cout << "steps_per_time={1, 3, 5}:" << std::endl;
            std::cout << "\tstairs=" << stairs
                      << ", generic_num_ways_stairs=" << generic_num_ways_stairs(steps_per_time, 3, stairs)
                      << std::endl;

            std::cout << "\tstairs=" << stairs
                      << ", generic_num_ways_stairs=" << generic_num_ways_stairs(std::vector<int>{1, 3, 5}, stairs)
                      << std::endl;

            std::cout << "\tcoin_amount=" << stairs
                      << ", coin_change=" << coin_change(std::vector<int>{1, 3, 5}, stairs) << std::endl;
        }

        {
            int steps_per_time[] = {5, 3};
            std::cout << "steps_per_time={5, 3}:" << std::endl;
            std::cout << "\tstairs=" << stairs
                      << ", generic_num_ways_stairs=" << generic_num_ways_stairs(steps_per_time, 2, stairs)
                      << std::endl;

            std::cout << "\tstairs=" << stairs
                      << ", generic_num_ways_stairs=" << generic_num_ways_stairs(std::vector<int>{5, 3}, stairs)
                      << std::endl;

            std::cout << "\tcoin_amount=" << stairs << ", coin_change=" << coin_change(std::vector<int>{5, 3}, stairs)
                      << std::endl;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////

//
// given an non-duplicate elements interger array, print out all possible
// subsets, order within the subset doesn't matter
//
// the number of possible outcomes is 2^n
//
void print_all_subsets_helper(int value, std::vector<std::vector<int>>& results)
{
    if (results.empty()) {
        results.push_back({0});
        results.push_back({value});
    } else {
        auto len = results.size();
        for (size_t i = 0; i < len; ++i) {
            auto result = results.at(i);
            result.push_back(value);
            results.push_back(std::move(result));
        }
    }
}
void print_all_subsets(int* arr, int len)
{
    std::vector<std::vector<int>> results;
    for (int i = 0; i < len; ++i) {
        print_all_subsets_helper(*(arr + i), results);
    }

    int ignore = 0;
    std::cout << "print_all_subsets: ";
    print_array(arr, len, &ignore);
    std::cout << ", count=" << results.size() << std::endl;
    for (auto& result : results) {
        std::cout << "\t";
        print_array(result, ignore);
        std::cout << std::endl;
    }
}

///////////////////////////////////////////////////////////////////////////////

//
// given a non-negative integer array from which each element tells the max steps
// you can move forward at that position, make a function the returns if you can
// jump out of the array (the end of the array)
//
// i.e. {4, 2, 0, 0, 2, 0}, at position/index 0, you can jump at most 4 steps
// until index 4, from which you can jump up to 2 steps and out of array
//

//
// this solution traverse array backwards, with cache (dynamic programming) keep
// if it's able to jump out of array at the place/index; then for previous place
// just check if it's able to reach any place where cache flag is true
//
bool tower_hopper(int* arr, int len)
{
    int   index = len;
    bool* cache = new bool[len]{false};
    while (index-- > 0) {
        int steps = *(arr + index);
        while (steps-- > 0) {
            if (steps >= len - index - 1) {
                *(cache + index) = true;
                break;
            }
            bool possible = *(cache + index + steps + 1);
            if (possible) {
                *(cache + index) = possible;
                break;
            }
        }
    }
    std::cout << "\tmiddle result: ";
    print_array(cache, len);
    std::cout << "\n\t";
    return *cache;
}

//
// this is optimal solution, starting from beginning place, find the next optimal
// place: the biggest/farthest place given sum of current value and value at next
// potential place, traverse array until you jump out of array or finish traverse
//
bool tower_hopper_v2(int* arr, int len)
{
    auto next_optimal_pos = [=](int index) {
        int pos      = -1;
        int farthest = 0;
        int steps    = *(arr + index);
        while (steps-- > 0) {
            int next_pos = index + steps + 1;
            if (next_pos < len) {
                int next_pos_steps = *(arr + next_pos);
                if (next_pos_steps + steps + 1 > farthest) {
                    farthest = next_pos_steps + steps + 1;
                    pos      = next_pos;
                }
            } else {
                pos = next_pos;
                break;
            }
        }
        return pos;
    };

    int pos = 0;
    while (true) {
        int next_pos = next_optimal_pos(pos);
        std::cout << "\tpos=" << pos << ", next_pos=" << next_pos << std::endl;
        if (next_pos < 0 || next_pos == pos)
            return false;
        if (next_pos >= len)
            return true;
        pos = next_pos;
    }
    return false;
}

//
// https://www.enjoyalgorithms.com/blog/median-of-two-sorted-arrays
// given two sorted arrays A and B, find the median of combined array
//
// Input:  A[] = [1, 3], B[] = [0]
// Output: 1
//
// Input:  A[] = [1, 3], B[] = [2]
// Output: 2
//
// Input:  A[] = [2, 4], B[] = [1, 3, 5]
// Output: 3
//
// Input:  A[] = [1, 3, 6], B[] = [2, 8, 12]
// Output: 4.5
//
// Input:  A[] = [1, 3, 4, 6, 9], B[] = [2, 5, 7, 8, 10]
// Output: 5.5
//
double find_median_two_sorted_arrays(int* A, int m, int* B, int n)
{
    auto median = [](int* arr, int len) {
        if (len % 2 == 0) {
            auto x = (double)(*(arr + len / 2 - 1) + *(arr + len / 2)) / 2;
            std::cout << "x=" << x << ", " << *(arr + len / 2 - 1) << ", " << *(arr + len / 2) << std::endl;
            return x;
        } else
            return (double)*(arr + len / 2);
    };
    if (m <= 0)
        return median(B, n);
    if (n <= 0)
        return median(A, m);
    if (m == 1 && n == 1)
        return (double)(*A + *B) / 2;

    // TODO: miss some cornor cases like [1, 3], [0]...

    double mA = median(A, m);
    double mB = median(B, n);
    if (mA < mB)
        return find_median_two_sorted_arrays(A + m / 2, m - (m / 2), B, n - (n / 2 + (n % 2 == 0 ? 0 : 1)));
    else if (mA > mB)
        return find_median_two_sorted_arrays(A, m - (m / 2), B + n / 2, n - (n / 2));
    else
        return mA;
}

//
// given an array that contains n + 1 integers that range is [1, n]
// there is only one repeated number, find the number
//
// [3, 1, 3, 4, 3]
//
// 1) brute force:      O(n^2),         space O(1)
// 2) hashtable:        O(n),           space O(n)
// 3) marker:           O(n),           space O(1), changed input array
// 4) sorting:          O(n * log(n)),  space O(1), changed input array
// 5) bit by bit:       O(n * 32),      space O(1), not readable...
// 6) binary search:    O(n * log(n)),  space O(1)
//

// binary search solution
int find_duplicate(int* arr, int len)
{
    // find the smallest number m that satisfies count(num <= m) > m
    int left  = 1;
    int right = len;
    while (left < right) {
        const int m     = left + (right - left) / 2;
        int       count = 0;
        for (int i = 0; i < len; ++i) {
            if (*(arr + i) <= m)
                ++count;
        }

        if (count > m)
            right = m;
        else
            left = m + 1;
    }
    return left;
}

///////////////////////////////////////////////////////////////////////////////
//
// https://www.youtube.com/watch?v=dtLIe1rHYPg&list=RDCMUC5xDNEcvb1vgw3lE21Ack2Q&index=27&ab_channel=HuaHua
//
// given root node of a complete binary tree, find total number of nodes
// time complexity should be less than O(n)
//
int tree_count_complete_tree_nodes(tree_node_t<int>* root)
{
    if (!root)
        return 0;

    using lambda_t     = std::function<int(tree_node_t<int>*)>;
    lambda_t get_depth = [&](tree_node_t<int>* node) {
        if (!node)
            return 0;
        return 1 + get_depth(node->left);
    };

    const auto left_depth  = get_depth(root->left);
    const auto right_depth = get_depth(root->right);

    if (left_depth == right_depth) {
        // left branch is perfectly complete tree, but right branch may not be given the way of depth determined
        return 1 + ((1 << left_depth) - 1) + tree_count_complete_tree_nodes(root->right);
    } else {
        // right branch is perfectly complete, but left branch may not be
        return 1 + tree_count_complete_tree_nodes(root->left) + ((1 << right_depth) - 1);
    }
}

// a variant question: given root, find total number of leaf nodes
int tree_count_complete_tree_leaves(tree_node_t<int>* root)
{
    if (!root) {
        return 0;
    } else if (!root->left && !root->right) {
        return 1;
    } else {
        return tree_count_complete_tree_leaves(root->left) + tree_count_complete_tree_leaves(root->right);
    }
}

// get any type of tree max depth
// TODO: need test
int get_any_tree_depth(tree_node_t<int>* node)
{
    if (!node)
        return 0;

    int left_depth  = 1 + get_any_tree_depth(node->left);
    int right_depth = 1 + get_any_tree_depth(node->right);
    return std::max(left_depth, right_depth);
}

// check if an arbitrary tree is balanced
// TODO: need test
bool check_if_balanced_tree(tree_node_t<int>* node, int& depth)
{
    if (!node)
        return true;

    int  left_depth     = 0;
    int  right_depth    = 0;
    bool left_balanced  = check_if_balanced_tree(node->left, left_depth);
    bool right_balanced = check_if_balanced_tree(node->right, right_depth);

    // TODO: not update left_depth or right_depth...

    // always record the max depth only for log/test purpose
    depth = std::max(left_depth, right_depth) + 1;

    // assume left depth must not be less than right depth
    return left_balanced && right_balanced && (left_depth >= right_depth && left_depth - right_depth <= 1);
}

///////////////////////////////////////////////////////////////////////////////
//
// https://leetcode.com/problems/best-sightseeing-pair/
// given integer array, find the max score defined as (arr[i] + arr[j] + i - j) where i < j!
//

// solution 1), dynamic programming
//
int array_max_pair_score(int* arr, int len)
{
    using lambda_t = std::function<std::pair<int, int>(int*, int)>;
    lambda_t impl  = [&](int* arr, int idx) {
        if (idx == 0)
            return std::make_pair(std::numeric_limits<int>::min(), *arr);

        auto [max_pair_score, max_elem_n_idx] = impl(arr, idx - 1);

        // first:  max score up to idx, so the max score at position (idx+1) is either -
        //          a) max score at position idx; or
        //          b) max_elem_n_idx + (arr[idx+1] - (idx + 1))
        // second: max (arr[i] + i)  up to position idx
        return std::make_pair(
            std::max(max_pair_score, max_elem_n_idx + (*(arr + idx) - idx)),
            std::max(max_elem_n_idx, *(arr + idx) + idx));
    };

    return impl(arr, len - 1).first;
}

//
// solution 2), no recursion, track the max score so far and its right element's
// index, and find new possible max score in terms of -
// a) new element is bigger than max score's right element - more than index distance
// b) new element and the prior element form the max score
//
int array_max_pair_score_v2(int* arr, int len)
{
    int idx_pair_left  = 0;
    int idx_pair_right = 1;
    int max_pair_score = (*(arr + idx_pair_left)) + (*(arr + idx_pair_right)) + (idx_pair_left - idx_pair_right);
    for (int i = 2; i < len; ++i) {
        int value            = *(arr + i);
        int value_pair_right = *(arr + idx_pair_right);
        if (value - i > value_pair_right - idx_pair_right) {
            idx_pair_right = i;
            max_pair_score = (*(arr + idx_pair_left)) + value + (idx_pair_left - idx_pair_right);
        }
        int value_prior = *(arr + i - 1);
        if (value + value_prior - 1 > max_pair_score) {
            max_pair_score = value + value_prior - 1;
            idx_pair_left  = i - 1;
            idx_pair_right = i;
        }
    }
    std::cout << " idx_pair_left=" << idx_pair_left << ",idx_pair_right=" << idx_pair_right << " ";
    return max_pair_score;
}

// solution 3), O(n), go through array, find biggest value (arr[i] + i); find biggest value (arr[i] - i)
// then sum;
// NOTICE: if two different elements are required, we need to track two biggest numbers along with index
// on each side, finally sum the two of them with different index
int array_max_pair_score_v3([[maybe_unused]] int* arr, [[maybe_unused]] int len)
{
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// https://leetcode.com/problems/best-time-to-buy-and-sell-stock/
//
// given integer array of stock prices, find the best time to buy one share and sell in the future to gain max profit
//
// this is simpler version of above max pair score question, besides of below
// solution, you can also implement it by dynamic programming
//
int max_profit_1_transaction(const std::vector<int>& prices)
{
    int max_profit = std::max(0, prices[1] - prices[0]);
    int min_price  = std::min(prices[0], prices[1]);
    for (size_t i = 2; i < prices.size(); ++i) {
        int price  = prices[i];
        max_profit = std::max(max_profit, price - min_price);
        min_price  = std::min(min_price, price);
    }
    return max_profit;
}

int max_profit_1_transaction_v3(const std::vector<int>& prices)
{
    int port_buy  = -1 * prices[0];
    int port_sell = 0;
    for (size_t i = 1; i < prices.size(); ++i) {
        port_buy  = std::max(port_buy, -1 * prices[i]);
        port_sell = std::max(port_sell, prices[i] + port_buy);
    }
    return port_sell;
}

int max_profit_1_transaction_v2(const std::vector<int>& prices)
{
    using lambda_t = std::function<std::pair<int, int>(size_t)>;
    lambda_t impl  = [&](size_t len) {
        if (len == 1) {
            return std::make_pair(0, prices[0]);
        } else if (len == 2 && prices[0] >= prices[1]) {
            return std::make_pair(0, prices[1]);
        }

        auto [max_profit, min_price] = impl(len - 1);
        return std::make_pair(std::max(max_profit, prices[len] - min_price), std::min(min_price, prices[len]));
    };

    return impl(prices.size() - 1).first;
}

void run_max_profit_1_transaction()
{
    for (auto prices : {
             std::vector<int>{8, 1, 5, 2, 6},         // 5
             std::vector<int>{-5, -3, 2, 2, 9, 1},    // 14
             std::vector<int>{7, 1, 5, 3, 6, 4},      // 5
             std::vector<int>{1, 2, 3, 4, 5},         // 4
         }) {
        std::cout << "\nprices: ";
        print_array(prices);
        std::cout << std::endl;
        std::cout << "max_profit_1_transaction: " << max_profit_1_transaction(prices) << std::endl;
        std::cout << "max_profit_1_transaction_v2: " << max_profit_1_transaction_v2(prices) << std::endl;
        std::cout << "max_profit_1_transaction_v3: " << max_profit_1_transaction_v3(prices) << std::endl;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// https://leetcode.com/problems/maximum-product-subarray/
//
// given integer (may have negative) array, find the max product value of subarray
//
// besides of below solution, you may use dynamic programming as described here -
// https://www.youtube.com/watch?v=gwZm6mIYDfk&list=RDCMUC5xDNEcvb1vgw3lE21Ack2Q&index=31&ab_channel=HuaHua
//
int array_max_product_subarray(int* arr, int len)
{
    int max_product_incl = (*arr) * (*(arr + 1));
    int min_product_incl = (*arr) * (*(arr + 1));
    int max_product_excl = *arr;
    int min_product_excl = *arr;
    for (int i = 2; i < len; ++i) {
        int prior_max_incl = max_product_incl;
        int prior_min_incl = min_product_incl;

        int value        = *(arr + i);
        int prior_value  = *(arr + i - 1);
        max_product_incl = std::max({max_product_incl * value, min_product_incl * value, prior_value * value, value});
        min_product_incl = std::min({max_product_incl * value, min_product_incl * value, prior_value * value, value});
        max_product_excl = std::max({max_product_excl, prior_max_incl});
        min_product_excl = std::min({min_product_excl, prior_min_incl});
    }
    return std::max({max_product_incl, max_product_excl});
}

///////////////////////////////////////////////////////////////////////////////
//
// https://leetcode.com/problems/maximum-sum-circular-subarray/
//
// this video explains well -
// https://www.youtube.com/watch?v=Gk6yWhfzdOc&list=RDCMUC5xDNEcvb1vgw3lE21Ack2Q&index=31&ab_channel=HuaHua
// there're two cases -
// 1) max subarray is in the middle of array
//    just use regular way from subarray_max_sum.h
// 2) max subarray includes the tail and head of raw array
//    this is equivalent to find the min subarray from raw array, or find the
//    max subarray from the inverse sign of raw array,
//    then max subarray = sum of raw array - min subarray
//
// final answer will be max of 1) and 2)
//
int array_max_circular_subarray([[maybe_unused]] int* arr, [[maybe_unused]] int len)
{
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
struct Bid {
    ~Bid() { std::cout << "~Bid()" << std::endl; }
    static inline Bid& self()
    {
        static Bid o;
        return o;
    }
    static inline bool        isBuy() { return true; }
    static inline const char* sideString() { return "Bid"; }

private:
    Bid() { std::cout << "Bid()" << std::endl; }
};
struct Ask {
    ~Ask() { std::cout << "~Ask()" << std::endl; }
    static inline Ask& self()
    {
        static Ask o;
        return o;
    }
    static inline bool        isBuy() { return false; }
    static inline const char* sideString() { return "Ask"; }

private:
    Ask() { std::cout << "Ask()" << std::endl; }
};

struct Pricing {
    template <typename Side>
    void getRaw(Side, int n)
    {
        std::cout << "Pricing::getRaw: " << Side::sideString() << ", n=" << n << std::endl;
    }

    template <typename Side>
    void getTop(int n)
    {
        std::cout << "Pricing::getTop: " << Side::sideString() << ", n=" << n << std::endl;
    }
};
struct Pricing2 {
    template <typename Side>
    void getRaw(Side, int n)
    {
        std::cout << "Pricing2::getRaw: " << Side::sideString() << ", n=" << n << std::endl;
    }

    template <typename Side>
    void getTop(int n)
    {
        std::cout << "Pricing2::getTop: " << Side::sideString() << ", n=" << n << std::endl;
    }
};

void run_lambda_template()
{
    {
        auto lambda = []<typename Side>(Side, int n) {
            std::cout << n << ": " << Side::sideString() << ", " << Side::isBuy() << std::endl;
        };

        lambda(Bid::self(), 5);
        lambda(Ask::self(), 7);

        // std::bind with template lambda works great!!!
        auto func = std::bind(lambda, std::placeholders::_1, std::placeholders::_2);
        func(Bid::self(), 10);
        func(Ask::self(), 13);
    }

    {
        Pricing p;
        p.getRaw(Bid::self(), 2);

        // std::bind with class member template function doesn't work well :(((
        // auto ff = std::bind(&Pricing::getRaw, &p, std::placeholders::_1, std::placeholders::_2);
        // auto ff = std::bind(&Pricing::getRaw, &p, Bid{}, std::placeholders::_1);
        // ff(2);
        // ff(3);

        // ugly, has to explicitly tell the type
        // TODO: why this bind doesn't work in macbook????
        // auto gg = std::bind(&Pricing::template getRaw<Bid>, &p, Bid::self(), std::placeholders::_1);
        // gg(2);
        // gg(3);

        //
        // so what we can do is bind with lambda template, pass non-inherited object along with side type
        auto lambda = []<typename P, typename Side>(Side&, P&& pricing, int n) {
            // std::cout << n << ": " << Side::sideString() << ", " << Side::isBuy() << std::endl;
            pricing.template getTop<Side>(n + 1);
        };

        // std::bind with template lambda works great!!!
        auto func = std::bind(lambda, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

        func(Bid::self(), p, 10);
        func(Ask::self(), p, 13);

        Pricing2 p2;
        func(Bid::self(), p2, 10);
        func(Ask::self(), p2, 13);
    }
}

#endif
