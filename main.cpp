//#include <gtest/gtest.h>
//#include "header.h"
//#include "./test/Test_language.h"
//#include "./test/Test_algorithm.h"
//#include "./test/Test_multithreading.h"

//
// googletest articles:
// http://stackoverflow.com/questions/13513905/how-to-setup-googletest-as-a-shared-library-on-linux
// http://www.ibm.com/developerworks/aix/library/au-googletestingframework.html
// http://www.yolinux.com/TUTORIALS/Cpp-GoogleTest.html
// https://github.com/google/googletest/blob/master/googletest/docs/FAQ.md#why-is-it-not-recommended-to-install-a-pre-compiled-copy-of-google-test-for-example-into-usrlocal

#include "./include.h"
#include "./Order.h"
#include "./OrderHelper.h"

int main(int argc, char** argv) {
    if (argc < 2) return -1;

    OrderHandler handler(argv[1]);
    if (!handler.Parse()) return -1;

    string result = handler.AnswerQ1() + ", " +
                    handler.AnswerQ2() + ", " +
                    handler.AnswerQ3() + ", " +
                    handler.AnswerQ4() + ", " +
                    handler.AnswerQ5();
    cout << result << endl;

    /*
    for_each(begin(entryContainer), end(entryContainer), [](OrderEntry& o) {
        cout << o.ToString() << endl;
    });
    for_each(begin(ackContainer), end(ackContainer), [](OrderAck& o) {
        cout << o.ToString() << endl;
    });
    for_each(begin(fillContainer), end(fillContainer), [](OrderFill& o) {
        cout << o.ToString() << endl;
    });*/

    return 0;
}

/*
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
*/

