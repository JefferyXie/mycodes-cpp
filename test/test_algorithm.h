#include <gtest/gtest.h>
#include "../main/header.h"

#include "../algorithm/array_bus_station.h"
#include "../algorithm/array_kth_biggest.h"
#include "../algorithm/array_largest_perm.h"
#include "../algorithm/bike_racer.h"
#include "../algorithm/connected_cell_in_grid.h"
#include "../algorithm/count_luck.h"
#include "../algorithm/cuttree.h"
#include "../algorithm/cuttree_2.h"
#include "../algorithm/deque_maxk.h"
#include "../algorithm/encryption.h"
#include "../algorithm/find_common_ancestor.h"
#include "../algorithm/find_min_packet_length.h"
#include "../algorithm/find_missing_numbers.h"
#include "../algorithm/gridsearch.h"
#include "../algorithm/interpolate.h"
#include "../algorithm/matrix_rotation.h"
#include "../algorithm/max_points_on_line.h"
#include "../algorithm/max_increasing_subsequence.h"
#include "../algorithm/num_days_between.h"
#include "../algorithm/similarpair.h"
#include "../algorithm/sort_k_diff_pairs.h"
#include "../algorithm/subarray_max_mod.h"
#include "../algorithm/remove_duplicates.h"
#include "../algorithm/traversetree.h"

#include "../interview/optiver/judgebitree.h"
#include "../interview/worldquant/orderbook.h"

#include "../language/classtest.h"

// ASSERT_XXX(): If assertion fails then processing of test terminate.
// EXPECT_XXX(): nonfatal failure, allowing processing to continue.

TEST(algorithm, array_bus_station)
{
    const std::vector<int> arr       = {1, 2, 1, 1, 1, 2, 1, 3};
    const auto             result    = array_bus_station(arr);
    const auto             result_v2 = array_bus_station_v2(arr);
    EXPECT_EQ(result, (std::vector<int>{3, 4, 6, 12}));
    EXPECT_EQ(result, result_v2);
}

TEST(algorithm, array_kth_biggest)
{
    {
        const std::vector<int> arr = {1, 2, 3, 4};
        EXPECT_EQ(array_kth_biggest(arr, 1), 4);
        EXPECT_EQ(array_kth_biggest(arr, 2), 3);
        EXPECT_EQ(array_kth_biggest(arr, 3), 2);
        EXPECT_EQ(array_kth_biggest(arr, 4), 1);

        EXPECT_EQ(array_kth_biggest_v2(arr, 1), 4);
        EXPECT_EQ(array_kth_biggest_v2(arr, 2), 3);
        EXPECT_EQ(array_kth_biggest_v2(arr, 3), 2);
        EXPECT_EQ(array_kth_biggest_v2(arr, 4), 1);
    }

    {
        const std::vector<int> arr = {4, 2, 3, 5, 1};
        EXPECT_EQ(array_kth_biggest(arr, 1), 5);
        EXPECT_EQ(array_kth_biggest(arr, 2), 4);
        EXPECT_EQ(array_kth_biggest(arr, 3), 3);
        EXPECT_EQ(array_kth_biggest(arr, 4), 2);
        EXPECT_EQ(array_kth_biggest(arr, 5), 1);

        EXPECT_EQ(array_kth_biggest_v2(arr, 1), 5);
        EXPECT_EQ(array_kth_biggest_v2(arr, 2), 4);
        EXPECT_EQ(array_kth_biggest_v2(arr, 3), 3);
        EXPECT_EQ(array_kth_biggest_v2(arr, 4), 2);
        EXPECT_EQ(array_kth_biggest_v2(arr, 5), 1);
    }

    {
        const std::vector<int> arr = {10, 5, 4, 3, 48, 6, 2, 33, 53, 10};
        EXPECT_EQ(array_kth_biggest(arr, 1), 53);
        EXPECT_EQ(array_kth_biggest(arr, 2), 48);
        EXPECT_EQ(array_kth_biggest(arr, 3), 33);
        EXPECT_EQ(array_kth_biggest(arr, 4), 10);
        EXPECT_EQ(array_kth_biggest(arr, 5), 10);
        EXPECT_EQ(array_kth_biggest(arr, 6), 6);

        EXPECT_EQ(array_kth_biggest_v2(arr, 1), 53);
        EXPECT_EQ(array_kth_biggest_v2(arr, 2), 48);
        EXPECT_EQ(array_kth_biggest_v2(arr, 3), 33);
        EXPECT_EQ(array_kth_biggest_v2(arr, 4), 10);
        EXPECT_EQ(array_kth_biggest_v2(arr, 5), 10);
        EXPECT_EQ(array_kth_biggest_v2(arr, 6), 6);
    }
}

TEST(algorithm, array_largest_permutation)
{
    {
        const std::vector<int> arr    = {1, 2, 3, 4};
        const auto             result = array_largest_permutation(1, arr);
        EXPECT_EQ(result, (std::vector<int>{4, 2, 3, 1}));
    }

    {
        const std::vector<int> arr    = {1, 2, 3, 4};
        const auto             result = array_largest_permutation(2, arr);
        EXPECT_EQ(result, (std::vector<int>{4, 3, 2, 1}));
    }

    {
        const std::vector<int> arr    = {1, 2, 3, 4};
        const auto             result = array_largest_permutation(3, arr);
        EXPECT_EQ(result, (std::vector<int>{4, 3, 2, 1}));
    }

    {
        const std::vector<int> arr    = {4, 2, 3, 5, 1};
        const auto             result = array_largest_permutation(1, arr);
        EXPECT_EQ(result, (std::vector<int>{5, 2, 3, 4, 1}));
    }

    {
        const std::vector<int> arr    = {4, 2, 3, 5, 1};
        const auto             result = array_largest_permutation(2, arr);
        EXPECT_EQ(result, (std::vector<int>{5, 4, 3, 2, 1}));
    }

    {
        const std::vector<int> arr    = {2, 1, 3};
        const auto             result = array_largest_permutation(1, arr);
        EXPECT_EQ(result, (std::vector<int>{3, 1, 2}));
    }
}

TEST(algorithm, FindCommonAncestor)
{
    std::shared_ptr<tree_node_int_t> root(new tree_node_int_t(100));
    std::shared_ptr<tree_node_int_t> n1(new tree_node_int_t(1));
    std::shared_ptr<tree_node_int_t> n2(new tree_node_int_t(2));
    std::shared_ptr<tree_node_int_t> n3(new tree_node_int_t(3));
    std::shared_ptr<tree_node_int_t> n4(new tree_node_int_t(4));
    std::shared_ptr<tree_node_int_t> n5(new tree_node_int_t(5));
    std::shared_ptr<tree_node_int_t> n6(new tree_node_int_t(6));
    std::shared_ptr<tree_node_int_t> n7(new tree_node_int_t(7));
    std::shared_ptr<tree_node_int_t> n8(new tree_node_int_t(8));
    std::shared_ptr<tree_node_int_t> n9(new tree_node_int_t(9));
    std::shared_ptr<tree_node_int_t> n0(new tree_node_int_t(0));

    root->left  = n1.get();
    root->right = n2.get();
    n1->left    = n3.get();
    n1->right   = n4.get();
    n2->left    = n5.get();
    n2->right   = n6.get();
    n3->left    = n7.get();
    n3->right   = n8.get();
    n4->right   = n9.get();
    n9->left    = n0.get();

    {
        auto ancestor1 = find_common_ancestor<int>(root.get(), n7.get(), n0.get());
        EXPECT_EQ(ancestor1, n1.get());
        auto ancestor2 = find_common_ancestor<int>(root.get(), n5.get(), n9.get());
        EXPECT_EQ(ancestor2, root.get());
        auto ancestor3 = find_common_ancestor<int>(root.get(), n3.get(), n0.get());
        EXPECT_EQ(ancestor3, n1.get());
        auto ancestor4 = find_common_ancestor<int>(root.get(), n1.get(), n7.get());
        EXPECT_EQ(ancestor4, n1.get());
        auto ancestor5 = find_common_ancestor<int>(root.get(), n0.get(), n6.get());
        EXPECT_EQ(ancestor5, root.get());
        auto ancestor6 = find_common_ancestor<int>(root.get(), n9.get(), n5.get());
        EXPECT_EQ(ancestor6, root.get());
    }

    {
        auto ancestor1 = find_common_ancestor_v2<int>(root.get(), n7.get(), n0.get());
        EXPECT_EQ(ancestor1, n1.get());
        auto ancestor2 = find_common_ancestor_v2<int>(root.get(), n5.get(), n9.get());
        EXPECT_EQ(ancestor2, root.get());
        auto ancestor3 = find_common_ancestor_v2<int>(root.get(), n3.get(), n0.get());
        EXPECT_EQ(ancestor3, n1.get());
        auto ancestor4 = find_common_ancestor_v2<int>(root.get(), n1.get(), n7.get());
        EXPECT_EQ(ancestor4, n1.get());
        auto ancestor5 = find_common_ancestor_v2<int>(root.get(), n0.get(), n6.get());
        EXPECT_EQ(ancestor5, root.get());
        auto ancestor6 = find_common_ancestor_v2<int>(root.get(), n9.get(), n5.get());
        EXPECT_EQ(ancestor6, root.get());
    }
}

TEST(algorithm, traverse)
{
    tree_node_int_t root(0);
    tree_node_int_t n1(1);
    tree_node_int_t n2(2);
    tree_node_int_t n3(3);
    tree_node_int_t n4(4);
    tree_node_int_t n5(5);
    tree_node_int_t n6(6);
    tree_node_int_t n7(7);
    tree_node_int_t n8(8);
    tree_node_int_t n9(9);
    tree_node_int_t n10(10);
    tree_node_int_t n11(11);
    root.left  = &n1;
    root.right = &n2;
    n1.left    = &n3;
    n1.right   = &n4;
    n2.left    = &n5;
    n2.right   = &n6;
    n3.right   = &n7;
    n4.left    = &n8;
    n4.right   = &n9;
    n6.left    = &n10;
    n8.left    = &n11;

    TraverseInorder(&root);
    TraversePostorder(&root);
}

TEST(algorithm, numdaysbetween)
{
    int a = num_days_between(2010, 5, 1, 2011, 5, 1);
    EXPECT_EQ(a, 365);
    int b = num_days_between(1900, 1, 10, 1901, 3, 1);
    EXPECT_EQ(b, 415);
    int c = num_days_between(1999, 10, 1, 2015, 12, 10);
    EXPECT_EQ(c, 5914);
    int d = num_days_between(1644, 3, 21, 2016, 2, 23);
    EXPECT_EQ(d, 135843);
    int e = num_days_between(2016, 3, 21, 2016, 3, 30);
    EXPECT_EQ(e, 9);
    int f = num_days_between(1644, 3, 21, 1644, 3, 21);
    EXPECT_EQ(f, 0);
    std::cout << a << ", " << b << ", " << c << ", " << d << ", " << e << ", " << f << std::endl;
}

TEST(DISABLED_algorithm, similarpair)
{
    int arr[][2] = {
        {1, 7},  {1, 10}, {10, 15}, {15, 4},  {15, 16}, {16, 18}, {16, 17}, {17, 2},  {10, 5}, {5, 3},
        {3, 19}, {1, 9},  {9, 11},  {11, 13}, {11, 12}, {12, 8},  {12, 20}, {11, 14}, {1, 6},
    };
    int a = SimilarPair(5, arr);
    EXPECT_EQ(a, 23);
}

TEST(algorithm, kdiffparis)
{
    std::vector<int> arr = {1, 5, 3, 4, 2};
    int              a   = sort_k_diff_pairs(arr, 2);
    EXPECT_EQ(a, 3);
}

TEST(algorithm, maxconnectedcells)
{
    std::vector<std::vector<int>> mt1 = {
        {0, 1, 1},
        {1, 0, 0},
        {0, 1, 0},
    };
    int a = max_connected_cell_in_grid(mt1);
    EXPECT_EQ(a, 4);
    std::vector<std::vector<int>> mt2 = {
        {0, 1, 1, 1, 1}, {1, 0, 0, 0, 1}, {1, 1, 0, 1, 0}, {0, 1, 0, 1, 1}, {0, 1, 1, 1, 0},
    };
    int b = max_connected_cell_in_grid(mt2);
    EXPECT_EQ(b, 15);
}

TEST(algorithm, max_increasing_subsequence)
{
    std::vector<int> arr1 = {10, 7, 6};
    std::vector<int> arr2 = {10, 7, 12, 30, 22, 35, 60, 9};
    std::vector<int> arr3 = {10, 7, 12, 30, 11, 5, 60, 9};
    EXPECT_EQ(max_increasing_subsequence(arr1), 1);
    EXPECT_EQ(max_increasing_subsequence(arr2), 5);
    EXPECT_EQ(max_increasing_subsequence(arr3), 4);
}

TEST(algorithm, subarray_max_mod)
{
    long long arr[] = {3, 3, 9, 9, 5};
    auto      a     = subarray_max_mod(arr, sizeof(arr) / sizeof(arr[0]), 7);
    EXPECT_EQ(a, 6);
    long long arr1[] = {846930887,  1681692778, 1714636916, 1957747794, 424238336,  719885387,  1649760493, 596516650,
                        1189641422, 1025202363, 1350490028, 783368691,  1102520060, 2044897764, 1967513927, 1365180541,
                        1540383427, 304089173,  1303455737, 35005212,   521595369,  294702568,  1726956430, 336465783,
                        861021531,  278722863,  233665124,  2145174068, 468703136,  1101513930, 1801979803, 1315634023,
                        635723059,  1369133070, 1125898168, 1059961394, 2089018457, 628175012,  1656478043, 1131176230,
                        1653377374, 859484422,  1914544920, 608413785,  756898538,  1734575199, 1973594325, 149798316,
                        2038664371, 1129566414};
    auto      b      = subarray_max_mod(arr1, sizeof(arr) / sizeof(arr[0]), 1804289384);
    EXPECT_EQ(b, 1745498720);
}

TEST(algorithm, missingnumbers)
{
    int           A[] = {203, 204, 205, 206, 207, 208, 203, 204, 205, 206};
    int           B[] = {203, 204, 204, 205, 206, 207, 205, 208, 203, 206, 205, 206, 204};
    auto          s   = find_missing_numbers(A, sizeof(A) / sizeof(A[0]), B, sizeof(B) / sizeof(B[0]));
    std::set<int> ss{204, 205, 206};
    EXPECT_EQ(s, ss);
}

TEST(DISABLED_algorithm, bikeracer)
{
    std::vector<Coordinate> bikers1 = {
        {0, 1},
        {0, 2},
        {0, 3},
    };
    std::vector<Coordinate> bikes1 = {
        {100, 1},
        {200, 2},
        {300, 3},
    };
    int  K1            = 2;
    auto shortestTime1 = BikeRacerShortestTime(bikers1, bikes1, K1);
    EXPECT_EQ(shortestTime1, 40000);

    std::vector<Coordinate> bikers2 = {
        {286, 686}, {439, 775}, {34, 471},  {116, 512}, {206, 632},
        {549, 365}, {961, 702}, {999, 514}, {81, 249},  {538, 268},
    };
    std::vector<Coordinate> bikes2 = {
        {261, 789}, {929, 217}, {909, 187}, {729, 411}, {309, 147},
        {185, 184}, {960, 947}, {770, 624}, {136, 346}, {979, 238},
    };
    /*
    // the algorithm needs to be optimized so that this case won't time out
    int K2 = 5;
    auto shortestTime2 = BikeRacerShortestTime(bikers2, bikes2, K2);
    EXPECT_EQ(shortestTime2, 42565);*/

    std::vector<Coordinate> bikers3 = {
        {411, 353}, {162, 727}, {6, 293},  {370, 16},  {334, 610},
        {201, 999}, {354, 272}, {475, 73}, {474, 356}, {651, 607},
    };
    std::vector<Coordinate> bikes3 = {
        {213, 835}, {871, 716}, {161, 707}, {348, 439}, {163, 847},
        {200, 67},  {146, 926}, {571, 714}, {82, 152},  {115, 293},
    };
    int  K3            = 2;
    auto shortestTime3 = BikeRacerShortestTime(bikers3, bikes3, K3);
    EXPECT_EQ(shortestTime3, 8354);

    std::vector<Coordinate> bikers4 = {
        {145, 862},
        {533, 105},
        {34, 192},
        {897, 656},
    };
    std::vector<Coordinate> bikes4 = {
        {902, 518},
        {78, 108},
        {658, 369},
        {127, 364},
    };
    int  K4            = 2;
    auto shortestTime4 = BikeRacerShortestTime(bikers4, bikes4, K4);
    EXPECT_EQ(shortestTime4, 19069);
}

TEST(DISABLED_algorithm, permutation)
{
    std::vector<int> source = {1, 2, 3, 4};
    std::vector<int> target;
    EXPECT_EQ(PossiblePermutation_WithOrder(source, 0, 4, target), 24);
}

TEST(algorithm, countluck)
{
    std::vector<std::vector<char>> forest;
    auto                           fillforest = [&forest](std::vector<std::string>& ar) {
        forest.clear();
        for_each(ar.begin(), ar.end(), [&forest](std::string s) {
            std::vector<char> v;
            for_each(s.begin(), s.end(), [&v](char c) {
                v.push_back(c);
            });
            forest.push_back(v);
        });
    };
    std::vector<std::string> arr1 = {
        {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."}, {"M........................................"},
        {"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX."}, {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."},
        {"........................................."}, {".XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"},
        {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."}, {"........................................."},
        {"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX."}, {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."},
        {"........................................."}, {".XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"},
        {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."}, {"........................................."},
        {"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX."}, {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."},
        {"........................................."}, {".XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"},
        {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."}, {"........................................."},
        {"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX."}, {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."},
        {"........................................."}, {".XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"},
        {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."}, {"........................................."},
        {"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX."}, {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."},
        {"........................................."}, {".XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"},
        {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."}, {"........................................."},
        {"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX."}, {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."},
        {"........................................."}, {".XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"},
        {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."}, {"........................................."},
        {"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX."}, {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."},
        {".*......................................."},
    };    // 280
    fillforest(arr1);
    int a1 = count_luck(forest);
    EXPECT_EQ(a1, 280);

    std::vector<std::string> arr2 = {
        {"XXXXXXXXXXXXXXXXX"}, {"XXX.XX.XXXXXXXXXX"}, {"XX.*..M.XXXXXXXXX"},
        {"XXX.XX.XXXXXXXXXX"}, {"XXXXXXXXXXXXXXXXX"},
    };    // 1*/
    fillforest(arr2);
    int a2 = count_luck(forest);
    EXPECT_EQ(a2, 1);

    std::vector<std::string> arr3 = {
        {".X.X......X"},
        {".X*.X.XXX.X"},
        {".XX.X.XM..."},
        {"......XXXX."},
    };    // 3
    fillforest(arr3);
    int a3 = count_luck(forest);
    EXPECT_EQ(a3, 3);

    std::vector<std::string> arr4 = {
        {"..........*"}, {".XXXXXXXXXX"}, {"..........."}, {"XXXXXXXXXX."}, {"M.........."},
    };    // 0
    fillforest(arr4);
    int a4 = count_luck(forest);
    EXPECT_EQ(a4, 0);

    std::vector<std::string> arr5 = {
        {"X.XXXX.XX....X.XX...X.XXXXXXXXX"}, {"X.XXX...XXX.X..XXX.XX..XXXXXXXX"}, {"...X.XX..X...X..XX.X..XXXXXXXXX"},
        {"X.X..X..X.X.X..X.....XX.XXXXXXX"}, {"...X..X.X....X.X.X.X.X..XXXXXXX"}, {".X..X....X.X.....XX....XXXXXXXX"},
        {"..X..XX.X.X..XX.X..XX.XXXXXXXXX"}, {".XXX.X.....X.X.X*.X.XX.XXXXXXXX"}, {"X..X..X.X.X.....X....X..XXXXXXX"},
        {"..X.X....X..XXXX..XXX..XXXXXXXX"}, {"X....XXX..X.....X...X.XXXXXXXXX"}, {"..XX.....X.X.X.X..X.X..XXXXXXXX"},
        {"XX.X.X.X...X.XX.X..X..X..XXXXXX"}, {".M...XXXX.X.....X.X.X...XXXXXXX"}, {"X.XXX..X...X.X.X..X..X.XXXXXXXX"},
        {".XX...X.XX..X..X.X.X....XXXXXXX"}, {"....X......X..X......XXX.XXXXXX"}, {"X.X.XX.XXX..X.X.XX.XX.....XXXXX"},
        {"X..X..X....XX.....X...X.X..XXXX"}, {"..X.X...XX....X.XX..X.X..XX.XXX"}, {".X..X.X.X.X.XX...X.X.X.XX...XXX"},
        {"XX.X...X....X..X........XX.X.XX"}, {".....XX..X.X.XX..XX.X.X......XX"}, {"X.X.XX..X.XX....X..XXXXX.XXXX.X"},
        {"XX..X.XX....XXX...X.X.X........"}, {".XXXX...XXX.....X......XX.XX.XX"}, {"......XX....X.X..XX.XX...XX.XXX"},
        {"X.X.X....X.X...X...X...X.....X."}, {"XX...X.X..X..X.XXXX.XX.X.X.X..."}, {"XXXX..X..XXX.X......X.X...X..XX"},
        {"XXX..XXX..X..XX.X.X....XX..X..X"}, {"X.XX..XX.X..X...X..X.X.XX.X..XX"}, {"...XXXX..X.X..X..X..XXX...XXXXX"},
        {"XX...X..XX.XX..X..X.XX..X..XXXX"}, {"XX.XX..XXXXX.X.X.X...X.XXXXXXXX"}, {"X.....X.XXX..X.X..X.XXXXXXXXXXX"},
        {"..X.X......X...X.X..XXXXXXXXXXX"},
    };    // 21
    fillforest(arr5);
    int a5 = count_luck(forest);
    EXPECT_EQ(a5, 21);

    std::vector<std::string> arr6 = {
        {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."},
        {"M.......................................*"},
        {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."},
    };    // 20
    fillforest(arr6);
    int a6 = count_luck(forest);
    EXPECT_EQ(a6, 20);
}

TEST(algorithm, cuttree)
{
    std::vector<int>                 vertices = {100, 200, 100, 500, 100, 600};
    std::vector<std::pair<int, int>> edges    = {
        {1, 2}, {2, 3}, {2, 5}, {4, 5}, {5, 6},
    };
    int a = CutTree(vertices, edges);
    EXPECT_EQ(a, 400);
}

TEST(algorithm, cuttree_2)
{
    std::vector<int>                 vertices = {100, 200, 100, 500, 100, 600};
    std::vector<std::pair<int, int>> edges    = {
        {1, 2}, {2, 3}, {2, 5}, {4, 5}, {5, 6},
    };
    int a = CutTree_2(vertices, edges);
    EXPECT_EQ(a, 400);
}

TEST(algorithm, gridsearch)
{
    std::vector<std::string> G = {"1234567890", "0987654321", "1111111111", "1111111111", "2222222222"};
    std::vector<std::string> P = {
        "876543",
        "111111",
        "111111",
    };
    auto a = gridsearch(G, P);
    EXPECT_EQ(a, true);
    /*
    int T = 1;
    cin >> T;
    while (T--) {
        std::string s;
        unsigned int G_rows, G_cols, P_rows, P_cols;
        cin >> G_rows >> G_cols;
        getline(cin, s);
        std::vector<std::string> G(G_rows);
        for (unsigned int i = 0; i < G_rows; ++i) {
            getline(cin, G[i]);
        }
        cin >> P_rows >> P_cols;
        getline(cin, s);
        std::vector<std::string> P(P_rows);
        for (unsigned int i = 0; i < P_rows; ++i) {
            getline(cin, P[i]);
        }
        std::cout << (GridSearch(G, P) ? "YES" : "NO") << std::endl;
    }*/
}

TEST(algorithm, matrixrotation)
{
    auto dumpMatrix = [](const std::vector<std::vector<int>>& m) {
        for_each(begin(m), end(m), [](const std::vector<int>& v) {
            for_each(begin(v), end(v), [](int a) {
                std::cout << a << " ";
            });
            std::cout << std::endl;
        });
    };
    int                           R      = 1;
    std::vector<std::vector<int>> matrix = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16},
    };
    std::vector<std::vector<int>> result = {
        {2, 3, 4, 8},
        {1, 7, 11, 12},
        {5, 6, 10, 16},
        {9, 13, 14, 15},
    };
    auto r1 = matrix_rotation(matrix, R);
    dumpMatrix(r1);
    EXPECT_EQ(r1, result);

    R      = 7;
    matrix = {
        {1, 2, 3, 4}, {7, 8, 9, 10}, {13, 14, 15, 16}, {19, 20, 21, 22}, {25, 26, 27, 28},
    };
    result = {
        {28, 27, 26, 25}, {22, 9, 15, 19}, {16, 8, 21, 13}, {10, 14, 20, 7}, {4, 3, 2, 1},
    };
    auto r2 = matrix_rotation(matrix, R);
    dumpMatrix(r2);
    EXPECT_EQ(r2, result);

    /*
    int M, N, R;
    cin >> M >> N >> R;
    matrix = std::vector<std::vector<int> >(M, std::vector<int>(N,0));
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j)
            cin >> matrix[i][j];
    }*/
}

TEST(algorithm, deque_maxk)
{
    int  arr1[] = {3, 4, 6, 3, 4};
    auto r1     = print_k_max_1(arr1, sizeof(arr1) / sizeof(arr1[0]), 2);
    EXPECT_EQ(r1, "4 6 6 4");
    auto r21 = print_k_max_2(arr1, sizeof(arr1) / sizeof(arr1[0]), 2);
    EXPECT_EQ(r21, "4 6 6 4");

    int  arr2[] = {3, 4, 5, 8, 1, 4, 10};
    auto r2     = print_k_max_1(arr2, sizeof(arr2) / sizeof(arr2[0]), 4);
    EXPECT_EQ(r2, "8 8 8 10");
    auto r22 = print_k_max_2(arr2, sizeof(arr2) / sizeof(arr2[0]), 4);
    EXPECT_EQ(r22, "8 8 8 10");
}

TEST(algorithm, encryption)
{
    std::string t1 = "ifmanwasmeanttostayonthegroundgodwouldhavegivenusroots";
    auto        a1 = encryption(t1);
    EXPECT_EQ(a1, "imtgdvs fearwer mayoogo anouuio ntnnlvt wttddes aohghn sseoau");

    std::string t2 = "haveaniceday";
    auto        a2 = encryption(t2);
    EXPECT_EQ(a2, "hae and via ecy");

    std::string t3 = "chillout";
    auto        a3 = encryption(t3);
    EXPECT_EQ(a3, "clu hlt io");
}

TEST(algorithm, removeduplicates)
{
    auto printarr = [](int* arr, int len) {
        for (int i = 0; i < len; ++i)
            std::cout << *(arr + i) << " ";
        std::cout << std::endl;
    };
    int arr1[] = {1, 2, 3, 4, 5};
    int len1   = remove_duplicates_1(arr1, sizeof(arr1) / sizeof(arr1[0]));
    printarr(arr1, len1);
    EXPECT_EQ(len1, 5);

    int arr2[] = {1, 2, 2, 3, 1, 5, 6, 0};
    int len2   = remove_duplicates_1(arr2, sizeof(arr2) / sizeof(arr2[0]));
    printarr(arr2, len2);
    EXPECT_EQ(len2, 6);

    int arr3[] = {1, 2, 2, 2, 3};
    int len3   = remove_duplicates_1(arr3, sizeof(arr3) / sizeof(arr3[0]));
    printarr(arr3, len3);
    EXPECT_EQ(len3, 3);

    int arr4[] = {1, 1, 1, 2, 2, 5, 3, 2, 1, 3, 2};
    int len4   = remove_duplicates_1(arr4, sizeof(arr4) / sizeof(arr4[0]));
    printarr(arr4, len4);
    EXPECT_EQ(len4, 4);
}

TEST(interview, judgeBiTree)
{
    std::string tc1 = JudgeBiTree("(B,D) (D,E) (A,B) (C,F) (E,G) (A,C)");
    EXPECT_EQ(tc1, "(A(B(D(E(G))))(C(F)))");

    std::string tc2 = JudgeBiTree("(A,B) (A,C) (B,D) (D,C)");
    EXPECT_EQ(tc2, "E5");

    std::string tc3 = JudgeBiTree("(A,B) (A,C) (B,D) (E,F) (F,G) (F,H)");
    EXPECT_EQ(tc3, "E4");

    std::string tc4 = JudgeBiTree("(A,B) (B,D) (B,C) (C,A)");
    EXPECT_EQ(tc4, "E5");

    std::string tc5 = JudgeBiTree("(A,B) (B,C) (A,B) (A,C)");
    EXPECT_EQ(tc5, "E2");

    std::string tc6 = JudgeBiTree("(A,B) (A,C) (A,D) (B,E)");
    EXPECT_EQ(tc6, "E3");

    std::string tc7 = JudgeBiTree("(U,P) (O,L) (A,S) (N,O) (S,T) (U,M) (A,N) (S,U) (L,D)");
    EXPECT_EQ(tc7, "(A(N(O(L(D))))(S(T)(U(M)(P))))");

    std::string tc8 = JudgeBiTree("(A,B) (B,C) (A,B)");
    EXPECT_EQ(tc8, "E2");

    std::string tc9 = JudgeBiTree("(A,C) (A,B) (B,Q) (B,P) (C,D) (Q,Z) (Q,X)");
    EXPECT_EQ(tc9, "(A(B(P)(Q(X)(Z)))(C(D)))");

    std::string tc10 = JudgeBiTree("(A,B)   (A,C) (B, E) (B,F)");
    EXPECT_EQ(tc10, "E1");

    std::string tc11 = JudgeBiTree("(P,J) (X,T) (R,E) (P,C) (X,B) (R,X)");
    EXPECT_EQ(tc11, "E4");

    std::string tc12 = JudgeBiTree("(X,Y) (R,D) (D,P) (R,A) (X,Z) (Z,T) (Z,B) (R,G)");
    EXPECT_EQ(tc12, "E3");

    std::string tc13 = JudgeBiTree("(R,N) (U,T) (C,H) (Y,B) (R,A) (Y,C) (K,Y) (B,R) (H,E) (H,S) (K,U) (T,M)");
    EXPECT_EQ(tc13, "(K(U(T(M)))(Y(B(R(A)(N)))(C(H(E)(S)))))");

    std::string tc14 = JudgeBiTree("(K,M) (P,L) (L,S) (Y,A) (T,P) (T,K) (L,E) (K,Y)");
    EXPECT_EQ(tc14, "(T(K(M)(Y(A)))(P(L(E)(S))))");

    std::string tc15 = JudgeBiTree("(A,B) (A,C (B,D) B,E)");
    EXPECT_EQ(tc15, "E1");

    std::string tc16 = JudgeBiTree("   (P,B) (E,X) (P,D) (E,Y) (P,A)");
    EXPECT_EQ(tc16, "E1");
}

TEST(interview, matching_engine_FIFO)
{
    const std::initializer_list<instrument_id_t> instruments = {10, 11};
    MatchingEngine_FIFO                          engine{instruments};

    // new order
    {
        order_t order{
            .side     = side_t::Bid,
            .id       = 10,
            .price    = 5,
            .quantity = 3,
        };
        auto result = engine.handle_order_new<side_t::Bid>(10, order.id, order.price, order.quantity);
        EXPECT_EQ(result.result_code, matching_result_code_t::NoFill);
        EXPECT_EQ(result.filled_quantity, 0);
        EXPECT_EQ(result.matching_orders.size(), 0);
    }

    // new order
    {
        order_t order{
            .side     = side_t::Ask,
            .id       = 50,
            .price    = 7,
            .quantity = 5,
        };
        auto result = engine.handle_order_new<side_t::Ask>(10, order.id, order.price, order.quantity);
        EXPECT_EQ(result.result_code, matching_result_code_t::NoFill);
        EXPECT_EQ(result.filled_quantity, 0);
        EXPECT_EQ(result.matching_orders.size(), 0);
    }

    {
        order_t order{
            .side     = side_t::Ask,
            .id       = 100,
            .price    = 5,
            .quantity = 1,
        };
        // full filled
        auto result = engine.handle_order_new<side_t::Ask>(10, order.id, order.price, order.quantity);
        EXPECT_EQ(result.result_code, matching_result_code_t::FullFilled);
        EXPECT_EQ(result.filled_quantity, 1);
        EXPECT_EQ(result.matching_orders.size(), 1);
        EXPECT_EQ(result.matching_orders.front().id, 10);

        // partial filled
        order.id       = 101;
        order.quantity = 4;
        result         = engine.handle_order_new<side_t::Ask>(10, order.id, order.price, order.quantity);
        EXPECT_EQ(result.result_code, matching_result_code_t::PartialFilled);
        EXPECT_EQ(result.filled_quantity, 2);
        EXPECT_EQ(result.matching_orders.size(), 1);
        EXPECT_EQ(result.matching_orders.front().id, 10);

        // bid side book is empty now
        auto existing_order = engine.find_order(10, 10);
        EXPECT_EQ(existing_order, nullptr);

        existing_order = engine.find_order(10, 101);
        EXPECT_NE(existing_order, nullptr);
        EXPECT_EQ(existing_order->quantity, 2);
        EXPECT_EQ(existing_order->side, side_t::Ask);

        // A:50     5@7
        // A:101    2@5
    }

    {
        order_t order{
            .side     = side_t::Ask,
            .id       = 51,
            .price    = 6,
            .quantity = 10,
        };
        auto result = engine.handle_order_new<side_t::Ask>(10, order.id, order.price, order.quantity);
        EXPECT_EQ(result.result_code, matching_result_code_t::NoFill);
        EXPECT_EQ(result.filled_quantity, 0);

        // A:50      5@7
        // A:51     10@6
        // A:101     2@5
    }

    // replacement, increase size, A:50 7@7 > 10@7
    {
        order_t order{
            .side     = side_t::Ask,
            .id       = 50,
            .price    = 7,
            .quantity = 7,
        };
        auto result = engine.handle_order_replacement<side_t::Ask>(10, order.id, order.price, order.quantity);
        EXPECT_EQ(result.result_code, matching_result_code_t::NoFill);

        auto existing_order = engine.find_order(10, 50);
        EXPECT_NE(existing_order, nullptr);
        EXPECT_EQ(existing_order->price, 7);
        EXPECT_EQ(existing_order->quantity, 7);

        // A:50      7@7
        // A:51     10@6
        // A:101     2@5
    }

    // replacement, improve price, A:50 7@7 > 7@6
    {
        order_t order{
            .side     = side_t::Ask,
            .id       = 50,
            .price    = 6,
            .quantity = 7,
        };
        auto result = engine.handle_order_replacement<side_t::Ask>(10, order.id, order.price, order.quantity);
        EXPECT_EQ(result.result_code, matching_result_code_t::NoFill);

        auto existing_order = engine.find_order(10, 50);
        EXPECT_NE(existing_order, nullptr);
        EXPECT_EQ(existing_order->price, 6);
        EXPECT_EQ(existing_order->quantity, 7);

        // A:50      7@6
        // A:51     10@6
        // A:101     2@5
    }

    {
        order_t order{
            .side     = side_t::Bid,
            .id       = 20,
            .price    = 6,
            .quantity = 15,
        };
        // partial filled
        auto result = engine.handle_order_new<side_t::Bid>(10, order.id, order.price, order.quantity);
        EXPECT_EQ(result.result_code, matching_result_code_t::FullFilled);
        EXPECT_EQ(result.filled_quantity, 15);
        EXPECT_EQ(result.matching_orders.size(), 3);

        auto existing_order = engine.find_order(10, 101);
        EXPECT_EQ(existing_order, nullptr);

        existing_order = engine.find_order(10, 51);
        EXPECT_EQ(existing_order, nullptr);

        existing_order = engine.find_order(10, 50);
        EXPECT_NE(existing_order, nullptr);
        EXPECT_EQ(existing_order->price, 6);
        EXPECT_EQ(existing_order->quantity, 4);

        existing_order = engine.find_order(10, 20);
        EXPECT_EQ(existing_order, nullptr);

        // A:50      4@6
    }

    {
        order_t order{
            .side     = side_t::Bid,
            .id       = 21,
            .price    = 5,
            .quantity = 3,
        };
        auto result = engine.handle_order_new<side_t::Bid>(10, order.id, order.price, order.quantity);
        EXPECT_EQ(result.result_code, matching_result_code_t::NoFill);

        auto existing_order = engine.find_order(10, 21);
        EXPECT_NE(existing_order, nullptr);

        // A:50      4@6
        // B:21      3@5
    }
}

TEST(algorithm, interpolate)
{
    {
        std::vector<int64_t> series = {1};
        std::vector<int>     values = {10};

        EXPECT_EQ(interpolate(series, values, 1), 10);
        EXPECT_EQ(interpolate(series, values, 2), 0);
        EXPECT_EQ(interpolate(series, values, 3), 0);
        EXPECT_EQ(interpolate(series, values, 0), 0);
    }

    {
        std::vector<int64_t> series = {2, 3};
        std::vector<int>     values = {20, 30};

        EXPECT_EQ(interpolate(series, values, 0), (20 + (0 - 2) * (30 - 20) / (3 - 2)));
        EXPECT_EQ(interpolate(series, values, 1), (20 + (1 - 2) * (30 - 20) / (3 - 2)));
        EXPECT_EQ(interpolate(series, values, 2), 20);
        EXPECT_EQ(interpolate(series, values, 3), 30);
        EXPECT_EQ(interpolate(series, values, 4), (20 + (4 - 2) * (30 - 20) / (3 - 2)));
    }

    {
        std::vector<int64_t> series = {1, 2, 3, 4, 5};
        std::vector<int>     values = {10, 20, 30, 40, 50};

        EXPECT_EQ(interpolate(series, values, 1), 10);
        EXPECT_EQ(interpolate(series, values, 3), 30);
        EXPECT_EQ(interpolate(series, values, 5), 50);
        EXPECT_EQ(interpolate(series, values, 0), 0);
        EXPECT_EQ(interpolate(series, values, 10), (40 + (10 - 4) * (50 - 40) / (5 - 4)));
    }

    {
        std::vector<int64_t> series = {10, 20, 30, 40, 50, 60};
        std::vector<int>     values = {100, 200, 300, 400, 500, 600};

        EXPECT_EQ(interpolate(series, values, 1), (100 + (1 - 10) * (200 - 100) / (20 - 10)));
        EXPECT_EQ(interpolate(series, values, 5), (100 + (5 - 10) * (200 - 100) / (20 - 10)));
        EXPECT_EQ(interpolate(series, values, 15), (100 + (15 - 10) * (200 - 100) / (20 - 10)));
        EXPECT_EQ(interpolate(series, values, 25), (200 + (25 - 20) * (300 - 200) / (30 - 20)));
        EXPECT_EQ(interpolate(series, values, 42), (400 + (42 - 40) * (500 - 400) / (50 - 40)));
        EXPECT_EQ(interpolate(series, values, 90), (600 + (90 - 60) * (600 - 500) / (60 - 50)));
    }
}

TEST(algorithm, find_min_packet_length)
{
    {
        std::vector<uint32_t> messages = {4, 3, 10, 3, 2, 5};
        EXPECT_EQ(find_min_packet_length(messages, 1), 27);
        EXPECT_EQ(find_min_packet_length(messages, 2), 17);
        EXPECT_EQ(find_min_packet_length(messages, 3), 10);
        EXPECT_EQ(find_min_packet_length(messages, 4), 10);
        EXPECT_EQ(find_min_packet_length(messages, 6), 10);
    }

    {
        std::vector<uint32_t> messages = {6, 5, 4, 3, 3, 2, 5};
        EXPECT_EQ(find_min_packet_length(messages, 0), 0);
        EXPECT_EQ(find_min_packet_length(messages, 1), 28);
        EXPECT_EQ(find_min_packet_length(messages, 2), 15);
        EXPECT_EQ(find_min_packet_length(messages, 3), 11);
        EXPECT_EQ(find_min_packet_length(messages, 4), 9);
        EXPECT_EQ(find_min_packet_length(messages, 5), 7);
        EXPECT_EQ(find_min_packet_length(messages, 6), 6);
        EXPECT_EQ(find_min_packet_length(messages, 7), 6);
        EXPECT_EQ(find_min_packet_length(messages, 8), 6);
    }
}
