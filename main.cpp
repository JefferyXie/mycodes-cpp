#include <gtest/gtest.h>
#include "header.h"
#include "./test/Test_language.h"
#include "./test/Test_algorithm.h"
#include "./test/Test_multithreading.h"

//
// googletest articles:
// http://stackoverflow.com/questions/13513905/how-to-setup-googletest-as-a-shared-library-on-linux
// http://www.ibm.com/developerworks/aix/library/au-googletestingframework.html
// http://www.yolinux.com/TUTORIALS/Cpp-GoogleTest.html
// https://github.com/google/googletest/blob/master/googletest/docs/FAQ.md#why-is-it-not-recommended-to-install-a-pre-compiled-copy-of-google-test-for-example-into-usrlocal


bool LarryArray(vector<int> input) {
    auto ordered = [](int a, int b, int c) {
        int bi = max(a,max(b,c));
        int sm = min(a,min(b,c));
        int mi = a+b+c-bi-sm;
        if ((sm == a && mi == c && bi == b) ||
            (sm == b && mi == a && bi == c) ||
            (sm == c && mi == b && bi == a))
            return false;
        return true;
    };
    if (input.size() == 3) return ordered(input[0], input[1], input[2]);
    for (int i = 0; i < (int)input.size()-3; ++i) {
        int A = input[i];
        int B = input[i+1];
        int C = input[i+2];
        int D = input[i+3];
        if (A < B && A < C && A < D) {
            if (!ordered(B,C,D))
                return false;
        }
        if (D > A && D > B && D > C) {
            if (!ordered(A,B,C))
                return false;
        }
    }
    return true;
}

int main(int argc, char **argv) {
    vector<int> v1 = {1,2,3,4};
    cout << (LarryArray(v1) ? "TRUE" : "FALSE") << endl;
    vector<int> v2 = {3,1,2};
    cout << (LarryArray(v2) ? "TRUE" : "FALSE") << endl;
    vector<int> v3 = {1,3,4,2};
    cout << (LarryArray(v3) ? "TRUE" : "FALSE") << endl;
    vector<int> v4 = {1,2,3,5,4};
    cout << (LarryArray(v4) ? "TRUE" : "FALSE") << endl;
    vector<int> v5 = {9,6,8,12,3,7,1,11,10,2,5,4}; // NO
    cout << (LarryArray(v5) ? "TRUE" : "FALSE") << endl;
    vector<int> v6 = {9,6,8,12,3,7,1,11,10,2,5,4}; // YES
    cout << (LarryArray(v6) ? "TRUE" : "FALSE") << endl;
    vector<int> v7 = {5,8,13,3,10,4,12,1,2,7,14,6,15,11,9}; // NO
    cout << (LarryArray(v7) ? "TRUE" : "FALSE") << endl;
    vector<int> v8 = {8,10,6,11,7,1,9,12,3,5,13,4,2}; // YES
    cout << (LarryArray(v8) ? "TRUE" : "FALSE") << endl;
    vector<int> v9 = {7,9,15,8,10,16,6,14,5,13,17,12,3,11,4,1,18,2}; // NO
    cout << (LarryArray(v9) ? "TRUE" : "FALSE") << endl;
    return 0;
    /*
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();*/
}


