#include "../header.h"
#include "../language/auto.h"
#include "../language/classtest.h"
#include "../language/speed-dynamic-cast.cpp"
#include "../language/thread.h"

void Test_constructorOrder() {
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

void Test_auto() {
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

void Test_typeinfo() {
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

void Test_dynamic_cast() {
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

void Test_thread() {
    Recurisve_Mutex oo;
    oo.run();
}
