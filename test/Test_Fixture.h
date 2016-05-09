#include <gtest/gtest.h>
#include "../main/header.h"

// It is typical to do some custom initialization work before executing a unit test. 
// For example, if you are trying to measure the time/memory footprint of a test, you 
// need to put some test-specific code in place to measure those values. This is where 
// fixtures come in—they help you set up such custom testing needs.
class MyTestFixture : public ::testing::Test {
public:
    MyTestFixture() { cout << "MyTestFixture()" << endl; }
    ~MyTestFixture() { cout << "~MyTestFixture()" << endl; }

    void SetUp() {
        // code here will execute just before the test ensues 
        cout << "SetUp()" << endl;
    }
    void TearDown() {
        // code here will be called just after the test completes
        cout << "TearDown()" << endl;
    }
    // put in any custom data members that you need 
};

// The same test fixture is not used across multiple tests. For every new unit test, 
// the framework creates a new test fixture. So in below codes, the SetUp routine is
// called twice because two myFixture1 objects are created.

TEST_F(MyTestFixture, algorithm) {
    cout << "TEST_F(MyTestFixture, algorithm)" << endl;
}

TEST_F(MyTestFixture, langurage) {
    cout << "TEST_F(MyTestFixture, langurage)" << endl;
}



