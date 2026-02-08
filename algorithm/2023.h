#pragma once

#include "../core/node.h"
#include "../core/util.h"

///////////////////////////////////////////////////////////////////////////////
//
// given two binary arrays, calculate the sum in binary format
//
std::string sum_binary(const std::string& arr1, const std::string& arr2)
{
    const auto  len1 = arr1.size();
    const auto  len2 = arr2.size();
    uint32_t    len  = (len1 > len2 ? len1 : len2) + 1;
    std::string result(len, '\0');
    bool        carry = false;
    for (uint32_t n = 0; n < len; ++n) {
        char ch1 = '0';
        char ch2 = '0';
        if (n < len1) {
            ch1 = arr1[len1 - n - 1];
        }
        if (n < len2) {
            ch2 = arr2[len2 - n - 1];
        }

        char& ch = result[len - n - 1];
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
    return result.front() == '0' ? result.substr(1) : result;
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
int decode_no_recursive(const std::string& code)
{
    const auto       len = (int)code.size();
    std::vector<int> cache(len, 0);
    cache[0] = 1;
    for (int i = 1; i < len; ++i) {
        const auto pre_c = code[i - 1];
        const auto c     = code[i];
        if (pre_c == '1' || (pre_c == '2' && c <= '6')) {
            cache[i] = cache[i - 1] + (i >= 2 ? cache[i - 2] : 1);
        } else {
            cache[i] = cache[i - 1];
        }
    }
    return cache[len - 1];
}

void run_decode_num_to_letters()
{
    using use_case_t = std::pair<std::string, int>;
    for (auto [code, exp_v] : {
             use_case_t{"3", 1},
             use_case_t{"12", 2},
             use_case_t{"1", 1},
             use_case_t{"11", 2},
             use_case_t{"111", 3},
             use_case_t{"1111", 5},
             use_case_t{"11111", 8},
             use_case_t{"12345", 3},
             use_case_t{"27345", 1},
         }) {
        const auto v1 = decode_without_cache(code);
        const auto v2 = decode_with_cache(code);
        const auto v3 = decode_no_recursive(code);

        std::cout << __FUNCTION__ << ": code=" << code << ", decode_without_cache=" << v1 << ", "
                  << (exp_v == v1 ? "SUCCESS" : "FAILED") << "; decode_with_cache=" << v2 << ", "
                  << (exp_v == v2 ? "SUCCESS" : "FAILED") << "; decode_no_recursive=" << v3 << ", "
                  << (exp_v == v3 ? "SUCCESS" : "FAILED") << std::endl;
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
        }
    }
}

// Given a set of coins with different face value, find the number of combinations that sum of coins equal to amount
// leetcode 518
int coin_change(const std::vector<int>& coins, int amount)
{
    const auto                    count_coins = (int)coins.size();
    std::vector<std::vector<int>> matrix(count_coins + 1, std::vector<int>(amount + 1, 0));

    // when amount == 0, there is always 1 combination available (take 0 coin)
    matrix[0][0] = 1;
    for (int i = 1; i < count_coins + 1; ++i) {
        const auto coin_amount = coins[i - 1];
        // NOTICE: start with j == 0, so that matrix[i][0] == 1
        for (int j = 0; j < amount + 1; ++j) {
            matrix[i][j] = matrix[i - 1][j];    // dont use new coin
            if (j >= coin_amount) {
                matrix[i][j] += matrix[i][j - coin_amount];    // use new coin
            }
        }
    }
    return matrix[count_coins][amount];
}
void run_coin_change()
{
    using use_case_t = std::tuple<std::vector<int>, int, int>;
    for (auto [coins, amount, exp_v] : {
             use_case_t{{1, 2}, 1, 1},
             use_case_t{{1, 2}, 2, 2},
             use_case_t{{1, 2}, 3, 2},
             use_case_t{{1, 2}, 4, 3},
             use_case_t{{1, 2, 5}, 5, 4},
             use_case_t{{2, 5}, 3, 0},
         }) {
        const auto v = coin_change(coins, amount);
        std::cout << __FUNCTION__ << ": coins=" << util::dump_array(coins) << ", amount=" << amount
                  << ", coin_change=" << v << ", " << (exp_v == v ? "SUCCESS" : "FAILED") << std::endl;
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
    std::cout << "print_all_subsets: " << util::dump_array(arr, len, &ignore) << ", count=" << results.size()
              << std::endl;
    for (auto& result : results) {
        std::cout << "\t" << util::dump_array(result, ignore) << std::endl;
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
bool tower_hopper(const std::vector<int>& arr)
{
    const auto len   = (int)arr.size();
    int        index = len;
    bool*      cache = new bool[len]{false};
    while (index-- > 0) {
        int steps = arr[index];
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
    std::cout << "\tmiddle result: " << util::dump_array(cache, len) << "\n\t";
    return *cache;
}

//
// this is optimal solution, starting from beginning place, find the next optimal
// place: the biggest/farthest place given sum of current value and value at next
// potential place, traverse array until you jump out of array or finish traverse
//
bool tower_hopper_v2(const std::vector<int>& arr)
{
    const auto len              = (int)arr.size();
    auto       next_optimal_pos = [=](int index) {
        int pos      = -1;
        int farthest = 0;
        int steps    = arr[index];
        while (steps-- > 0) {
            int next_pos = index + steps + 1;
            if (next_pos < len) {
                int next_pos_steps = arr[next_pos];
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
bool tower_hopper_v3(const std::vector<int>& arr)
{
    [[maybe_unused]] auto jumps           = 1;         // the number of min jumps
    auto                  max_pos         = arr[0];    // the max reachable position given the jumps so far
    auto                  max_pos_if_jump = arr[0];    // the max reachable position if jumping one more time so far
    for (int i = 1; i < (int)arr.size(); ++i) {
        max_pos_if_jump = std::max(max_pos_if_jump, i + arr[i]);    // always update max_pos_if_jump if possible
        if (i == max_pos) {    // given the jumps so far, this is max reachable pos, we have to jump one more time
            max_pos = max_pos_if_jump;
            ++jumps;

            if (max_pos >= (int)arr.size()) {
                return true;
            }
        }
    }
    return false;
}

void run_tower_hopper()
{
    using use_case_t = std::pair<std::vector<int>, bool>;
    for (auto [arr, exp_v] : {
             use_case_t({2, 1, 0, 1}, false),
             use_case_t({1, 2, 0, 0}, false),
             use_case_t({4, 2, 0, 0, 2, 0}, true),
             use_case_t({4, 2, 0, 0, 1, 0}, false),
         }) {
        const auto v1 = tower_hopper(arr);
        const auto v2 = tower_hopper_v2(arr);
        const auto v3 = tower_hopper_v3(arr);
        std::cout << "array=" << util::dump_array(arr, std::numeric_limits<int>::min()) << std::boolalpha
                  << ", tower_hopper=" << v1 << ", " << (exp_v == v1 ? "SUCCESS" : "FAILED")
                  << "; tower_hopper_v2=" << v2 << ", " << (exp_v == v2 ? "SUCCESS" : "FAILED")
                  << "; tower_hopper_v3=" << v3 << ", " << (exp_v == v3 ? "SUCCESS" : "FAILED") << std::noboolalpha
                  << std::endl;
    }
}

//
// https://www.geeksforgeeks.org/dsa/median-of-two-sorted-arrays-of-different-sizes/
//
// 0) do binary serach in the smaller array;
// 1) from smaller array, find middle point midA;
// 2) in the other array, find posB=(nA + nB + 1) / 2 - midA, this ensures (midA + posB) is the median point!!
// 3) check the elements around points midA and posB, they are the target if leftMidA <= rightPosB && rightMidA >=
// leftPosB; 4) otherwise, if leftMidA > rightPosB, should search the left side of midA, so high = midA - 1; 5) if
// rightMidA < leftPosB, should search the right side of midA, so low = midA + 1;
int find_median_two_sorted_arrays(const std::vector<int>& A, const std::vector<int>& B)
{
    const auto nA = (int)A.size();
    const auto nB = (int)B.size();
    if (nA > nB) {
        return find_median_two_sorted_arrays(B, A);
    }

    int low  = 0;
    int high = nA;
    while (low <= high) {
        int midA = (low + high) / 2;
        int posB = (nA + nB + 1) / 2 - midA;

        int leftMidA  = (midA > 0 ? A[midA - 1] : std::numeric_limits<int>::min());
        int rightMidA = (midA < nA ? A[midA] : std::numeric_limits<int>::max());

        int leftPosB  = (posB > 0 ? B[posB - 1] : std::numeric_limits<int>::min());
        int rightPosB = (posB < nB ? B[posB] : std::numeric_limits<int>::max());

        if (leftMidA <= rightPosB && rightMidA >= leftPosB) {
            if ((nA + nB) % 2 == 0) {
                return (std::max(leftMidA, leftPosB) + std::min(rightMidA, rightPosB)) / 2;
            } else {
                return std::max(leftMidA, leftPosB);
            }
        }

        if (leftMidA > rightPosB) {
            high = midA - 1;
        } else {
            low = midA + 1;
        }
    }
    // How do we get here????
    return -1;
}

void run_find_median_two_sorted_arrays()
{
    using use_case_t = std::tuple<std::vector<int>, std::vector<int>, int>;
    for (auto [A, B, exp_v] : {
             use_case_t{{1, 3}, {}, 2},
             use_case_t{{1, 3}, {2}, 2},
             use_case_t{{2, 4}, {1, 3, 5}, 3},
             use_case_t{{1, 3, 6}, {2, 8, 12}, 4},
             use_case_t{{1, 3, 4, 6, 9}, {2, 5, 7, 8, 10}, 5},
             use_case_t{{1, 3, 4, 6, 9, 11}, {2, 5, 7, 8, 10}, 6},
             use_case_t{{-5, 3, 6, 12, 15}, {-12, -10, -6, -3, 4, 10}, 3},
         }) {
        const auto v = find_median_two_sorted_arrays(A, B);
        std::cout << __FUNCTION__ << ": A=" << util::dump_array(A) << ", B=" << util::dump_array(B)
                  << ", find_median_two_sorted_arrays=" << v << ", " << (exp_v == v ? "SUCCESS" : "FAILED")
                  << std::endl;
    }
}

//
// given an array that contains n + 1 integers that range is [1, n]
// there is only one repeated number, find the number
//
// [3, 1, 2, 4, 3]
//
// 1) brute force:      O(n^2),         space O(1)
// 2) hashtable:        O(n),           space O(n)
// 3) marker:           O(n),           space O(1), changed input array
// 4) sorting:          O(n * log(n)),  space O(1), changed input array
// 5) bit by bit:       O(n * 32),      space O(1), not readable...
// 6) binary search:    O(n * log(n)),  space O(1)
//

// binary search solution
int find_duplicate(const std::vector<int>& arr)
{
    // find the smallest number m that satisfies count(num <= m) > m
    int left  = 1;
    int right = (int)arr.size();
    while (left < right) {
        const int m     = left + (right - left) / 2;
        int       count = 0;
        for (int i = 0; i < (int)arr.size(); ++i) {
            if (arr[i] <= m)
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
int tree_count_complete_tree_nodes(tree_node_int_t* root)
{
    if (!root) {
        return 0;
    }

    std::function<int(tree_node_int_t*)> get_depth = [&](tree_node_int_t* node) {
        return node ? (1 + get_depth(node->left)) : 0;
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
int tree_count_complete_tree_leaves(tree_node_int_t* root)
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
int get_any_tree_depth(tree_node_int_t* node)
{
    if (!node) {
        return 0;
    }

    int left_depth  = 1 + get_any_tree_depth(node->left);
    int right_depth = 1 + get_any_tree_depth(node->right);
    return std::max(left_depth, right_depth);
}

// check if an arbitrary tree is balanced
bool check_if_balanced_tree(tree_node_int_t* node)
{
    std::function<int(tree_node_int_t*)> impl = [&](tree_node_int_t* n) {
        if (!n) {
            return 0;
        }
        const auto lh = impl(n->left);
        const auto rh = impl(n->right);
        if (lh < 0 || rh < 0 || std::abs(lh - rh) > 1) {
            return -1;
        }
        return 1 + std::max(lh, rh);
    };
    return impl(node) >= 0;
}
bool check_if_balanced_tree2(tree_node_int_t* root)
{
    std::function<int(tree_node_int_t*)> get_depth = [&](tree_node_int_t* node) {
        if (!node) {
            return 0;
        }
        const auto lh = 1 + get_depth(node->left);
        const auto rh = 1 + get_depth(node->right);
        if (lh < 0 || rh < 0 || std::abs(lh - rh) > 1) {
            return -1;
        }
        return std::max(lh, rh);
    };

    return get_depth(root) >= 0;
}
void run_check_if_balanced_tree()
{
    /*
                        100
                       /   \
                     1      2
                    / \    / \
                  3   4   5   6
                 / \ / \
                7  8 9  0
    */
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

    using use_case_t = std::pair<tree_node_int_t*, int>;
    for (auto [node, exp_v] : {
             use_case_t(&n0, 1),
             use_case_t(&n4, 3),
             use_case_t(&n1, 7),
             use_case_t(&root, 11),
         }) {
        const auto v = tree_count_complete_tree_nodes(node);
        std::cout << "node=" << node->data << ", tree_count_complete_tree_nodes=" << v << ", "
                  << (exp_v == v ? "SUCCESS" : "FAILED") << std::endl;
    }

    for (auto [node, exp_v] : {
             use_case_t(&n0, 1),
             use_case_t(&n4, 2),
             use_case_t(&n1, 4),
             use_case_t(&root, 6),
         }) {
        const auto v = tree_count_complete_tree_leaves(node);
        std::cout << "node=" << node->data << ", tree_count_complete_tree_leaves=" << v << ", "
                  << (exp_v == v ? "SUCCESS" : "FAILED") << std::endl;
    }

    for (auto [node, exp_v] : {
             use_case_t(&root, true),
             use_case_t(&n0, true),
             use_case_t(&n1, true),
             use_case_t(&n2, true),
             use_case_t(&n3, true),
             use_case_t(&n4, true),
             use_case_t(&n5, true),
             use_case_t(&n6, true),
             use_case_t(&n7, true),
             use_case_t(&n8, true),
             use_case_t(&n9, true),
         }) {
        const auto v1 = check_if_balanced_tree(node);
        const auto v2 = check_if_balanced_tree2(node);
        std::cout << "node=" << node->data << ", check_if_balanced_tree=" << v1 << ", "
                  << (exp_v == v1 ? "SUCCESS" : "FAILED") << "; check_if_balanced_tree2=" << v2 << ", "
                  << (exp_v == v2 ? "SUCCESS" : "FAILED") << std::endl;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// https://leetcode.com/problems/best-sightseeing-pair/
// given integer array, find the max score defined as (arr[i] + arr[j] + i - j) where i < j!
//

// solution 1), dynamic programming
//
int array_max_pair_score(const std::vector<int>& arr)
{
    std::function<std::pair<int, int>(int)> impl = [&](int idx) {
        if (idx == 0) {
            return std::make_pair(std::numeric_limits<int>::min(), arr[idx]);
        }

        auto [max_pair_score, max_elem_n_idx] = impl(idx - 1);

        // first:  max score up to idx, so the max score at position (idx+1) is either -
        //         a) max score at position idx; or,
        //         b) max_elem_n_idx + (arr[idx+1] - (idx + 1))
        // second: max (arr[i] + i)  up to position idx
        return std::make_pair(
            std::max(max_pair_score, max_elem_n_idx + (arr[idx] - idx)), std::max(max_elem_n_idx, arr[idx] + idx));
    };
    return impl((int)arr.size() - 1).first;
}

//
// solution 2), no recursion, track the max score so far and its right element's index, and find new possible max score
//              in terms of -
//              a) new element is bigger than max score's right element - more than index distance;
//              b) new element and the prior element form the max score;
//
int array_max_pair_score_v2(const std::vector<int>& arr)
{
    int idx_pair_left  = 0;
    int idx_pair_right = 1;
    int max_pair_score = arr[idx_pair_left] + arr[idx_pair_right] + (idx_pair_left - idx_pair_right);
    for (int i = 2; i < (int)arr.size(); ++i) {
        int value            = arr[i];
        int value_pair_right = arr[idx_pair_right];
        if (value - i > value_pair_right - idx_pair_right) {
            idx_pair_right = i;
            max_pair_score = arr[idx_pair_left] + value + (idx_pair_left - idx_pair_right);
        }
        int value_prior = arr[i - 1];
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
int array_max_pair_score_v3(const std::vector<int>& arr)
{
    int max_sum[2]     = {std::numeric_limits<int>::min()};
    int max_sum_idx[2] = {std::numeric_limits<int>::min()};
    int max_sub[2]     = {std::numeric_limits<int>::min()};
    int max_sub_idx[2] = {std::numeric_limits<int>::min()};

    for (int i = 0; i < (int)arr.size(); ++i) {
        const auto sum = arr[i] + i;
        if (sum > max_sum[0]) {
            max_sum[1]     = max_sum[0];
            max_sum_idx[1] = max_sum_idx[0];

            max_sum[0]     = sum;
            max_sum_idx[0] = i;
        } else if (sum > max_sum[1]) {
            max_sum[1]     = sum;
            max_sum_idx[1] = i;
        }

        const auto sub = arr[i] - i;
        if (sub > max_sub[0]) {
            max_sub[1]     = max_sub[0];
            max_sub_idx[1] = max_sub_idx[0];

            max_sub[0]     = sub;
            max_sub_idx[0] = i;
        } else if (sub > max_sub[1]) {
            max_sub[1]     = sub;
            max_sub_idx[1] = i;
        }
    }

    // TODO: it is tricky to find desired max_sub & max_sum that satisfy index sum_i < sub_j
    // ...

    return std::max(max_sum[0] + max_sub[1], max_sum[1] + max_sub[0]);
}

void run_array_max_pair_score()
{
    using use_case_t = std::pair<std::vector<int>, int>;
    for (auto [arr, exp_v] : {
             use_case_t({8, 1, 5, 2, 6}, 11),
             use_case_t({-5, -3, 2, 2, 9, 1}, 10),
             use_case_t({7, 1, 5, 3, 6, 4}, 10),
             use_case_t({1, 2, 3, 4, 5}, 8),
         }) {

        const auto v1 = array_max_pair_score(arr);
        const auto v2 = array_max_pair_score_v2(arr);
        // const auto v3 = array_max_pair_score_v3(arr);

        std::cout << "array=" << util::dump_array(arr) << ", array_max_pair_score=" << v1 << ", "
                  << (exp_v == v1 ? "SUCCESS" : "FAILED") << "; array_max_pair_score_v2=" << v2 << ", "
                  << (exp_v == v2 ? "SUCCESS" : "FAILED") << std::endl;
    }
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
    using use_case_t = std::pair<std::vector<int>, int>;
    for (auto [prices, exp_v] : {
             use_case_t{{8, 1, 5, 2, 6}, 5},
             use_case_t{{-5, -3, 2, 2, 9, 1}, 14},
             use_case_t{{7, 1, 5, 3, 6, 4}, 5},
             use_case_t{{1, 2, 3, 4, 5}, 4},
         }) {
        const auto v1 = max_profit_1_transaction(prices);
        const auto v2 = max_profit_1_transaction_v2(prices);
        const auto v3 = max_profit_1_transaction_v3(prices);
        std::cout << __FUNCTION__ << ": prices=" << util::dump_array(prices) << ", max_profit_1_transaction=" << v1
                  << ", " << (exp_v == v1 ? "SUCCESS" : "FAILED") << ", max_profit_1_transaction_v2=" << v2 << ", "
                  << (exp_v == v2 ? "SUCCESS" : "FAILED") << ", max_profit_1_transaction_v3=" << v3 << ", "
                  << (exp_v == v3 ? "SUCCESS" : "FAILED") << std::endl;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// https://leetcode.com/problems/best-time-to-buy-and-sell-stock-iii/description/
//
// You are given an array prices where prices[i] is the price of a given stock on the ith day.
// Find the maximum profit you can achieve. You may complete at most two transactions.
// Note: You may not engage in multiple transactions simultaneously (i.e., you must sell the stock before you buy
// again).
//
// Another less optimal solution: loop the array, split into 2 arrays, find max profit in each part and get the max sum
//
int max_profit_2_transactions(const std::vector<int>& prices)
{
    //
    // clang-format off
    //
    // I can explain following variables in this way -
    // @port_value_1st_buy:  port value if this is the 1st buy on this day, meaning '-1 * prices[0] (price on this day)'
    // @port_value_1st_sell: port value if this is the 1st sell, meaning 'port_value_1st_buy + prices[0] (price on this day) = 0'
    // @port_value_2nd_buy:  port value if this is the 2nd buy, meaning 'port_value_1st_sell - prices[0] (price on this day) = -prices[0]'
    // @port_value_2nd_sell: port value if this is the 2nd sell, meaning 'port_value_2nd_buy + prices[0] (price on this day) = 0'
    //
    // clang-format on

    // starting point, on day 1
    int port_value_1st_buy  = -1 * prices[0];
    int port_value_1st_sell = 0;
    int port_value_2nd_buy  = -1 * prices[0];
    int port_value_2nd_sell = 0;

    for (size_t i = 1; i < prices.size(); ++i) {
        const auto curr_prc = prices[i];
        port_value_1st_buy  = std::max(port_value_1st_buy, -1 * curr_prc);
        port_value_1st_sell = std::max(port_value_1st_sell, port_value_1st_buy + curr_prc);
        port_value_2nd_buy  = std::max(port_value_2nd_buy, port_value_1st_sell - curr_prc);
        port_value_2nd_sell = std::max(port_value_2nd_sell, port_value_2nd_buy + curr_prc);
    }
    return port_value_2nd_sell;
}

///////////////////////////////////////////////////////////////////////////////
//
// https://leetcode.com/problems/best-time-to-buy-and-sell-stock-iv/description/
//
// You are given an array prices where prices[i] is the price of a given stock on the ith day.
// Find the maximum profit you can achieve. You may complete at most k transactions.
//
// https://www.youtube.com/watch?v=t92vU3NvZ8k
//
int max_profit_k_transactions(int k, const std::vector<int>& prices)
{
    // represent the max portfolio value with 0-k transactions:
    // max_port_values[i][0]: max port value with i transactions and hold position;
    // max_port_values[i][1]: max port value with i transactions but no position;
    std::vector<std::array<int, 2>> max_port_values(k, {0, 0});

    const auto price_0 = prices[0];
    for (int i = 0; i < k; ++i) {
        max_port_values[i] = {
            -1 * price_0,    // buy on day-1 and hold
            0,               // sell on day-1, meaning empty portfolio
        };
    }

    for (size_t n = 1; n < prices.size(); ++n) {
        const auto price_n = prices[n];
        for (int i = 0; i < k; ++i) {
            const auto max_port_prev_none = (i > 0 ? max_port_values[i - 1][1] : 0);
            const auto max_port_hold      = std::max(max_port_values[i][0], max_port_prev_none - price_n);
            const auto max_port_none      = std::max(max_port_values[i][1], max_port_hold + price_n);
            max_port_values[i]            = {max_port_hold, max_port_none};
        }
    }

    return max_port_values.back()[1];
}

int max_profit_k_transactions_2(int k, const std::vector<int>& prices)
{
    // number of days
    const int n = (int)prices.size();

    // if there are 0 days then answer will be 0.
    if (n == 0) {
        return 0;
    }

    // Declaration and initialization of the dp vector.
    std::vector<std::vector<int>> dp(k + 1, std::vector<int>(n, 0));

    // Whenever k=0, you cannot make any transaction, so no profit.
    for (int j = 0; j < n; j++) {
        dp[0][j] = 0;
    }

    // Whenever the number of days is 0, the transaction is 0, and thus no profit.
    for (int i = 0; i <= k; i++) {
        dp[i][0] = 0;
    }
    for (int i = 1; i <= k; i++) {
        // Set mx initially to -prices[0], the max portfolio value if holding stock
        int mx = -prices[0];
        for (int j = 1; j < n; j++) {

            // option 1 -> don't do anything on this day
            int op1 = dp[i][j - 1];

            // option 2 -> sell the stock on this day if bought on some day t.
            int op2  = prices[j] + mx;
            dp[i][j] = std::max(op1, op2);

            // Keep updating mx every time.
            mx = std::max(mx, dp[i - 1][j - 1] - prices[j]);
        }
    }
    return dp[k][n - 1];
}

void run_max_profit()
{
    using use_case_t = std::pair<std::vector<int>, int>;

    for (const auto& [prices, exp_v] : {
             use_case_t{{3, 3, 5, 0, 0, 3, 1, 4}, 4},    // k=1, max=4; k=2, max=6; k=3, max=8; k=4, max=8;
             use_case_t{{1, 2, 3, 4, 5}, 4},             // k=any, max=4;
             use_case_t{{7, 6, 4, 3, 1}, 0},             // max=0;
             use_case_t{{2, 4, 1}, 2},                   // k=1, max=2; k=2, max=2;
             use_case_t{{3, 2, 6, 5, 0, 3}, 4},          // k=1, max=4; k=2, max=7; k=3, max=7;
         }) {
        std::cout << "--------------------" << std::endl;

        const auto v1 = max_profit_1_transaction(prices);
        const auto v2 = max_profit_k_transactions(1, prices);
        const auto v3 = max_profit_k_transactions_2(1, prices);
        std::cout << __FUNCTION__ << ": max transaction 1, prices=" << util::dump_array(prices)
                  << ", max_profit_1_transaction=" << v1 << ", " << (exp_v == v1 ? "SUCCESS" : "FAILED")
                  << ", max_profit_k_transactions=" << v2 << ", " << (exp_v == v2 ? "SUCCESS" : "FAILED")
                  << ", max_profit_k_transactions_2=" << v3 << ", " << (exp_v == v3 ? "SUCCESS" : "FAILED")
                  << std::endl;
    }

    for (const auto& [prices, exp_v] : {
             use_case_t{{3, 3, 5, 0, 0, 3, 1, 4}, 6},    // k=1, max=4; k=2, max=6; k=3, max=8; k=4, max=8;
             use_case_t{{1, 2, 3, 4, 5}, 4},             // k=any, max=4;
             use_case_t{{7, 6, 4, 3, 1}, 0},             // max=0;
             use_case_t{{2, 4, 1}, 2},                   // k=1, max=2; k=2, max=2;
             use_case_t{{3, 2, 6, 5, 0, 3}, 7},          // k=1, max=4; k=2, max=7; k=3, max=7;
         }) {
        std::cout << "--------------------" << std::endl;

        const auto v1 = max_profit_2_transactions(prices);
        const auto v2 = max_profit_k_transactions(2, prices);
        const auto v3 = max_profit_k_transactions_2(2, prices);
        std::cout << __FUNCTION__ << ": max transactions 2, prices=" << util::dump_array(prices)
                  << ", max_profit_2_transactions=" << v1 << ", " << (exp_v == v1 ? "SUCCESS" : "FAILED")
                  << ", max_profit_k_transactions=" << v2 << ", " << (exp_v == v2 ? "SUCCESS" : "FAILED")
                  << ", max_profit_k_transactions_2=" << v3 << ", " << (exp_v == v3 ? "SUCCESS" : "FAILED")
                  << std::endl;
    }

    for (const auto& [prices, exp_v] : {
             use_case_t{{3, 3, 5, 0, 0, 3, 1, 4}, 8},    // k=1, max=4; k=2, max=6; k=3, max=8; k=4, max=8;
             use_case_t{{1, 2, 3, 4, 5}, 4},             // k=any, max=4;
             use_case_t{{7, 6, 4, 3, 1}, 0},             // max=0;
             use_case_t{{2, 4, 1}, 2},                   // k=1, max=2; k=2, max=2;
             use_case_t{{3, 2, 6, 5, 0, 3}, 7},          // k=1, max=4; k=2, max=7; k=3, max=7;
         }) {
        std::cout << "--------------------" << std::endl;

        const auto v1 = max_profit_k_transactions(3, prices);
        const auto v2 = max_profit_k_transactions_2(3, prices);
        std::cout << __FUNCTION__ << ": max transactions 3, prices=" << util::dump_array(prices)
                  << ", max_profit_k_transactions=" << v1 << ", " << (exp_v == v1 ? "SUCCESS" : "FAILED")
                  << ", max_profit_k_transactions_2=" << v2 << ", " << (exp_v == v2 ? "SUCCESS" : "FAILED")
                  << std::endl;
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
int array_max_product_subarray(const std::vector<int>& arr)
{
    int max_product_incl = arr[0] * arr[1];
    int min_product_incl = arr[0] * arr[1];
    int max_product_excl = arr[0];
    int min_product_excl = arr[0];
    for (int i = 2; i < (int)arr.size(); ++i) {
        int prior_max_incl = max_product_incl;
        int prior_min_incl = min_product_incl;

        int value        = arr[i];
        int prior_value  = arr[i - 1];
        max_product_incl = std::max({max_product_incl * value, min_product_incl * value, prior_value * value, value});
        min_product_incl = std::min({max_product_incl * value, min_product_incl * value, prior_value * value, value});
        max_product_excl = std::max({max_product_excl, prior_max_incl});
        min_product_excl = std::min({min_product_excl, prior_min_incl});
    }
    return std::max({max_product_incl, max_product_excl});
}
void run_array_max_product_subarray()
{
    using use_case_t = std::pair<std::vector<int>, int>;
    for (auto [arr, exp_v] : {
             use_case_t({2, 3, -2, 4}, 6),
             use_case_t({-2, 0, -1}, 0),
             use_case_t({-2, 3, -4}, 24),
         }) {
        const auto v = array_max_product_subarray(arr);
        std::cout << "array=" << util::dump_array(arr) << ", array_max_product_subarray=" << v << ", "
                  << (exp_v == v ? "SUCCESS" : "FAILED") << std::endl;
    }
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

