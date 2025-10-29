#include <gtest/gtest.h>
#include "../language/auto.h"
#include "../language/circular_buf.h"
#include "../language/circular_buf_async.h"
#include "../language/classtest.h"
#include "../language/copy_elision.h"
#include "../language/matrix.h"
#include "../language/memoryleakdetector.h"
#include "../language/new_delete.h"
#include "../language/template_constraints.h"
#include "../language/thread.h"

TEST(DISABLED_language, constructorOrder)
{
    // always call base constructor no matter how the object is created
    A* a = new A();
    B* b = new B();
    A* c = new B();
    // since c is A* and destructor is not virtual, only base destructor is called!
    // ~B() won't be called here!
    delete c;
    delete b;
    delete a;
}

TEST(DISABLED_language, auto)
{
    int        i   = 5;
    const int  ci  = 0;
    const int* pci = &i;

    auto& b1 = i;       // b1 is of type int&
    auto& b2 = ci;      // b2 is of type const int&
    auto& b3 = *pci;    // b3 is also of type const int&
    auto& b4 = pci;     // b4 is of type const int&, too

    int& r = i;

    auto a1 = i;
    auto a2 = r;
    auto a3 = bar();

    A_auto<decltype(i)>::foo();
    A_auto<decltype(r)>::foo();
    A_auto<decltype(a1)>::foo();
    A_auto<decltype(a2)>::foo();
    A_auto<decltype(bar())>::foo();
    A_auto<decltype(a3)>::foo();

    A_auto<decltype(i)>::goo(i);
    A_auto<decltype(r)>::goo(r);
    A_auto<decltype(a1)>::goo(a1);
    A_auto<decltype(a2)>::goo(a2);
    A_auto<decltype(bar())>::goo(bar());
    A_auto<decltype(a3)>::goo(a3);
}

TEST(DISABLED_language, thread)
{
    Recurisve_Mutex oo;
    oo.run();
}

TEST(DISABLED_language, copy_elision)
{
    std::cout << "=============RVO==============" << std::endl;
    std::cout << "++Plain obj rvo for copy construct" << std::endl;
    auto obj1 = getPlain();

    std::cout << "--------------" << std::endl;
    std::cout << "++Plain obj rvo for move construct" << std::endl;
    auto obj111 = std::move(getPlain());

    std::cout << "--------------" << std::endl;
    std::cout << "++Plain obj rvo for copy assignment" << std::endl;
    Plain obj11;
    obj11 = getPlain();

    std::cout << "--------------" << std::endl;
    std::cout << "++Plain object rvo for move assignment" << std::endl;
    Plain obj1111;
    obj1111 = std::move(getPlain());

    std::cout << "=============NRVO==============" << std::endl;
    std::cout << "++Plain obj nrvo for copy construct" << std::endl;
    auto obj2 = getPlainWithName();

    std::cout << "--------------" << std::endl;
    std::cout << "++Plain obj nrvo for move construct" << std::endl;
    auto obj222 = std::move(getPlainWithName());

    std::cout << "--------------" << std::endl;
    std::cout << "++Plain obj nrvo for copy assignment" << std::endl;
    Plain obj22;
    obj22 = getPlainWithName();

    std::cout << "--------------" << std::endl;
    std::cout << "++Plain obj nrvo for move assignment" << std::endl;
    Plain obj2222;
    obj2222 = std::move(getPlainWithName());
    std::cout << "==============================" << std::endl;
}

TEST(DISABLED_language, matrix)
{
    Matrix<int> m(2, 3);
    m(0, 0) = 1;
    m(1, 1) = 3;

    std::cout << m[0][0] << "," << m[1][1] << std::endl;
}

TEST(DISABLED_language, lib)
{
    /*
        SharedClass shObj;
        shObj.Fo();
        shObj.Process("first string", "second input string");

        A& aObj = shObj;
        aObj.Fo();
        aObj.Go();
    */
}

TEST(DISABLED_language, lambda)
{
    A a;
    A aa = A();

    auto lam = [](A* a) {
        a->Go();
    };
    auto lam1 = [](A* a) {
        auto temp = static_cast<B*>(a);
        if (nullptr == temp)
            a->Go();
        else
            temp->Go();
    };
    std::vector<int> v = {
        2,
        3,
    };
    v.push_back(10);
    list<std::string> ll;
    ll.push_back("1st ele");
    ll.push_back("2nd ele");
    ll.push_back("3rd ele");
    // A a = A();
    // a.foo();
    B b;
    b.Calc(4, 5);
    lam(&b);
    lam1(&b);
    lam1(&a);
}

TEST(DISABLED_language, new_delete)
{
#if CUSTOM_NEW_ENABLED > 0
    int* a = new int;
    delete a;
    int* b = new int(10);
    delete b;
    int* c = new int[10];
    delete[] c;
    char* ch = new char[10];
    delete[] ch;
    char* cc = new ('m') char[10];
    delete[] cc;

    auto p1 = new OpNewDelete();
    delete p1;
    auto p2 = new OpNewDelete[2];
    delete[] p2;
#endif
}

TEST(language, memoryleakdetector)
{
#if MEMORYLEAK_ENABLED > 0
    int* p = NEW(int);
    *p     = 10;

    Plain* pl = NEW(Plain);

    DumpMemUsage();

    DEL(pl);

    DEL(p);
#endif
}

TEST(language, circular_buf)
{
    circular_buf cbuf;
    cbuf.write("abcde", 5);
    cbuf.write("fghijk", 6);
    int n = 5;
    while (n--) {
        cbuf.read();
    }
    cbuf.write("lmnopqrst", 9);
    while (n++ < 2) {
        cbuf.read();
    }
    cbuf.write("uvwxyz", 6);
    string s;
    while (1) {
        char ch = cbuf.read();
        std::cout << ch;
        if (ch == '\0') {
            std::cout << std::endl;
            break;
        } else {
            s += ch;
        }
    }

    EXPECT_EQ(s, string("ijklmnopqrst"));

    circular_buf cbuf1;
    auto         writer = [&cbuf1]() {
        const char* letters = "abcdefghijklmnopqrstuvwxyz";
        int         count   = 0;
        int         i       = 0;
        string      ss;
        while (i < strlen(letters)) {
            int ret = cbuf1.write(letters + i, 1);
            if (ret <= 0) {
                std::cout << "*" << std::this_thread::get_id() << "**[" << ++count
                          << "] failed to write: " << *(letters + i) << std::endl;
            } else {
                ss += *(letters + i);
                ++i;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::cout << "*writer*" << std::this_thread::get_id() << ": " << ss << std::endl;
    };
    bool stopped = false;
    auto reader  = [&cbuf1, &stopped]() {
        int    count = 0;
        string ss;
        while (1) {
            char ch = cbuf1.read();
            if (ch == '\0') {
                std::cout << "*" << std::this_thread::get_id() << "**[" << ++count << "] read nothing." << std::endl;
            } else {
                ss += ch;
                // this judgement doesn't work well for mutiple writer (multiple 'z')
                if (ch == 'z')
                    stopped = true;
            }
            if (stopped)
                break;
            std::this_thread::sleep_for(std::chrono::milliseconds(80));
        }
        std::cout << "*reader*" << std::this_thread::get_id() << ": " << ss << std::endl;
    };
    std::thread th_w1(writer);
    std::thread th_w2(writer);
    std::thread th_r1(reader);
    std::thread th_r2(reader);
    th_w1.join();
    th_w2.join();
    th_r1.join();
    th_r2.join();
}

// TODO: compile error complains boost in linux
TEST(DISABLED_language, flagset)
{
    /*
    assert((int)Options::FULLSCREEN == 0);
    test_AND();
    test_OR();
    test_set_reset();
    */
}

