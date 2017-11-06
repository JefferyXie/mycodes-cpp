#ifndef UNITTEST_H
#define UNITTEST_H

#include "./include.h"
#include "./SearchWorker.h"

#define UNITTEST_RUN(functionality) \
    UNITTEST_##functionality();
#define UNITTEST(functionality) \
    void UNITTEST_##functionality() 
#ifdef _MSC_VER // for visual studio
#define EQUAL(target, expected) \
    if (target != expected) { \
        cout << "FAILED: " << __FUNCTION__ << "\n\t"; \
        cout << "[TARGET]: " << target << "\n\t"; \
        cout << "[EXPECTED]: " << expected << endl; \
    } else { \
        cout << "PASSED: " << __FUNCTION__ << endl; \
    }
#else
#define EQUAL(target, expected) \
    if (target != expected) { \
        cout << "\033[31mFAILED: \033[0m" << __PRETTY_FUNCTION__ << "\n\t"; \
        cout << "\033[31m[TARGET]: \033[0m" << target << "\n\t"; \
        cout << "\033[31m[EXPECTED]: \033[0m" << expected << endl; \
    } else { \
        cout << "\033[32mPASSED: \033[0m" << __PRETTY_FUNCTION__ << endl; \
    }
#endif

UNITTEST(proximitysearch) {
    cout << "[Unit test starts...]" << endl;
    SearchWorker o;
    int c1 = o.RunSearch("tc1.txt", "the", "canal", 6);
    EQUAL(c1, 3);

    int c2 = o.RunSearch("tc1.txt", "the", "canal", 3);
    EQUAL(c2, 1);

    int c3 = o.RunSearch("tc2.txt", "the", "canal", 6);
    EQUAL(c3, 5);

    int c4 = o.RunSearch("tc2.txt", "the", "canal", 1000);
    EQUAL(c4, 48);

    int c5 = o.RunSearch("tc3.txt", "buffalo", "buffalo", 20);
    EQUAL(c5, 52);

    int c6 = o.RunSearch("tc3.txt", "buffalo", "had", 5);
    EQUAL(c6, 0);

    int c7 = o.RunSearch("tc3.txt", "buffalo", "had", 100);
    EQUAL(c7, 462);
    cout << "[Unit test ends.]" << endl;
}

UNITTEST(otherfunction) {
}

#endif


