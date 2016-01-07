#include "../header.h"

//
// http://www.zhihu.com/question/29511959
// http://www.tuicool.com/articles/F7Fjuu3
// http://en.cppreference.com/w/cpp/language/copy_elision
//
struct Test {
    Test() {
        std::cout << "construct a Test object" << std::endl;
    }
    Test(const Test&) {
        std::cout << "copy construct a Test object" << std::endl;
    }
    Test& operator=(const Test&) {
        std::cout << "copy assignment a Test object" << std::endl;
        return *this;
    }
    Test(Test&&) {
        std::cout << "move construct a Test object" << std::endl;
    }
    Test& operator=(Test &&t) {
        std::cout << "move assignment a Test object" << std::endl;
        return *this;
    }
    ~Test() {
        std::cout << "destruct a Test object" << std::endl;
    }
};

Test getTest() {
    return Test();
}

Test getTestWithName() {
    Test temp;
    return temp;
}

vector<Test> getVector() {
    // copy elision from tempory object to 'temp', omit copy and move constructor
    vector<Test> temp = vector<Test>(1); 

    cout << "message after vector is created." << endl;

    return temp; // NRVO temp to returned 
}


