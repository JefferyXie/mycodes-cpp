#include <gtest/gtest.h>
#include "../main/header.h"
#include "../algorithm/find_common_ancestor.h"
#include "../algorithm/traversetree.h"
#include "../algorithm/num_days_between.h"
#include "../algorithm/similarpair.h"
#include "../algorithm/sort_k_diff_pairs.h"
#include "../algorithm/connected_cell_in_grid.h"
#include "../algorithm/subarray_max_mod.h"
#include "../algorithm/find_missing_numbers.h"
#include "../algorithm/bike_racer.h"
#include "../algorithm/count_luck.h"
#include "../algorithm/cuttree.h"
#include "../algorithm/cuttree_2.h"
#include "../algorithm/gridsearch.h"
#include "../algorithm/matrix_rotation.h"
#include "../algorithm/deque_maxk.h"
#include "../algorithm/encryption.h"
#include "../algorithm/remove_duplicates.h"
#include "../algorithm/my_list.h"
#include "../algorithm/my_queue.h"
#include "../algorithm/my_smartpointer.h"
#include "../algorithm/array_bus_station.h"
#include "../algorithm/max_points_on_line.h"
#include "../interview/optiver/judgebitree.h"
#include "../language/classtest.h"

// ASSERT_XXX(): If assertion fails then processing of test terminate.
// EXPECT_XXX(): nonfatal failure, allowing processing to continue.

TEST(algorithm, FindCommonAncestor) {
    shared_ptr<TreeNode<int> > root(new TreeNode<int>(10));
    shared_ptr<TreeNode<int> > n1(new TreeNode<int>(2));
    shared_ptr<TreeNode<int> > n2(new TreeNode<int>(4));
    shared_ptr<TreeNode<int> > n3(new TreeNode<int>(8));
    shared_ptr<TreeNode<int> > n4(new TreeNode<int>(1));
    shared_ptr<TreeNode<int> > n5(new TreeNode<int>(12));
    shared_ptr<TreeNode<int> > n6(new TreeNode<int>(111));
    shared_ptr<TreeNode<int> > n7(new TreeNode<int>(43));
    shared_ptr<TreeNode<int> > n8(new TreeNode<int>(40));
    shared_ptr<TreeNode<int> > n9(new TreeNode<int>(14));
    shared_ptr<TreeNode<int> > n0(new TreeNode<int>(11));
    
    root->left = n1.get();
    root->right = n2.get();
    n1->left = n3.get();
    n1->right = n4.get();
    n2->left = n5.get();
    n2->right = n6.get();
    n3->left = n7.get();
    n3->right = n8.get();
    n4->right = n9.get();
    n9->left = n0.get();
    
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

TEST(algorithm, traverse) {
    tNode root(0);
    tNode n1(1);
    tNode n2(2);
    tNode n3(3);
    tNode n4(4);
    tNode n5(5);
    tNode n6(6);
    tNode n7(7);
    tNode n8(8);
    tNode n9(9);
    tNode n10(10);
    tNode n11(11);
    root.left = &n1;
    root.right = &n2;
    n1.left = &n3;
    n1.right = &n4;
    n2.left = &n5;
    n2.right = &n6;
    n3.right = &n7;
    n4.left = &n8;
    n4.right = &n9;
    n6.left = &n10;
    n8.left = &n11;

    TraverseInorder(&root);
    TraversePostorder(&root);
}

TEST(algorithm, judgeBiTree) {
    string tc1 = JudgeBiTree("(B,D) (D,E) (A,B) (C,F) (E,G) (A,C)");
    EXPECT_EQ(tc1, "(A(B(D(E(G))))(C(F)))");

    string tc2 = JudgeBiTree("(A,B) (A,C) (B,D) (D,C)");
    EXPECT_EQ(tc2, "E5");

    string tc3 = JudgeBiTree("(A,B) (A,C) (B,D) (E,F) (F,G) (F,H)");
    EXPECT_EQ(tc3, "E4");
    
    string tc4 = JudgeBiTree("(A,B) (B,D) (B,C) (C,A)");
    EXPECT_EQ(tc4, "E5");

    string tc5 = JudgeBiTree("(A,B) (B,C) (A,B) (A,C)");
    EXPECT_EQ(tc5, "E2");

    string tc6 = JudgeBiTree("(A,B) (A,C) (A,D) (B,E)");
    EXPECT_EQ(tc6, "E3");
    
    string tc7 = JudgeBiTree("(U,P) (O,L) (A,S) (N,O) (S,T) (U,M) (A,N) (S,U) (L,D)");
    EXPECT_EQ(tc7, "(A(N(O(L(D))))(S(T)(U(M)(P))))");

    string tc8 = JudgeBiTree("(A,B) (B,C) (A,B)");
    EXPECT_EQ(tc8, "E2");

    string tc9 = JudgeBiTree("(A,C) (A,B) (B,Q) (B,P) (C,D) (Q,Z) (Q,X)");
    EXPECT_EQ(tc9, "(A(B(P)(Q(X)(Z)))(C(D)))");
    
    string tc10 = JudgeBiTree("(A,B)   (A,C) (B, E) (B,F)");
    EXPECT_EQ(tc10, "E1");

    string tc11 = JudgeBiTree("(P,J) (X,T) (R,E) (P,C) (X,B) (R,X)");
    EXPECT_EQ(tc11, "E4");

    string tc12 = JudgeBiTree("(X,Y) (R,D) (D,P) (R,A) (X,Z) (Z,T) (Z,B) (R,G)");
    EXPECT_EQ(tc12, "E3");

    string tc13 = JudgeBiTree("(R,N) (U,T) (C,H) (Y,B) (R,A) (Y,C) (K,Y) (B,R) (H,E) (H,S) (K,U) (T,M)");
    EXPECT_EQ(tc13, "(K(U(T(M)))(Y(B(R(A)(N)))(C(H(E)(S)))))");

    string tc14 = JudgeBiTree("(K,M) (P,L) (L,S) (Y,A) (T,P) (T,K) (L,E) (K,Y)");
    EXPECT_EQ(tc14, "(T(K(M)(Y(A)))(P(L(E)(S))))");

    string tc15 = JudgeBiTree("(A,B) (A,C (B,D) B,E)");
    EXPECT_EQ(tc15, "E1");

    string tc16 = JudgeBiTree("   (P,B) (E,X) (P,D) (E,Y) (P,A)");
    EXPECT_EQ(tc16, "E1");
}

TEST(algorithm, numdaysbetween) {
    int a = num_days_between(2010,5,1,2011,5,1);
    EXPECT_EQ(a, 365);
    int b = num_days_between(1900,1,10,1901,3,1);
    EXPECT_EQ(b, 415);
    int c = num_days_between(1999,10,1,2015,12,10);
    EXPECT_EQ(c, 5914);
    int d = num_days_between(1644,3,21,2016,2,23);
    EXPECT_EQ(d, 135843);
    int e = num_days_between(2016,3,21,2016,3,30);
    EXPECT_EQ(e, 9);
    int f = num_days_between(1644,3,21,1644,3,21);
    EXPECT_EQ(f, 0);
    cout << a << ", " << b << ", " << c << ", " 
        << d << ", " << e << ", " << f << endl;
}

TEST(DISABLED_algorithm, similarpair) {
    int arr[][2] = {
        {1,7},
        {1,10},
        {10,15},
        {15,4},
        {15,16},
        {16,18},
        {16,17},
        {17,2},
        {10,5},
        {5,3},
        {3,19},
        {1,9},
        {9,11},
        {11,13},
        {11,12},
        {12,8},
        {12,20},
        {11,14},
        {1,6},
    };
    int a = SimilarPair(5, arr);
    EXPECT_EQ(a, 23);
}

TEST(algorithm, kdiffparis) {
    vector<int> arr = { 1,5,3,4,2 };
    int a = sort_k_diff_pairs(arr, 2);
    EXPECT_EQ(a, 3);
}

TEST(algorithm, maxconnectedcells) {
    vector<vector<int> > mt1 = {
        {0,1,1},
        {1,0,0},
        {0,1,0},
    };
    int a = max_connected_cell_in_grid(mt1);
    EXPECT_EQ(a, 4);
    vector<vector<int> > mt2 = {
        {0,1,1,1,1},
        {1,0,0,0,1},
        {1,1,0,1,0},
        {0,1,0,1,1},
        {0,1,1,1,0},
    };
    int b = max_connected_cell_in_grid(mt2);
    EXPECT_EQ(b, 15);
}

TEST(algorithm, subarray_max_mod) {
    long long arr[] = {3,3,9,9,5};
    auto a = subarray_max_mod(arr, sizeof(arr)/sizeof(arr[0]), 7);
    EXPECT_EQ(a, 6);
    long long arr1[] = {
        846930887, 1681692778, 1714636916, 1957747794, 424238336, 719885387, 1649760493, 
        596516650, 1189641422, 1025202363, 1350490028, 783368691, 1102520060, 2044897764, 
        1967513927, 1365180541, 1540383427, 304089173, 1303455737, 35005212, 521595369, 
        294702568, 1726956430, 336465783, 861021531, 278722863, 233665124, 2145174068, 
        468703136, 1101513930, 1801979803, 1315634023, 635723059, 1369133070, 1125898168,
        1059961394, 2089018457, 628175012, 1656478043, 1131176230, 1653377374, 859484422,
        1914544920, 608413785, 756898538, 1734575199, 1973594325, 149798316, 2038664371,
        1129566414 };
    auto b = subarray_max_mod(arr1, sizeof(arr)/sizeof(arr[0]), 1804289384);
    EXPECT_EQ(b, 1745498720);
}

TEST(algorithm, missingnumbers) {
    int A[] = {203, 204, 205, 206, 207, 208, 203, 204, 205, 206};
    int B[] = {203, 204, 204, 205, 206, 207, 205, 208, 203, 206, 205, 206, 204};
    auto s = find_missing_numbers(A, sizeof(A)/sizeof(A[0]), B, sizeof(B)/sizeof(B[0]));
    set<int> ss{204,205,206};
    EXPECT_EQ(s, ss);
}

TEST(DISABLED_algorithm, bikeracer) {
    vector<Coordinate> bikers1 = {
        {0,1},
        {0,2},
        {0,3},
    };
    vector<Coordinate> bikes1 = {
        {100,1},
        {200,2},
        {300,3},
    };
    int K1 = 2;
    auto shortestTime1 = BikeRacerShortestTime(bikers1, bikes1, K1);
    EXPECT_EQ(shortestTime1, 40000);

    vector<Coordinate> bikers2 = {
        {286, 686},
        {439, 775},
        {34, 471},
        {116, 512},
        {206, 632},
        {549, 365},
        {961, 702},
        {999, 514},
        {81, 249},
        {538, 268},
    };
    vector<Coordinate> bikes2 = {
        {261, 789},
        {929, 217},
        {909, 187},
        {729, 411},
        {309, 147},
        {185, 184},
        {960, 947},
        {770, 624},
        {136, 346},
        {979, 238},
    };
    /*
    // the algorithm needs to be optimized so that this case won't time out
    int K2 = 5;
    auto shortestTime2 = BikeRacerShortestTime(bikers2, bikes2, K2);
    EXPECT_EQ(shortestTime2, 42565);*/

    vector<Coordinate> bikers3 = {
        {411, 353},
        {162, 727},
        {6, 293},
        {370, 16},
        {334, 610},
        {201, 999},
        {354, 272},
        {475, 73},
        {474, 356},
        {651, 607},
    };
    vector<Coordinate> bikes3 = {
        {213, 835},
        {871, 716},
        {161, 707},
        {348, 439},
        {163, 847},
        {200, 67},
        {146, 926},
        {571, 714},
        {82, 152},
        {115, 293},
    };
    int K3 = 2;
    auto shortestTime3 = BikeRacerShortestTime(bikers3, bikes3, K3);
    EXPECT_EQ(shortestTime3, 8354);

    vector<Coordinate> bikers4 = {
        {145, 862},
        {533, 105},
        {34, 192},
        {897, 656},
    };
    vector<Coordinate> bikes4 = {
        {902, 518},
        {78, 108},
        {658, 369},
        {127, 364},
    };
    int K4 = 2;
    auto shortestTime4 = BikeRacerShortestTime(bikers4, bikes4, K4);
    EXPECT_EQ(shortestTime4, 19069);
}

TEST(algorithm, permutation) {
    vector<int> source = {1,2,3,4};
    vector<int> target;
    int a = PossiblePermutation_WithOrder(source, 0, 4, target);
    EXPECT_EQ(a, 24);
}

TEST(algorithm, countluck) {
    vector<vector<char> > forest;
    auto fillforest = [&forest](vector<string>& ar) {
        forest.clear();
        for_each(ar.begin(), ar.end(), [&forest](string s) {
            vector<char> v;
            for_each(s.begin(), s.end(), [&v](char c) {
                v.push_back(c);
            });
            forest.push_back(v);
        });
    };
    vector<string> arr1 = {
        {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."},
        {"M........................................"},
        {"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX."},
        {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."},
        {"........................................."},
        {".XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"},
        {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."},
        {"........................................."},
        {"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX."},
        {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."},
        {"........................................."},
        {".XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"},
        {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."},
        {"........................................."},
        {"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX."},
        {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."},
        {"........................................."},
        {".XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"},
        {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."},
        {"........................................."},
        {"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX."},
        {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."},
        {"........................................."},
        {".XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"},
        {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."},
        {"........................................."},
        {"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX."},
        {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."},
        {"........................................."},
        {".XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"},
        {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."},
        {"........................................."},
        {"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX."},
        {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."},
        {"........................................."},
        {".XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"},
        {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."},
        {"........................................."},
        {"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX."},
        {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."},
        {".*......................................."},
    }; // 280
    fillforest(arr1);
    int a1 = count_luck(forest);
    EXPECT_EQ(a1, 280);

    vector<string> arr2 = {
        {"XXXXXXXXXXXXXXXXX"},
        {"XXX.XX.XXXXXXXXXX"},
        {"XX.*..M.XXXXXXXXX"},
        {"XXX.XX.XXXXXXXXXX"},
        {"XXXXXXXXXXXXXXXXX"},
    }; // 1*/
    fillforest(arr2);
    int a2 = count_luck(forest);
    EXPECT_EQ(a2, 1);

    vector<string> arr3 = {
        {".X.X......X"},
        {".X*.X.XXX.X"},
        {".XX.X.XM..."},
        {"......XXXX."},
    }; // 3
    fillforest(arr3);
    int a3 = count_luck(forest);
    EXPECT_EQ(a3, 3);

    vector<string> arr4 = {
        {"..........*"},
        {".XXXXXXXXXX"},
        {"..........."},
        {"XXXXXXXXXX."},
        {"M.........."},
    }; // 0
    fillforest(arr4);
    int a4 = count_luck(forest);
    EXPECT_EQ(a4, 0);

    vector<string> arr5 = {
        {"X.XXXX.XX....X.XX...X.XXXXXXXXX"},
        {"X.XXX...XXX.X..XXX.XX..XXXXXXXX"},
        {"...X.XX..X...X..XX.X..XXXXXXXXX"},
        {"X.X..X..X.X.X..X.....XX.XXXXXXX"},
        {"...X..X.X....X.X.X.X.X..XXXXXXX"},
        {".X..X....X.X.....XX....XXXXXXXX"},
        {"..X..XX.X.X..XX.X..XX.XXXXXXXXX"},
        {".XXX.X.....X.X.X*.X.XX.XXXXXXXX"},
        {"X..X..X.X.X.....X....X..XXXXXXX"},
        {"..X.X....X..XXXX..XXX..XXXXXXXX"},
        {"X....XXX..X.....X...X.XXXXXXXXX"},
        {"..XX.....X.X.X.X..X.X..XXXXXXXX"},
        {"XX.X.X.X...X.XX.X..X..X..XXXXXX"},
        {".M...XXXX.X.....X.X.X...XXXXXXX"},
        {"X.XXX..X...X.X.X..X..X.XXXXXXXX"},
        {".XX...X.XX..X..X.X.X....XXXXXXX"},
        {"....X......X..X......XXX.XXXXXX"},
        {"X.X.XX.XXX..X.X.XX.XX.....XXXXX"},
        {"X..X..X....XX.....X...X.X..XXXX"},
        {"..X.X...XX....X.XX..X.X..XX.XXX"},
        {".X..X.X.X.X.XX...X.X.X.XX...XXX"},
        {"XX.X...X....X..X........XX.X.XX"},
        {".....XX..X.X.XX..XX.X.X......XX"},
        {"X.X.XX..X.XX....X..XXXXX.XXXX.X"},
        {"XX..X.XX....XXX...X.X.X........"},
        {".XXXX...XXX.....X......XX.XX.XX"},
        {"......XX....X.X..XX.XX...XX.XXX"},
        {"X.X.X....X.X...X...X...X.....X."},
        {"XX...X.X..X..X.XXXX.XX.X.X.X..."},
        {"XXXX..X..XXX.X......X.X...X..XX"},
        {"XXX..XXX..X..XX.X.X....XX..X..X"},
        {"X.XX..XX.X..X...X..X.X.XX.X..XX"},
        {"...XXXX..X.X..X..X..XXX...XXXXX"},
        {"XX...X..XX.XX..X..X.XX..X..XXXX"},
        {"XX.XX..XXXXX.X.X.X...X.XXXXXXXX"},
        {"X.....X.XXX..X.X..X.XXXXXXXXXXX"},
        {"..X.X......X...X.X..XXXXXXXXXXX"},
    };// 21
    fillforest(arr5);
    int a5 = count_luck(forest);
    EXPECT_EQ(a5, 21);

    vector<string> arr6 = {
        {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."},
        {"M.......................................*"},
        {".X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X.X."},
    };// 20 
    fillforest(arr6);
    int a6 = count_luck(forest);
    EXPECT_EQ(a6, 20);
}

TEST(algorithm, cuttree) {
    vector<int> vertices = {100,200,100,500,100,600};
    vector<pair<int,int> > edges = {
        {1,2},
        {2,3},
        {2,5},
        {4,5},
        {5,6},
    };
    int a = CutTree(vertices, edges);
    EXPECT_EQ(a, 400);
}

TEST(algorithm, cuttree_2) {
    vector<int> vertices = {100,200,100,500,100,600};
    vector<pair<int,int> > edges = {
        {1,2},
        {2,3},
        {2,5},
        {4,5},
        {5,6},
    };
    int a = CutTree_2(vertices, edges);
    EXPECT_EQ(a, 400);
}

TEST(algorithm, gridsearch) {
    vector<string> G = {
        "1234567890",
        "0987654321",
        "1111111111",
        "1111111111",
        "2222222222"
    };
    vector<string> P = {
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
        string s;
        unsigned int G_rows, G_cols, P_rows, P_cols;
        cin >> G_rows >> G_cols;
        getline(cin, s);
        vector<string> G(G_rows);
        for (unsigned int i = 0; i < G_rows; ++i) {
            getline(cin, G[i]);
        }
        cin >> P_rows >> P_cols;
        getline(cin, s);
        vector<string> P(P_rows);
        for (unsigned int i = 0; i < P_rows; ++i) {
            getline(cin, P[i]);
        }
        cout << (GridSearch(G, P) ? "YES" : "NO") << endl;
    }*/
}

TEST(algorithm, matrixrotation) {
    auto dumpMatrix = [](const vector<vector<int> >& m) {
        for_each(begin(m), end(m), [](const vector<int>& v) {
            for_each(begin(v), end(v), [](int a) {
                cout << a << " ";
            });
            cout << endl;
        });
    };
    int R = 1;
    vector<vector<int> > matrix = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16},
    };
    vector<vector<int> > result = {
        {2, 3, 4, 8},
        {1, 7, 11, 12},
        {5, 6, 10, 16},
        {9, 13, 14, 15},
    };
    auto r1 = matrix_rotation(matrix, R);
    dumpMatrix(r1);
    EXPECT_EQ(r1, result);

    R = 7;
    matrix = {
        {1, 2, 3, 4},
        {7, 8, 9, 10},
        {13, 14, 15, 16},
        {19, 20, 21, 22},
        {25, 26, 27, 28},
    };
    result = {
        {28, 27, 26, 25},
        {22, 9, 15, 19},
        {16, 8, 21, 13},
        {10, 14, 20, 7},
        {4, 3, 2, 1},
    };
    auto r2 = matrix_rotation(matrix, R);
    dumpMatrix(r2);
    EXPECT_EQ(r2, result);

    /*
    int M, N, R;
    cin >> M >> N >> R;
    matrix = vector<vector<int> >(M, vector<int>(N,0));
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j)
            cin >> matrix[i][j];
    }*/
}

TEST(algorithm, deque_maxk) {
    int arr1[] = {3,4,6,3,4};
    auto r1 = print_k_max_1(arr1, sizeof(arr1)/sizeof(arr1[0]), 2);
    EXPECT_EQ(r1, "4 6 6 4");
    auto r21 = print_k_max_2(arr1, sizeof(arr1)/sizeof(arr1[0]), 2);
    EXPECT_EQ(r21, "4 6 6 4");

    int arr2[] = {3,4,5,8,1,4,10};
    auto r2 = print_k_max_1(arr2, sizeof(arr2)/sizeof(arr2[0]), 4);
    EXPECT_EQ(r2, "8 8 8 10");
    auto r22 = print_k_max_2(arr2, sizeof(arr2)/sizeof(arr2[0]), 4);
    EXPECT_EQ(r22, "8 8 8 10");
}

TEST(algorithm, encryption) {
    string t1 = "ifmanwasmeanttostayonthegroundgodwouldhavegivenusroots";
    auto a1 = encryption(t1);
    EXPECT_EQ(a1, "imtgdvs fearwer mayoogo anouuio ntnnlvt wttddes aohghn sseoau");

    string t2 = "haveaniceday";
    auto a2 = encryption(t2);
    EXPECT_EQ(a2, "hae and via ecy");

    string t3 = "chillout";
    auto a3 = encryption(t3);
    EXPECT_EQ(a3, "clu hlt io");
}

TEST(algorithm, removeduplicates) {
    auto printarr = [](int* arr, int len) {
        for (int i = 0; i < len; ++i) 
            cout << *(arr+i) << " ";
        cout << endl;
    };
    int arr1[] = {1,2,3,4,5};
    int len1 = remove_duplicates_1(arr1, sizeof(arr1)/sizeof(arr1[0]));
    printarr(arr1, len1);
    EXPECT_EQ(len1, 5);

    int arr2[] = {1,2,2,3,1,5,6,0};
    int len2 = remove_duplicates_1(arr2, sizeof(arr2)/sizeof(arr2[0]));
    printarr(arr2, len2);
    EXPECT_EQ(len2, 6);

    int arr3[] = {1,2,2,2,3};
    int len3 = remove_duplicates_1(arr3, sizeof(arr3)/sizeof(arr3[0]));
    printarr(arr3, len3);
    EXPECT_EQ(len3, 3);

    int arr4[] = {1,1,1,2,2,5,3,2,1,3,2};
    int len4 = remove_duplicates_1(arr4, sizeof(arr4)/sizeof(arr4[0]));
    printarr(arr4, len4);
    EXPECT_EQ(len4, 4);
}

TEST(algorithm, my_list) {
    my_list<int> olist;
    int i_2 = 2;
    int i_3 = 3;
    int i_5 = 5;
    int i_8 = 8;
    int i_7 = 7;
    int i_0 = 0;
    int i_11 = 11;
    int i_10 = 10;
    olist.Add(i_3);
    olist.Add(i_5);
//    olist.Add(i_2);
    ListNode<int>* p = olist.Find(i_3);
    p = olist.Insert(p, i_8);
    olist.Insert(p, i_7);
    olist.Insert2Head(i_0);
    olist.Insert(p, i_11);
    olist.Insert2End(i_5);
    olist.Insert2End(i_3);
    olist.Remove(i_8);
    p = olist.Insert2Head(i_10);
    olist.Remove(i_10);

//    olist.Display();

    olist.Reverse();
    olist.Display();
}

TEST(DISABLED_algorithm, my_quque_int) {
    my_queue<int> q1, q2;
    for (int i = 0; i < 100; ++i) {
        q1.Push(i);
        cout << q1.ToString() << endl;
    }
    while (q1.Length() > 0) {
        cout << q1.Pop() << ", ";
        cout << q1.ToString() << endl;
    }
    cout << endl;

    cout << "--------------------------------------------------" << endl;

    auto writer = [&q2] {
        this_thread::sleep_for(chrono::seconds(10));
        for (int i = 0; i < 100; ++i) {
            q2.Push(i);
            cout << i << " pushed * " << q2.ToString() << endl;
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    };
    auto reader = [&q2] {
        while (1) {
            int i = q2.Pop();
            cout << i << " poped * " << q2.ToString() << endl;
            this_thread::sleep_for(chrono::milliseconds(30));
        }
    };

    thread th_writer(writer); 
    thread th_reader(reader);

    th_writer.join();
    th_reader.join();
}

TEST(DISABLED_algorithm, my_quque_class) {
    {
        my_queue<MyMember> q1;
        for (int i = 0; i < 100; ++i) {
            q1.Push(MyMember());
            cout << q1.ToString() << endl;
        }
        while (q1.Length() > 0) {
            cout << q1.Pop().ToString() << ", ";
            cout << q1.ToString() << endl;
        }
    }
    cout << endl;

    cout << "--------------------------------------------------" << endl;

    {
        bool writter_finished = false;
        my_queue<MyMember> q2;
        auto writer = [&q2, &writter_finished] {
            this_thread::sleep_for(chrono::seconds(10));
            for (int i = 0; i < 100; ++i) {
                q2.Push(MyMember());
                cout << i << " pushed * " << q2.ToString() << endl;
                this_thread::sleep_for(chrono::milliseconds(100));
            }
            writter_finished = true;
        };
        auto reader = [&q2, &writter_finished] {
            while (!writter_finished || q2.Length() > 0) {
                auto i = q2.Pop().ToString();
                cout << i << " poped * " << q2.ToString() << endl;
                this_thread::sleep_for(chrono::milliseconds(30));
            }
        };

        thread th_writer(writer); 
        thread th_reader(reader);

        th_writer.join();
        th_reader.join();
    }
    cout << "test end!" << endl;
}

TEST(DISABLED_algorithm, my_smartpointer) {
    my_smartpointer<int> s1(new int(2));
    my_smartpointer<int> s2(s1);
    my_smartpointer<int> s3 = s1;

    my_smartpointer<int> s4(new int(5));
    s4 = s1;

    my_smartpointer<int> s5;
    s5 = my_smartpointer<int>(new int(9));

    my_smartpointer<Plain> s11(new Plain());
    my_smartpointer<Plain> s22(s11);
    my_smartpointer<Plain> s33 = s11;

    my_smartpointer<Plain> s44(new Plain());
    s44 = s11;

    my_smartpointer<Plain> s55;
    s55 = my_smartpointer<Plain>(new Plain());

    s44.reset(new Plain());
    s44.reset(s55.get());

    s44.reset(new Plain());
    s44.reset(s55.get());
}

