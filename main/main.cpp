/*
#include <gtest/gtest.h>

#include "../test/Test_Fixture.h"
#include "../test/Test_algorithm.h"
#include "../test/Test_language.h"
#include "../test/Test_multithreading.h"
#include "../test/Test_utility.h"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
*/

#include "../algorithm/2023.h"
#include "../algorithm/2024.h"
#include "../algorithm/2025.h"
#include "../algorithm/bitwise_operation.h"
#include "../algorithm/stock_max_profit.h"
#include "../algorithm/str_palindrome.h"

#include "../core/my_tuple.h"

#include "../interview/headlands/headlands.h"
#include "../interview/worldquant/orderbook.h"

#include "../language/c++11.h"

#include "../message/detect_func_args.h"
#include "../message/message.h"
#include "../message/chain.h"

#include "../misc/builtin_expect.h"
#include "../misc/atomic_is_lock_free.h"
#include "../misc/gauge_time.h"
#include "../misc/hybrid_range.h"
#include "../misc/macro_variadic.h"
#include "../misc/make_enum.h"
#include "../misc/maybe.h"
#include "../misc/realtime.h"
#include "../misc/speed_dynamic_cast.h"
#include "../misc/xml.h"

#include "../thread/mutex_spinlock.h"
#include "../thread/thread_pool.h"
#include "../thread/lock_rw.h"

int main()
{
    run_number_sum_k();

    run_contiguous_sub_array();

    run_graph_biparties_check();

    run_binary_tree_2_double_list();

    run_merge_arrays();

    run_simplify_dir_path();

    run_possible_words();

    run_max_sub_array();

    run_tree_to_list();

    run_find_pascal_triangle_row();

    run_find_min_steps_to_1_dp();

    run_decode_num_to_letters();

    run_factorization_divisor_prime();

    run_matrix_traverse();

    run_max_profit_2_transactions();

    run_atomic_is_lock_free();

    run_stream_test();

    run_verify_job_chain();

    run_enhanced_job_complte_time();

    run_graph_directed_cycle();

    run_job_complte_time();

    run_max_square_sub_matrix();

    run_max_rectangle_sub_matrix();

    run_max_rectangle();

    run_my_pow();

    run_find_next_greater_with_same_set_digits();

    run_tree_all_paths();

    run_find_min_steps_to_1_bitwise();

    run_find_min_steps_to_N_multi();

    run_find_min_steps_to_x_y();

    run_find_min_steps_to_1();

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

    run_max_profit_1_transaction();

    run_knight_move();

    run_graph_find_max_depth();

    run_graph_find_max_difference();

    run_graph_find_dominator();

    run_max_nodes_exclu_neighbors();

    run_min_path_sum_matrix();

    {
        std::shared_ptr<TreeNode<int>> root(new TreeNode<int>(100));
        std::shared_ptr<TreeNode<int>> n1(new TreeNode<int>(1));
        std::shared_ptr<TreeNode<int>> n2(new TreeNode<int>(2));
        std::shared_ptr<TreeNode<int>> n3(new TreeNode<int>(3));
        std::shared_ptr<TreeNode<int>> n4(new TreeNode<int>(4));
        std::shared_ptr<TreeNode<int>> n5(new TreeNode<int>(5));
        std::shared_ptr<TreeNode<int>> n6(new TreeNode<int>(6));
        std::shared_ptr<TreeNode<int>> n7(new TreeNode<int>(7));
        std::shared_ptr<TreeNode<int>> n8(new TreeNode<int>(8));
        std::shared_ptr<TreeNode<int>> n9(new TreeNode<int>(9));
        std::shared_ptr<TreeNode<int>> n0(new TreeNode<int>(0));

        root->left  = n1.get();
        root->right = n2.get();
        n1->left    = n3.get();
        n1->right   = n4.get();
        n2->left    = n5.get();
        n2->right   = n6.get();
        n3->left    = n7.get();
        n3->right   = n8.get();
        n4->left    = n9.get();
        n4->right   = n0.get();

        std::cout << "n0: 1 == " << tree_count_complete_tree_nodes(n0.get()) << std::endl;
        std::cout << "n4: 3 == " << tree_count_complete_tree_nodes(n4.get()) << std::endl;
        std::cout << "n1: 7 == " << tree_count_complete_tree_nodes(n1.get()) << std::endl;
        std::cout << "root: 11 == " << tree_count_complete_tree_nodes(root.get()) << std::endl;

        std::cout << "n0: 1 == " << tree_count_complete_tree_leaves(n0.get()) << std::endl;
        std::cout << "n4: 2 == " << tree_count_complete_tree_leaves(n4.get()) << std::endl;
        std::cout << "n1: 4 == " << tree_count_complete_tree_leaves(n1.get()) << std::endl;
        std::cout << "root: 6 == " << tree_count_complete_tree_leaves(root.get()) << std::endl;

        int depth = 0;
        std::cout << "check_if_balanced_tree: root=" << check_if_balanced_tree(root.get(), depth) << ", depth=" << depth
                  << std::endl;
        std::cout << "check_if_balanced_tree: n0=" << check_if_balanced_tree(n0.get(), depth) << ", depth=" << depth
                  << std::endl;
        std::cout << "check_if_balanced_tree: n1=" << check_if_balanced_tree(n1.get(), depth) << ", depth=" << depth
                  << std::endl;
        std::cout << "check_if_balanced_tree: n2=" << check_if_balanced_tree(n2.get(), depth) << ", depth=" << depth
                  << std::endl;
        std::cout << "check_if_balanced_tree: n3=" << check_if_balanced_tree(n3.get(), depth) << ", depth=" << depth
                  << std::endl;
        std::cout << "check_if_balanced_tree: n4=" << check_if_balanced_tree(n4.get(), depth) << ", depth=" << depth
                  << std::endl;
        std::cout << "check_if_balanced_tree: n5=" << check_if_balanced_tree(n5.get(), depth) << ", depth=" << depth
                  << std::endl;
        std::cout << "check_if_balanced_tree: n6=" << check_if_balanced_tree(n6.get(), depth) << ", depth=" << depth
                  << std::endl;
        std::cout << "check_if_balanced_tree: n7=" << check_if_balanced_tree(n7.get(), depth) << ", depth=" << depth
                  << std::endl;
        std::cout << "check_if_balanced_tree: n8=" << check_if_balanced_tree(n8.get(), depth) << ", depth=" << depth
                  << std::endl;
        std::cout << "check_if_balanced_tree: n9=" << check_if_balanced_tree(n9.get(), depth) << ", depth=" << depth
                  << std::endl;
    }

    {
        // integer to binary
        auto integer_to_binary = [](uint64_t x) {
            if (x == 0) {
                return std::string{"0"};
            }
            const auto max_size             = 64u;
            char       binary[max_size + 1] = {'0'};
            binary[max_size]                = '\0';
            auto idx                        = max_size;
            while (x) {
                binary[--idx] = ((x & 0x01) ? '1' : '0');
                x             = x >> 1;
            }
            return std::string{binary + idx};
        };
        std::cout << "integer_to_binary(0): " << integer_to_binary(0) << std::endl;
        std::cout << "integer_to_binary(1): " << integer_to_binary(1) << std::endl;
        std::cout << "integer_to_binary(5): " << integer_to_binary(5) << std::endl;
        std::cout << "integer_to_binary(10): " << integer_to_binary(10) << std::endl;
        std::cout << "integer_to_binary(1024): " << integer_to_binary(1024) << std::endl;
        std::cout << "integer_to_binary(1024*1024): " << integer_to_binary(1024 * 1024) << std::endl;
    }

    {
        // log(x)
        auto find_log_n = [](uint64_t x) {
            if (x == 0) {
                throw std::invalid_argument("invalid argument");
            }

            uint32_t count = 0;
            while (x) {
                ++count;
                x = x >> 1;
            }
            return count - 1;
        };
        std::cout << "log(1): " << find_log_n(1) << std::endl;
        std::cout << "log(8): " << find_log_n(8) << std::endl;
        std::cout << "log(32): " << find_log_n(32) << std::endl;
        std::cout << "log(1024): " << find_log_n(1024) << std::endl;
        std::cout << "log(1024*1024): " << find_log_n(1024 * 1024) << std::endl;
    }

    {
        run_lock_reader_writer();
    }

    {
        run_matching_engine_FIFO();
    }

    {
        int arr[] = {8, 1, 5, 2, 6};    // 11
        print_array(arr);
        std::cout << "\n array_max_pair_score: " << array_max_pair_score(arr, ARRAY_SIZE(arr));
        std::cout << "\n array_max_pair_score_v2: " << array_max_pair_score_v2(arr, ARRAY_SIZE(arr)) << std::endl;

        std::cout << "\n array_max_product_subarray: " << array_max_product_subarray(arr, ARRAY_SIZE(arr)) << std::endl;
    }
    {
        int arr[] = {-5, -3, 2, 2, 9, 1};    // 10
        print_array(arr);
        std::cout << "\n array_max_pair_score: " << array_max_pair_score(arr, ARRAY_SIZE(arr));
        std::cout << "\n array_max_pair_score_v2: " << array_max_pair_score_v2(arr, ARRAY_SIZE(arr)) << std::endl;

        std::cout << "\n array_max_product_subarray: " << array_max_product_subarray(arr, ARRAY_SIZE(arr)) << std::endl;
    }
    {
        int arr[] = {7, 1, 5, 3, 6, 4};    // 10
        print_array(arr);
        std::cout << "\n array_max_pair_score: " << array_max_pair_score(arr, ARRAY_SIZE(arr));
        std::cout << "\n array_max_pair_score_v2: " << array_max_pair_score_v2(arr, ARRAY_SIZE(arr)) << std::endl;

        std::cout << "\n array_max_product_subarray: " << array_max_product_subarray(arr, ARRAY_SIZE(arr)) << std::endl;
    }
    {
        int arr[] = {1, 2, 3, 4, 5};    // 8
        print_array(arr);
        std::cout << "\n array_max_pair_score: " << array_max_pair_score(arr, ARRAY_SIZE(arr));
        std::cout << "\n array_max_pair_score_v2: " << array_max_pair_score_v2(arr, ARRAY_SIZE(arr)) << std::endl;

        std::cout << "\n array_max_product_subarray: " << array_max_product_subarray(arr, ARRAY_SIZE(arr)) << std::endl;
    }

    {
        int arr[] = {2, 3, -2, 4};    // 6
        print_array(arr);
        std::cout << "\n array_max_product_subarray: " << array_max_product_subarray(arr, ARRAY_SIZE(arr)) << std::endl;
    }

    {
        int arr[] = {-2, 0, -1};    // 0
        print_array(arr);
        std::cout << "\n array_max_product_subarray: " << array_max_product_subarray(arr, ARRAY_SIZE(arr)) << std::endl;
    }
    {
        int arr[] = {-2, 3, -4};    // 24
        print_array(arr);
        std::cout << "\n array_max_product_subarray: " << array_max_product_subarray(arr, ARRAY_SIZE(arr)) << std::endl;
    }

    build_chain();

    run_message_check();

    detect_class_member();

    {
        std::vector<int> arr = {7, 1, 5, 3, 6, 4};
        print_array(arr);
        std::cout << stock_max_profit(arr) << std::endl;    // 9
    }

    {
        std::vector<int> arr = {7, 1, 5, 3, 6, 4};
        print_array(arr);
        // cout << stock_max_profit_v2(arr) << std::endl; // 7
    }

    {
        std::vector<int> arr = {1, 2, 3, 4, 5};
        print_array(arr);
        // cout << stock_max_profit_v2(arr) << std::endl; // 4
    }

    ////////////////////////////////////////////////////////////////////////////
    std::cout << "ab: " << substring_longest_palindrome("ab") << std::endl;
    std::cout << "aba: " << substring_longest_palindrome("aba") << std::endl;
    std::cout << "babad: " << substring_longest_palindrome("babad") << std::endl;
    std::cout << "cbbd: " << substring_longest_palindrome("cbbd") << std::endl;
    std::cout << "aabcdcbaa: " << substring_longest_palindrome("aabcdcbaa") << std::endl;
    std::cout << "aabcdcbab: " << substring_longest_palindrome("aabcdcbab") << std::endl;

    ////////////////////////////////////////////////////////////////////////////
    // Input:  A[] = [1, 3], B[] = [2]
    // Output: 2
    //
    // Input:  A[] = [2, 4], B[] = [1, 3, 5]
    // Output: 3
    //
    /// Input:  A[] = [1, 3, 6], B[] = [2, 8, 12]
    // Output: 4.5
    //
    // Input:  A[] = [1, 3, 4, 6, 9], B[] = [2, 5, 7, 8, 10]
    // Output: 5.5
    {
        int A[] = {1, 3};
        int B[] = {2};
        std::cout << find_median_two_sorted_arrays(A, ARRAY_SIZE(A), B, ARRAY_SIZE(B)) << std::endl;
    }

    {
        int A[] = {2, 4};
        int B[] = {1, 3, 5};
        std::cout << find_median_two_sorted_arrays(A, ARRAY_SIZE(A), B, ARRAY_SIZE(B)) << std::endl;
    }

    {
        int A[] = {2};
        int B[] = {};
        std::cout << find_median_two_sorted_arrays(A, ARRAY_SIZE(A), B, ARRAY_SIZE(B)) << std::endl;
    }

    {
        int A[] = {1, 3};
        int B[] = {0};
        std::cout << find_median_two_sorted_arrays(A, ARRAY_SIZE(A), B, ARRAY_SIZE(B)) << std::endl;
    }

    {
        int A[] = {1, 3, 6};
        int B[] = {2, 8, 12};
        std::cout << find_median_two_sorted_arrays(A, ARRAY_SIZE(A), B, ARRAY_SIZE(B)) << std::endl;
    }

    {
        int A[] = {1, 3, 4, 6, 9};
        int B[] = {2, 5, 7, 8, 10};
        std::cout << find_median_two_sorted_arrays(A, ARRAY_SIZE(A), B, ARRAY_SIZE(B)) << std::endl;
    }

    ////////////////////////////////////////////////////////////////////////////
    std::cout << "1*2=" << bitwise_multiplication(1, 2) << std::endl;
    std::cout << "0*2=" << bitwise_multiplication(0, 2) << std::endl;
    std::cout << "10*2=" << bitwise_multiplication(10, 2) << std::endl;
    std::cout << "10*118=" << bitwise_multiplication(10, 118) << std::endl;
    std::cout << "-10*118=" << bitwise_multiplication(-10, 118) << std::endl;

    std::cout << "1+2=" << bitwise_addition(1, 2) << std::endl;
    std::cout << "0+2=" << bitwise_addition(0, 2) << std::endl;
    std::cout << "10+2=" << bitwise_addition(10, 2) << std::endl;
    std::cout << "10+118=" << bitwise_addition(10, 118) << std::endl;
    std::cout << "-10+118=" << bitwise_addition(-10, 118) << std::endl;

    std::cout << "1-2=" << bitwise_subtraction(1, 2) << std::endl;
    std::cout << "0-2=" << bitwise_subtraction(0, 2) << std::endl;
    std::cout << "10-2=" << bitwise_subtraction(10, 2) << std::endl;
    std::cout << "10-118=" << bitwise_subtraction(10, 118) << std::endl;
    std::cout << "-10-118=" << bitwise_subtraction(-10, 118) << std::endl;

    ////////////////////////////////////////////////////////////////////////////
    sum_binary("1", 1, "0", 1);    // 1
    sum_binary("0", 1, "0", 1);    // 0
    sum_binary("1", 1, "1", 1);    // 10
    sum_binary("0", 1, "1", 1);    // 1

    sum_binary("101", 3, "110", 3);          // 1011
    sum_binary("1101", 4, "110", 3);         // 10011
    sum_binary("110", 3, "100101011", 9);    // 100110001

    ////////////////////////////////////////////////////////////////////////////
    {
        int arr[] = {1, 2};
        print_all_subsets(arr, sizeof(arr) / sizeof(arr[0]));
    }

    {
        int arr[] = {1, 2, 3};
        print_all_subsets(arr, sizeof(arr) / sizeof(arr[0]));
    }

    {
        int arr[] = {1, 2, 3, 4};
        print_all_subsets(arr, sizeof(arr) / sizeof(arr[0]));
    }

    ////////////////////////////////////////////////////////////////////////////
    {
        int arr[] = {2, 1, 0, 1};
        int len   = sizeof(arr) / sizeof(arr[0]);
        print_array(arr);
        std::cout << "\n--- v1 ---" << std::endl;
        std::cout << std::boolalpha << tower_hopper(arr, len) << std::noboolalpha << std::endl;
        std::cout << "--- v2 ---" << std::endl;
        std::cout << std::boolalpha << tower_hopper_v2(arr, len) << std::noboolalpha << std::endl;
    }

    {
        int arr[] = {1, 2, 0, 0};
        int len   = sizeof(arr) / sizeof(arr[0]);
        print_array(arr);
        std::cout << "\n--- v1 ---" << std::endl;
        std::cout << std::boolalpha << tower_hopper(arr, len) << std::noboolalpha << std::endl;
        std::cout << "--- v2 ---" << std::endl;
        std::cout << std::boolalpha << tower_hopper_v2(arr, len) << std::noboolalpha << std::endl;
    }

    {
        int arr[] = {4, 2, 0, 0, 2, 0};
        int len   = sizeof(arr) / sizeof(arr[0]);
        print_array(arr);
        std::cout << "\n--- v1 ---" << std::endl;
        std::cout << std::boolalpha << tower_hopper(arr, len) << std::noboolalpha << std::endl;
        std::cout << "--- v2 ---" << std::endl;
        std::cout << std::boolalpha << tower_hopper_v2(arr, len) << std::noboolalpha << std::endl;
    }

    {
        int arr[] = {4, 2, 0, 0, 1, 0};
        int len   = sizeof(arr) / sizeof(arr[0]);
        print_array(arr);
        std::cout << "\n--- v1 ---" << std::endl;
        std::cout << std::boolalpha << tower_hopper(arr, len) << std::noboolalpha << std::endl;
        std::cout << "--- v2 ---" << std::endl;
        std::cout << std::boolalpha << tower_hopper_v2(arr, len) << std::noboolalpha << std::endl;
    }

    run_atomic_is_lock_free();
    run_builtin_expect();
    run_for_each();
    run_macro_variadic();

    run_gauge_time();
    run_hybrid_range();
    run_make_enum();
    run_maybe();
    run_realtime();
    run_speed_dynamic_cast();
    run_json();

    return 0;
}

