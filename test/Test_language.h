#include <gtest/gtest.h>
#include "../main/header.h"
#include "../language/auto.h"
#include "../language/classtest.h"
#include "../language/speed-dynamic-cast.cpp"
#include "../language/thread.h"
#include "../language/copy_elision.h"
#include "../language/matrix.h"
#include "../language/sizeof.h"
#include "../language/template_constraints.h"
#include "../language/operatornewdelete.h"
#include "../language/memoryleakdetector.h"
#include "../language/circular_buf.h"
#include "../language/circular_buf_async.h"
#include "../language/flagset.h"

TEST(DISABLED_language, constructorOrder) {
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

TEST(DISABLED_language, auto) {
    int i = 5;
    const int ci = 0;
    const int *pci = &i;

    auto& b1 = i;       // b1 is of type int&
    auto& b2 = ci;      // b2 is of type const int&
    auto& b3 = *pci;    // b3 is also of type const int&
    auto& b4 = pci;     // b4 is of type const int&, too

    int &r = i;

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

TEST(DISABLED_language, typeinfo) {
    OneBase a;
    OneLevel1 b;
    OneLevel2 c;
    OneLevel3 d;
    //OneBase* ones[] = {&a, &b, &c, &d};

    auto& ti_a = typeid(a);
    auto& ti_b = typeid(b);
    const type_info& ti_c = typeid(c);
    const type_info& ti_d = typeid(d);

    cout << "OneBase:[" << typeid(OneBase).name() << ", " << typeid(OneBase).hash_code() 
        << "]\t a:[" << ti_a.name() << ", " << ti_a.hash_code() << "]" << endl;
    cout << "OneLevel1:[" << typeid(OneLevel1).name() << ", " << typeid(OneLevel1).hash_code() 
        << "]\t b:[" << ti_b.name() << ", " << ti_b.hash_code() << "]" << endl;
    cout << "OneLevel2:[" << typeid(OneLevel2).name() << ", " << typeid(OneLevel2).hash_code() 
        << "]\t c:[" << ti_c.name() << ", " << ti_c.hash_code() << "]" << endl;
    cout << "OneLevel3:[" << typeid(OneLevel3).name() << ", " << typeid(OneLevel3).hash_code() 
        << "]\t d:[" << ti_d.name() << ", " << ti_d.hash_code() << "]" << endl;
}

TEST(DISABLED_language, dynamic_cast) {
    std::vector< std::vector<double> > timings(17);

    for (size_t r=0 ; r<R ; ++r) {
        OneBase a;
        OneLevel1 b;
        OneLevel2 c;
        OneLevel3 d;
        OneBase* ones[] = {&a, &b, &c, &d};

        ticks start, end;
        double timed = 0.0;
        size_t res = 0;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            OneLevel3 *a = reinterpret_cast<OneLevel3*>(ones[3]);
            res += a->getType();
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[0].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            if (ones[2]->getType() == T_ONE_LEVEL2) {
                OneLevel2 *a = reinterpret_cast<OneLevel2*>(ones[2]);
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[1].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            OneBase *a = dynamic_cast<OneBase*>(ones[0]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[2].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            OneLevel1 *a = dynamic_cast<OneLevel1*>(ones[1]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[3].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            OneLevel2 *a = dynamic_cast<OneLevel2*>(ones[2]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[4].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            OneLevel3 *a = dynamic_cast<OneLevel3*>(ones[3]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[5].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            OneBase *a = dynamic_cast<OneBase*>(ones[1]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[6].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            OneBase *a = dynamic_cast<OneBase*>(ones[2]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[7].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            OneBase *a = dynamic_cast<OneBase*>(ones[3]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[8].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            OneLevel1 *a = dynamic_cast<OneLevel1*>(ones[2]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[9].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            OneLevel1 *a = dynamic_cast<OneLevel1*>(ones[3]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[10].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            OneLevel2 *a = dynamic_cast<OneLevel2*>(ones[3]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[11].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            TwoBase *a = dynamic_cast<TwoBase*>(ones[0]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[12].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            TwoBase *a = dynamic_cast<TwoBase*>(ones[1]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[13].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            TwoBase *a = dynamic_cast<TwoBase*>(ones[2]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[14].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            TwoBase *a = dynamic_cast<TwoBase*>(ones[3]);
            if (a) {
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[15].push_back(timed);
        std::cerr << res << std::endl;

        res = 0;
        start = getticks();
        for (size_t i=0 ; i<N ; ++i) {
            if (ones[3]->type == T_ONE_LEVEL3) {
                OneLevel3 *a = reinterpret_cast<OneLevel3*>(ones[3]);
                res += a->getType();
            }
        }
        end = getticks();
        timed = elapsed(end, start);
        timings[16].push_back(timed);
        std::cerr << res << std::endl;

        std::cerr << std::endl;
    }

    std::cout << "reinterpret_cast known-type: ";
    PrintStats(timings[0]);
    std::cout << std::endl;

    std::cout << "virtual function + reinterpret_cast: ";
    PrintStats(timings[1]);
    std::cout << std::endl;

    std::cout << "member variable + reinterpret_cast: ";
    PrintStats(timings[16]);
    std::cout << std::endl;

    std::cout << "dynamic_cast same-type-base success: ";
    PrintStats(timings[2]);
    std::cout << std::endl;

    std::cout << "dynamic_cast same-type-level1 success: ";
    PrintStats(timings[3]);
    std::cout << std::endl;

    std::cout << "dynamic_cast same-type-level2 success: ";
    PrintStats(timings[4]);
    std::cout << std::endl;

    std::cout << "dynamic_cast same-type-level3 success: ";
    PrintStats(timings[5]);
    std::cout << std::endl;

    std::cout << "dynamic_cast level1-to-base success: ";
    PrintStats(timings[6]);
    std::cout << std::endl;

    std::cout << "dynamic_cast level2-to-base success: ";
    PrintStats(timings[7]);
    std::cout << std::endl;

    std::cout << "dynamic_cast level3-to-base success: ";
    PrintStats(timings[8]);
    std::cout << std::endl;

    std::cout << "dynamic_cast level2-to-level1 success: ";
    PrintStats(timings[9]);
    std::cout << std::endl;

    std::cout << "dynamic_cast level3-to-level1 success: ";
    PrintStats(timings[10]);
    std::cout << std::endl;

    std::cout << "dynamic_cast level3-to-level2 success: ";
    PrintStats(timings[11]);
    std::cout << std::endl;

    std::cout << "dynamic_cast onebase-to-twobase fail: ";
    PrintStats(timings[12]);
    std::cout << std::endl;

    std::cout << "dynamic_cast onelevel1-to-twobase fail: ";
    PrintStats(timings[13]);
    std::cout << std::endl;

    std::cout << "dynamic_cast onelevel2-to-twobase fail: ";
    PrintStats(timings[14]);
    std::cout << std::endl;

    std::cout << "dynamic_cast onelevel3-to-twobase fail: ";
    PrintStats(timings[15]);
    std::cout << std::endl;

    std::cout << std::endl;
}

TEST(DISABLED_language, thread) {
    Recurisve_Mutex oo;
    oo.run();
}

TEST(DISABLED_language, copy_elision) {
    vector<Plain> vv = getVector();

    std::cout << "=============RVO==============" << std::endl; 
    std::cout << "++Plain obj rvo for copy construct" << std::endl;
    auto obj1 = getPlain();

    std::cout << "--------------" << std::endl;
    std::cout << "++Plain obj rvo for move construct" << std::endl;
    auto obj111 = std::move(getPlain());

    std::cout << "--------------" << std::endl;  
    std::cout << "++Plain obj rvo for copy assignment" << std::endl;
    Plain obj11; obj11 = getPlain();
  
    std::cout << "--------------" << std::endl;
    std::cout << "++Plain object rvo for move assignment" << std::endl;
    Plain obj1111; obj1111 = std::move(getPlain());
    

    std::cout << "=============NRVO==============" << std::endl; 
    std::cout << "++Plain obj nrvo for copy construct" << std::endl;
    auto obj2 = getPlainWithName();
    
    std::cout << "--------------" << std::endl;
    std::cout << "++Plain obj nrvo for move construct" << std::endl;
    auto obj222 = std::move(getPlainWithName());
    
    std::cout << "--------------" << std::endl;
    std::cout << "++Plain obj nrvo for copy assignment" << std::endl;
    Plain obj22; obj22 = getPlainWithName();

    std::cout << "--------------" << std::endl;
    std::cout << "++Plain obj nrvo for move assignment" << std::endl;
    Plain obj2222; obj2222 = std::move(getPlainWithName());
    std::cout << "==============================" << std::endl;
}

TEST(DISABLED_language, matrix) {
    Matrix<int> m(2,3);
    m(0,0) = 1;
    m(1,1) = 3;
    
    cout << m[0][0] << "," << m[1][1] << endl;
}

TEST(DISABLED_language, sizeof_) {
    run_CheckBasicTypes();
    run_CheckClass(stlClass("temp", 1000));

    stlClass stlObj("test", 43);
    stlClass stlObj_1 = stlObj;
    cout << "**stlObj_1**" << endl;
    //	cout << stlObj_1;
    //	cout << "stlObj_1 == stlObj: " << (stlObj==stlObj_1) << endl;

    set<stlClass> data;
    data.insert(stlClass("hello", 5));
    //	data.insert(stlClass("any", 1));
    //	data.insert(stlClass("nobody", 11));
    //	data.insert(stlClass("hahah", 0));
    //	data.insert(stlObj);
    //	data.insert(stlObj_1);

    //	copy(data.begin(), data.end(), ostream_iterator<stlClass>(cout, " * "));
}

TEST(DISABLED_language, lib) {
/*
    SharedClass shObj;
    shObj.Fo();
    shObj.Process("first string", "second input string");

    A& aObj = shObj;
    aObj.Fo();
    aObj.Go();
*/
}

TEST(DISABLED_language, lambda) {
    A a;
    A aa = A();
	
    auto lam = [](A* a) { a->Go(); };
    auto lam1 = [](A* a) { 
	auto temp = static_cast<B*>(a);
	if (nullptr == temp)
	    a->Go();
	else
	    temp->Go();
    };
    vector<int> v = { 2, 3, };
    v.push_back(10);
    list<string> ll;
    ll.push_back("1st ele");
    ll.push_back("2nd ele");
    ll.push_back("3rd ele");
    //A a = A();
    //a.foo();
    B b;
    b.Calc(4, 5);
	lam(&b);
	lam1(&b);
	lam1(&a);
}

TEST(DISABLED_language, operatornewdelete) {
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

TEST(language, memoryleakdetector) {
#if MEMORYLEAK_ENABLED > 0
    int* p = NEW(int);
    *p = 10;

    Plain* pl = NEW(Plain);

    DumpMemUsage();

    DEL(pl);

    DEL(p);
#endif
}

TEST(language, circular_buf) {
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
        cout << ch;
        if (ch == '\0') {
            cout << endl;
            break;
        } else {
            s += ch;
        }
    }

    EXPECT_EQ(s, string("ijklmnopqrst"));

    circular_buf cbuf1;
    auto writer = [&cbuf1]() {
        const char* letters = "abcdefghijklmnopqrstuvwxyz";
        int count = 0;
        int i = 0;
        string ss;
        while (i < strlen(letters)) {
            int ret = cbuf1.write(letters+i, 1);
            if (ret <= 0) {
                cout << "*" << this_thread::get_id() 
                            << "**[" << ++count << "] failed to write: " 
                            << *(letters+i) << endl;
            } else {
                ss += *(letters+i);
                ++i;
            }
            this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        cout << "*writer*" << this_thread::get_id() << ": " << ss << endl;
    };
    bool stopped = false;
    auto reader = [&cbuf1, &stopped]() {
        int count = 0;
        string ss;
        while (1) {
            char ch = cbuf1.read();
            if (ch == '\0') {
                cout << "*" << this_thread::get_id()
                                << "**[" << ++count << "] read nothing." 
                                << endl;
            } else {
                ss += ch;
                // this judgement doesn't work well for mutiple writer (multiple 'z')
                if (ch == 'z') stopped = true;
            }
            if (stopped) break;
            this_thread::sleep_for(std::chrono::milliseconds(80));
        }
        cout << "*reader*" << this_thread::get_id() << ": " << ss << endl;
    };
    thread th_w1(writer);
    thread th_w2(writer);
    thread th_r1(reader);
    thread th_r2(reader);
    th_w1.join();
    th_w2.join();
    th_r1.join();
    th_r2.join();
}

TEST(language, flagset) {
    auto t = std::bitset<4>();
    assert((int)Options::FULLSCREEN == 0);
    test_AND();
    test_OR();
    test_set_reset();
}


