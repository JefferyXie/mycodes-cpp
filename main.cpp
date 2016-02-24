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

int main(int argc, char **argv) {
    string tc2 = JudgeBiTree("(A,B) (A,C) (B,D) (D,C)");
 
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
/*
    vector<int> vec = {1,2,3,4,5,6};
    auto it = end(vec);
    while (it-- != begin(vec)) {
        auto newIt = vec.erase(it);
        cout << "[" << vec.size() << "] " << *it << ", " << *newIt << endl;
    }

    return 0;*/
}


