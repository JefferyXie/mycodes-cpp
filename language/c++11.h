#pragma once

#include "../core/header.h"

/******************************************************************************
 *
 * A few valuable resource:
 * http://stackoverflow.com/questions/9299101/what-c-idioms-are-deprecated-in-c11
 * http://www.cnblogs.com/catch/p/3507883.html
 *
 * ***************************************************************************/

namespace legacy {
template <class T>
auto begin(T& t) -> decltype(t.Legacy_begin())
{
    return t.Legacy_begin();
}
template <class T>
auto end(T& t) -> decltype(t.Legacy_end())
{
    return t.Legacy_end();
}
// classes definition
template <class T>
class Container_vector
{
private:
    std::vector<T> m_vec;

public:
    Container_vector(const std::vector<T>& src) { std::copy(src.begin(), src.end(), std::back_inserter(m_vec)); }

    auto                              Legacy_begin() -> decltype(m_vec.begin()) { return m_vec.begin(); }
    typename std::vector<T>::iterator Legacy_end()    // need typename, otherwise, compile error
    {
        return m_vec.end();
    }
};
template <class T>
class Container_map
{
private:
    std::map<int, T> m_map;

public:
    Container_map(const std::map<int, T>& src) { std::copy(src.begin(), src.end(), std::inserter(m_map, m_map.end())); }

    auto Legacy_begin() -> decltype(m_map.begin()) { return m_map.begin(); }
    auto Legacy_end() -> decltype(m_map.begin()) { return m_map.end(); }
};
}    // namespace legacy

class C11
{
public:
    // 001: auto
    template <class T1, class T2>
    static auto compose(T1 t1, T2 t2) -> decltype(t1 + t2)
    {
        return t1 + t2;
    }

    // decltype: https://msdn.microsoft.com/en-us/library/dd537655.aspx
    static auto fo(int n) -> decltype(compose(n, n))    // decltype(n)
    {
        std::cout << n << std::endl;
        return 10;
    }

    static void Run_auto()
    {
        auto i = 42;
        auto l = 42LL;
        std::cout << compose(i, l) << std::endl;
        std::map<std::string, int> oMap = {
            {"1st", 1},
            {"2nd", 2},
            {"3rd", 3},
        };
        oMap.insert(std::make_pair<std::string, int>("4th", 4));
        for (auto itr = oMap.begin(); itr != oMap.end(); ++itr)
            std::cout << itr->first << "  " << itr->second << std::endl;

        for (auto itr = std::begin(oMap); itr != std::end(oMap); ++itr)
            std::cout << itr->first << "  " << itr->second << std::endl;
    }

    // 002: nullptr, nullptr_t
    static void Run_nullptr()
    {
        auto fun_pint = [](int* p) {
            std::cout << "fun_pInt(int*):" << *p << std::endl;
        };
        [[maybe_unused]] auto fun_pshared_ptr = [](std::shared_ptr<int> p) {
            std::cout << "fun_pshared_ptr(shared_ptr<int>):" << *p << std::endl;
        };
        auto fun_nullptr_t = [](std::nullptr_t) {
            std::cout << "fun_nullptr_t(nullptr_t):nullptr as input only" << std::endl;
        };

        int* p1 = NULL;
        // shared_ptr<int> p3 = new int(10);

        // bool b = nullptr; // nullptr > bool but cannot nullptr > int
        // long i = static_cast<long>(nullptr); // compile error, static_cast doesn't work
        [[maybe_unused]] auto l = reinterpret_cast<long>(nullptr);    // reinterpret_cast works
        fun_pint(p1);
        // fun_pshared_ptr(p3);
        auto p4 = nullptr;
        fun_nullptr_t(p4);
    }

    // 003: range-based for loops
    static void Run_loops()
    {
        // will call initializer_list<int> for {...}
        for (auto x : {4, 2, 6, 9, 0})
            std::cout << x << ", ";
        std::cout << std::endl;

        std::vector<int>             vec1 = {0, 1, 2, 3};
        std::vector<int>             vec2 = {3, 4, 2};
        std::map<std::string, void*> oMap = {
            {"1st", &vec1},
            {"2nd", &vec2},
        };
        for (const auto& kp : oMap) {
            std::cout << kp.first << " {";
            for (auto& v : *(static_cast<std::vector<int>*>(kp.second)))
                std::cout << v << "  ";
            std::cout << "}" << std::endl;
        }

        std::string arr[] = {"1st str", "2nd str", "3rd str"};
        for (auto& str : arr)
            std::cout << str << "  ";
        std::cout << std::endl;

        std::vector<std::string> vecStr = {"1st Str", "2nd Str", "3rd Str"};
        std::copy(std::begin(vecStr), std::end(vecStr), std::ostream_iterator<std::string>(std::cout, ", "));
        std::cout << std::endl;

        legacy::Container_vector<std::string> con_vec(vecStr);
        std::copy(begin(con_vec), end(con_vec), std::ostream_iterator<std::string>(std::cout, ", "));
        std::cout << std::endl;

        std::map<int, std::string> mapStr = {
            {1, "1st pair"},
            {2, "2nd pair"},
            {3, "3rd pair"},
        };
        legacy::Container_map<std::string> con_map(mapStr);
        for (auto& vp : con_map)
            std::cout << vp.first << " " << vp.second << ", ";
        std::cout << std::endl;
    }

    // 004: override and final
    class B
    {
    public:
        B() { std::cout << "B::B()" << std::endl; }
        virtual ~B() { std::cout << "B::~B()" << std::endl; }
        virtual void f(int) const { std::cout << "B::f(int) const" << std::endl; }
        virtual void f(int) { std::cout << "B::f(int)" << std::endl; }
        virtual void f(B*) { std::cout << "B::f(B*)" << std::endl; }

        void f() { std::cout << "B::f()" << std::endl; }
        // error: cannot overload only by return type
        // int f() { std::cout << "int B::f()" << std::endl; }
    };
    class D : public B
    {
    public:
        virtual void f(int) const override { std::cout << "D::f(int) const" << std::endl; }
        // child class cannot override f(B*) anymore because of keyword "final"
        virtual void f(B*) final { std::cout << "D::f(B*)" << std::endl; }
        virtual void f(D*) { std::cout << "D::f(D*)" << std::endl; }

        // no error: this is in different scope than B::f()
        int f()
        {
            std::cout << "int D::f()" << std::endl;
            return 0;
        }
    };
    class G : D
    {
    public:
        void f() { std::cout << "G::f()" << std::endl; }
    };

    static void Run_override_final()
    {
        auto b0 = B();
        b0.f(0);
        const auto& b1 = b0;
        b1.f(0);

        B* pd0 = new D();
        pd0->f(&b0);
        pd0->f(pd0);
        pd0->f();
        D* pd1 = static_cast<D*>(pd0);
        pd1->f(pd1);
        pd1->f();

        // static_cast is not safe as dynamic_cast which will do type check
        D* ps = static_cast<D*>(&b0);
        D* pd = dynamic_cast<D*>(&b0);

        std::shared_ptr<D> s_ptr(new D());
        s_ptr.get()->f();
        B* pd2 = s_ptr.get();
        pd2->f(pd2);
        pd2->f();
    }

    // 005: strongly-typed enums
    enum RGB_98 { RED, GREEN, BLUE };
    // error since this is 98 style and duplicate names in the same scope is prevented
    // enum RGB_98s { RED, GREEN, BLUE };
    enum class RGB_11 { RED, GREEN, BLUE, NONE };
    enum class RGB_11s { RED, GREEN, BLUE };
    enum class RGB_char : char { RED = 1, GREEN, BLUE };
    enum class RGB_int : int64_t { RED = 1, GREEN, BLUE };
    static void f_enum(int) { std::cout << "f_enum(int)" << std::endl; }
    static void f_enum(char) { std::cout << "f_enum(char)" << std::endl; }
    static void f_enum(RGB_98) { std::cout << "f_enum(RGB_98)" << std::endl; }
    static void f_enum(RGB_11) { std::cout << "f_enum(RGB_11)" << std::endl; }
    static void f_enum(RGB_char) { std::cout << "f_enum(RGB_char)" << std::endl; }
    static void Run_enum()
    {
        f_enum(RED);    // if has f_enum(RGB_98) defined, won't call f_enum(int)
        f_enum((int)GREEN);
        f_enum(RGB_98::BLUE);
        f_enum(RGB_11::RED);
        f_enum(RGB_char::BLUE);
        // error since no corresponding function
        // f_enum(RGB_int::GREEN);

        std::cout << "sizeof(RGB_98::RED): " << sizeof(RGB_98::RED) << std::endl;
        std::cout << "sizeof(RGB_11::RED): " << sizeof(RGB_11::RED) << std::endl;
        std::cout << "sizeof(RGB_char::RED): " << sizeof(RGB_char::RED) << std::endl;
        std::cout << "sizeof(RGB_int::RED): " << sizeof(RGB_int::RED) << std::endl;

        auto i = (int)RGB_11::RED;
        auto j = (int)RGB_char::BLUE;
        auto k = (char)RGB_char::GREEN;
        std::cout << i << " " << j << " " << k << std::endl;
    }

    // 006: smart pointers
    template <class T>
    static void f_smart(T* p)
    {
        std::cout << "f_smart(T*): " << (nullptr == p ? "nullptr" : "not null") << std::endl;
    }
    struct Fo {
        int rnd = rand();
        Fo() { std::cout << "Fo(), @rnd:" << rnd << std::endl; }
        ~Fo() { std::cout << "~Fo(), @rnd:" << rnd << std::endl; }
    };
    struct Fo_D {
        int  count = 0;
        void goo() { std::cout << "Fo_D::goo()..." << std::endl; }
        void operator()(Fo* p)
        {
            std::cout << "Fo_D::operator(), Fo.rnd:" << p->rnd << ", [count]:" << ++count << std::endl;
            delete p;
        }
    };
    struct B_share;
    struct A_share {
        ~A_share() { std::cout << "~A_share()" << std::endl; }
        // if we use shared_ptr here, both A_share and B_share objects wont' destory
        // the solution is to use weak_ptr. Use in either A_share or B_share.
        // http://stackoverflow.com/questions/4984381/shared-ptr-and-weak-ptr-differences
        // shared_ptr<B_share> b;
        std::weak_ptr<B_share> b;
    };
    struct B_share {
        ~B_share() { std::cout << "~B_share()" << std::endl; }
        std::shared_ptr<A_share> a;
    };

    struct Shared_cycle {
        ~Shared_cycle() { std::cout << "~Shared_cycle()" << std::endl; }
        // if we use shared_ptr here, it causes cycle problem so object wont' destory
        // the solution is to use weak_ptr.
        // http://stackoverflow.com/questions/4984381/shared-ptr-and-weak-ptr-differences
        std::shared_ptr<Shared_cycle> next_;
    };
    struct Shared_no_cycle {
        ~Shared_no_cycle() { std::cout << "~Shared_no_cycle()" << std::endl; }
        std::weak_ptr<Shared_no_cycle> next_;
    };

    static void Run_smartpointers()
    {
        // these codes explain how weak_ptr work with shared_ptr
        {
            std::shared_ptr<A_share> p_a(new A_share());
            std::shared_ptr<B_share> p_b(new B_share());
            p_a.get()->b = p_b;
            p_b.get()->a = p_a;

            std::cout << "p_a.use_count=" << p_a.use_count() << std::endl;
            std::cout << "p_b.use_count=" << p_b.use_count() << std::endl;
            std::cout << "p_a->b.use_count=" << p_a->b.use_count() << std::endl;
            std::cout << "p_b->a.use_count=" << p_b->a.use_count() << std::endl;
        }

        {
            std::shared_ptr<Shared_cycle> sharedCycle_1(new Shared_cycle());
            std::shared_ptr<Shared_cycle> sharedCycle_2(new Shared_cycle());
            sharedCycle_1->next_ = sharedCycle_2;
            sharedCycle_2->next_ = sharedCycle_1;

            std::cout << "sharedCycle_1.use_count=" << sharedCycle_1.use_count() << std::endl;
            std::cout << "sharedCycle_2.use_count=" << sharedCycle_2.use_count() << std::endl;
        }

        {
            std::shared_ptr<Shared_no_cycle> noCycle_1(new Shared_no_cycle());
            std::shared_ptr<Shared_no_cycle> noCycle_2(new Shared_no_cycle());
            noCycle_1->next_ = noCycle_2;
            noCycle_2->next_ = noCycle_1;

            {
                std::cout << "noCycle_1.use_count=" << noCycle_1.use_count() << std::endl;
                // rvalue shared_ptr won't increase ref count
                noCycle_2->next_.lock();
                // lvalue shared_ptr will increase ref count
                auto sp = noCycle_2->next_.lock();
                std::cout << "noCycle_1.use_count=" << noCycle_1.use_count() << std::endl;
            }

            std::cout << "noCycle_1.use_count=" << noCycle_1.use_count() << std::endl;
            std::cout << "noCycle_2.use_count=" << noCycle_2.use_count() << std::endl;
        }

        // difference between unique_ptr and shared_ptr
        // http://stackoverflow.com/questions/6876751/differences-between-unique-ptr-and-shared-ptr
        std::unique_ptr<Fo, Fo_D> up(new Fo(), Fo_D());
        auto                      del = up.get_deleter();
        del.goo();

        Fo_D                       fd;
        std::unique_ptr<Fo, Fo_D>  up1(new Fo(), up.get_deleter());
        std::unique_ptr<Fo, Fo_D&> down(new Fo(), fd);
        std::unique_ptr<Fo, Fo_D&> down1(new Fo(), down.get_deleter());

        std::cout << "reset up...";
        up.reset(new Fo());
        std::cout << "reset up1...";
        up1.reset(new Fo());
        std::cout << "reset down...";
        down.reset(new Fo());
        std::cout << "reset down1...";
        down1.reset(new Fo());

        std::cout << "call down1 deleter...";
        down1.get_deleter()(new Fo());

        auto fun_del = [](int* p) {
            std::cout << "fun_del, *p:" << *p << std::endl;
            delete p;
            p = nullptr;
        };
        std::unique_ptr<int, decltype(fun_del)> u0(new int(10), fun_del);
        [[maybe_unused]] auto                   f_del = u0.get_deleter();

        /*
        unique_ptr<int> p1(new int(22));
        auto p2 = move(p1);
        f_smart(p1.get());
        f_smart(p2.get());

        auto ff = [] (shared_ptr<int> p) { ++(*p); };
        auto fp = [] (int* p) { ++(*p); };
        shared_ptr<int> p3(new int(11));
        auto p4 = p3;
        ff(p3);
        fp(p4.get());

        // http://stackoverflow.com/questions/20895648/difference-in-make-shared-and-normal-shared-ptr-in-c
        // http://stackoverflow.com/questions/24741061/how-to-avoid-big-memory-allocation-with-stdmake-shared
        // although the memory cost is same, make_shared performs better since it only allocate memory once
        // while shared_ptr will do twice: one is the 'new' and the other happens inside ref control
        shared_ptr<B> p5(new B());
        auto p6 = make_shared<B>();
        f_smart(p6.get());

        // when use vector for shared_ptr, below doesn't work since brace doesn't work well
        // in most type deduction contexts
        // shared_ptr<vector<int> > sp = std::make_shared<vector<int> > {1,2,5};
        // Instead, there're two ways to initialize vector for shared_ptr:
        // 1), explictly call std::initializer_list
        auto sp3 = std::make_shared<vector<int> > (std::initializer_list<int> {1,2,5});
        // 2), create vector object and pass it as parameter
        shared_ptr<vector<int> > sp4 = std::make_shared<vector<int> > (vector<int> {1,2,5});
        // this is to call vector<int>(int, int) constructor
        auto sp5 = std::make_shared<vector<int> > (3,2);

        weak_ptr<B> wp = p5;
        {
            auto sp = wp.lock();
            f_smart(sp.get());
        }
        p5.reset();

        if (wp.expired())
            std::cout << "wp is expired" << std::endl;
        */
    }

    // 007: lambdas
    static void Run_lambdas()
    {
        int arr[] = {3, 4, -2, 8, -5};
        std::sort(arr, arr + sizeof(arr) / sizeof(arr[0]), [](int a, int b) {
            return abs(a) > abs(b);
        });

        int num_positive = 0;
        int num_negative = 0;
        // error: for_each(arr, arr+sizeof(arr)/sizeof(arr[0]), [](int a) { if (a > 0) num_positive++; });
        std::for_each(arr, arr + sizeof(arr) / sizeof(arr[0]), [&](int a) {
            if (a > 0)
                num_positive++;
        });
        std::for_each(arr, arr + sizeof(arr) / sizeof(arr[0]), [&num_positive](int a) {
            if (a > 0)
                num_positive++;
        });
        // error: read-only! for_each(arr, arr+sizeof(arr)/sizeof(arr[0]), [=](int a) { if (a > 0) num_positive++; });

        num_positive = 0;
        num_negative = 0;
        std::for_each(arr, arr + sizeof(arr) / sizeof(arr[0]), [&](int a) {
            a > 0 ? ++num_positive : ++num_negative;
        });
        std::for_each(arr, arr + sizeof(arr) / sizeof(arr[0]), [&num_positive, &num_negative](int a) {
            a > 0 ? ++num_positive : ++num_negative;
        });

        int              i = 1, j = 1;
        std::vector<int> vec_int(5, 1);
        generate_n(vec_int.begin() + 2, vec_int.size() - 2, [&] {
            int n = i + j;
            i     = j;
            j     = n;
            return n;
        });

        auto fo = [=, &j] {
            return i + j;
        };
        auto go = [&, j] {
            return i + j;
        };
        auto ho = [&] {
            return i + j;
        };
        auto jo = [=] {
            return i + j;
        };
        i = j = 10;
        std::cout << fo() << std::endl;
        std::cout << go() << std::endl;
        std::cout << ho() << std::endl;
        std::cout << jo() << std::endl;

        int t = 5;
        // higher order lambda expression, embedded lambda
        auto addtwo = [](int x) -> std::function<int(int)> {
            // if use [&], addtwo(5) will give unexpected result
            // since 5 here is temporary variable, reference isn't suitable
            // also, addtwo(t) will also give unexpected result
            // since addtwo is declared as (int x), meaning x is temporary variable
            // the embedded lambda cannot identify x and refer to x
            // the right expression is:
            /*
            auto addtwo = [](int& x) {
                return [&](int y) { return x+y; };
            }
            auto addtwo = [](int x) {
                int m = x;
                return [&](int y) { return m+y; };
            }*/

            return [=](int y) {
                return x + y;
            };
        };
        auto higherorder = [](const std::function<int(int)>& ff, int k) {
            return ff(k) * 2;
        };
        std::cout << higherorder(addtwo(5), 3) << std::endl;
        std::cout << higherorder(addtwo(t), 3) << std::endl;

        std::cin >> num_negative;
    }

    // 008: non-member begin() and end()
    template <class T>
    static void bar(T begin, T end)
    {
        std::for_each(begin, end, [](int n) {
            std::cout << n << std::endl;
        });
        auto is_odd = [](int n) {
            return n % 2 == 1;
        };
        auto pos = std::find_if(begin, end, is_odd);
        if (pos != end)
            std::cout << *pos << std::endl;
    }
    template <class T>
    static void foo(T& t)
    {
        bar(begin(t), end(t));
    }
    // below foo is not necessary since above foo has T reference as para
    // not sure why template has "size_t N" character?
    template <class T, size_t N>
    static void foo(T (&arr)[N])
    {
        bar(std::begin(arr), std::end(arr));
    }

    static void Run_beginend()
    {
        int arr[] = {3, 5, 2, 1};
        foo(arr);

        std::vector<int> v = {3, 2, 6, 7, 8};
        foo(v);
    }

    // 009: static_assert and type traits
    // static_assert is compile-time check
    template <class T, size_t N>
    class VV
    {
        static_assert(N < 3, "size is too big");
    };
    template <class T1, class T2>
    static auto add(T1 t1, T2 t2) -> decltype(t1 + t2)
    {
        // static_assert(typeid(T1) == typeid(T2), "t1 and t2 are different types");
        static_assert(std::is_integral<T1>::value, "t1 should be integral");
        static_assert(std::is_integral<T2>::value, "t2 should be integral");
        return t1 + t2;
    }
    static void Run_static_assert()
    {
        [[maybe_unused]] VV<int, 2> v1;
        // compile error from static_assert
        // VV<int, 5> v2;

        add(1, 2);
        // compile error
        // add(1, 5.1);
        // add("ab", 2);
        // add(2, "ab");
    }

    // 010: move semantics
    class Buffer
    {
    private:
        std::string          m_name;
        std::unique_ptr<int> m_buffer_int;
        std::vector<B>       m_buffer_B;

    public:
        Buffer() { std::cout << "Buffer()" << std::endl; }
        Buffer(const std::string& name) : m_name(name) { std::cout << "Buffer(const string&)" << std::endl; }
        Buffer(const Buffer& buf) : m_name(buf.m_name)
        {
            std::cout << "Buffer(const Buffer&)" << std::endl;
            if (nullptr != buf.m_buffer_int.get())
                // m_buffer_int = make_unique<int>(*buf.m_buffer_int.get());
                // m_buffer_int = move(unique_ptr<int>(new int(*buf.m_buffer_int.get())));
                copy(buf.m_buffer_B.begin(), buf.m_buffer_B.end(), back_inserter(m_buffer_B));
        }
        Buffer& operator=(const Buffer& buf)
        {
            std::cout << "operator=(const Buffer&)" << std::endl;
            if (this != &buf) {
                m_name = buf.m_name;
                m_buffer_int.reset();    // equal to m_buffer_int = nullptr;
                if (buf.m_buffer_int.get() != nullptr) {
                    // m_buffer_int = make_unique<int>(*buf.m_buffer_int.get());
                    // m_buffer_int = move(unique_ptr<int>(new int(*buf.m_buffer_int.get())));
                }
                copy(begin(buf.m_buffer_B), end(buf.m_buffer_B), back_inserter(m_buffer_B));
            }
            return *this;
        }
        // move constructor
        Buffer(Buffer&& temp) = default;
        /*{
            std::cout << "Buffer(Buffer&&)" << std::endl;
            m_name = temp.m_name;
            //m_name = move(temp.m_name);
            m_buffer_int = move(temp.m_buffer_int);
            m_buffer_B = move(temp.m_buffer_B);
            //m_buffer_B.swap(temp.m_buffer_B);
        }*/
        // move assignment
        Buffer& operator=(Buffer&& temp) = default;
        /*{
            std::cout << "operator=(Buffer&&)" << std::endl;
            assert(this != &temp);
            m_name = temp.m_name;
            m_buffer_int = move(temp.m_buffer_int);
            m_buffer_B = move(temp.m_buffer_B);
            return *this;
        }*/
        void Init()
        {
            m_buffer_B.push_back(B());
            m_buffer_B.push_back(B());
            // m_buffer_int = make_unique<int>(100);
            m_buffer_int = std::move(std::unique_ptr<int>(new int(100)));
        }
    };
    static void Run_move()
    {
        auto createBuffer = [](const std::string& name) {
            // use "move" will force to use move constructor
            return std::move(Buffer(name));
            // return object directly won't call move constructor or
            // copy constructor because of "copy elision"
            // http://en.cppreference.com/w/cpp/language/copy_elision
            // http://blog.csdn.net/zwvista/article/details/6845020
            // return Buffer(name);
        };
        Buffer buf1;
        Buffer buf2("2nd buf");
        auto   buf3 = buf2;
        auto   buf4 = createBuffer("4th buf");
        buf1        = createBuffer("overwrite buf1");

        // use "move" will force to use move constructor
        auto buf5 = std::move(buf4);
    }

    // 011: forward
    static void overloaded(const int& x) { std::cout << "[lvalue]: " << x << std::endl; }
    static void overloaded(int&& x) { std::cout << "[rvalue]: " << x << std::endl; }

    // function template taking rvalue reference to deduced type:
    template <class T>
    static void fn(T&& x)
    {
        overloaded(x);                     // always an lvalue, why fn(0) also call lvalue???
        overloaded(std::forward<T>(x));    // rvalue if argument is rvalue
    }

    static void fon(int&& x)
    {
        overloaded(x);
        overloaded(std::forward<int>(x));
        overloaded(std::forward<int&>(x));
        overloaded(std::forward<int&&>(x));
    }

    static void Run_forward()
    {
        InClass inA;
        inA.a           = 15;
        auto getInClass = []() {
            InClass a;
            a.a = 20;
            return a;
        };
        overloaded(getInClass().a);    // member of rvalue is still rvalue
        overloaded(std::forward<int>(inA.a));
        std::string u = std::forward<std::string>(inA.s);
        std::string t = std::move(inA.s);

        std::cout << "***fon(10)" << std::endl;
        fon(10);

        int i = 2;
        std::cout << "***overloaded(3)" << std::endl;
        overloaded(3);
        std::cout << "***overloaded(i)" << std::endl;
        overloaded(i);
        std::cout << "***overloaded(forward<int>(3))" << std::endl;
        overloaded(std::forward<int>(3));
        // Question: forward is supposed to not convert i to rvalue
        // since i is lvalue, however it DOES. Why? Is this because
        // forward is only meaningful at function template when type
        // is uncerntain?
        // Answer: here should use forward<int&>(i) since i is treated as reference!
        // http://stackoverflow.com/questions/17539282/stdforward-test-in-template-and-non-template-function
        // http://blog.csdn.net/zwvista/article/details/6848582
        std::cout << "***overloaded(forward<int>(i))" << std::endl;
        overloaded(std::forward<int>(i));
        std::cout << "***overloaded(move(i))" << std::endl;
        overloaded(std::move(i));

        auto ffn = [](std::string&& v) {
            // auto m = move(v); std::cout << m << " " << v << std::endl;
            std::cout << "ffn(string&&): " << v << std::endl;
        };
        std::string s = "10";
        // ffn(s);
        std::cout << "***ffn(forward<string>(s))" << std::endl;
        ffn(std::forward<std::string>(s));
        std::cout << "***ffn(move(s))" << std::endl;
        ffn(std::move(s));
        std::cout << "***ffn(\"abc\")" << std::endl;
        ffn("abc");

        int a = 2;
        std::cout << "calling fn with lvalue: " << std::endl;
        fn(a);
        std::cout << '\n';

        std::cout << "calling fn with rvalue: " << std::endl;
        fn(5);
        std::cout << '\n';

        const int& ref = a;
        int&&      b   = 10;    // b is lvalue, int type
        fn(ref);                // treat ref as lvalue
        fn(b);                  // treat b as lvalue
        auto gn = [](int&& v) {
            fn(v);
            fn(std::forward<int>(v));
            fn(std::move(v));
        };
        gn(5);
        gn([]() {
            return 11;
        }());
    }

    // 012: in-class member initializer
    class InClass
    {
    public:
        int                  a = 10;
        std::string          s = "in class init.";
        std::unique_ptr<int> p;
        // unique_ptr<int> p = make_unique<int>(15);
    };
    static void Run_inclass_initializer()
    {
        int     i = 10;
        InClass ic;
        std::cin >> i;
    }

    // 013: delegating constructor
    class Delegating
    {
        int a = 11;

    public:
        Delegating(int m) : a(m) { std::cout << "Delegating(int)" << std::endl; }
        Delegating() : Delegating(5)    // delegating constructor
        {
            std::cout << "Delegating()" << std::endl;
        }
        Delegating(std::string s) : Delegating(stoi(s))    // delegating constructor
        {
            std::cout << "Delegating(string)" << std::endl;
        }
    };

    static void Run_delegating_constructor()
    {
        Delegating del1;
        Delegating del2(2);
        Delegating del3("a");
        int        i = 10;
        std::cin >> i;
    }

    // 014: type traits
    struct A {
    };
    struct C {
        virtual void f() {}
    };
    static void Run_type_traits()
    {
        std::cout << "has_virtual_destructor<int>: " << std::has_virtual_destructor<int>::value << std::endl;
        std::cout << "is_polymorphic<int>: " << std::is_polymorphic<int>::value << std::endl;
        std::cout << "is_polymorphic<A>: " << std::is_polymorphic<A>::value << std::endl;
        std::cout << "is_polymorphic<C>: " << std::is_polymorphic<C>::value << std::endl;
        std::cout << "is_abstract<C>: " << std::is_abstract<C>::value << std::endl;
        typedef int mytype[][10][20];
        std::cout << "0 dim: " << std::extent<mytype, 0>::value << std::endl;
        std::cout << "1 dim: " << std::extent<mytype, 1>::value << std::endl;
        std::cout << "2 dim: " << std::extent<mytype, 2>::value << std::endl;

        std::cout << "is_void(A): " << std::is_void<A>::value << std::endl;
        std::cout << "is_class(A): " << std::is_class<A>::value << std::endl;

        int       a;
        const int b = 5;
        int       c = 10;
        std::cout << "is_const<int>: " << std::is_const<int>::value << std::endl;
        std::cout << "is_const<const int>: " << std::is_const<const int>::value << std::endl;
        std::cout << "is_const<const int&>: " << std::is_const<const int&>::value << std::endl;
        std::cout << "is_const<decltype(a)>: " << std::is_const<decltype(a)>::value << std::endl;
        std::cout << "is_const<decltype(b)>: " << std::is_const<decltype(b)>::value << std::endl;
        std::cout << "is_const<decltype(c)>: " << std::is_const<decltype(c)>::value << std::endl;
    }

    // 015: bind function
    static void f(int n1, int n2, int n3, const int& n4, int n5)
    {
        std::cout << n1 << ", " << n2 << ", " << n3 << ", " << n4 << ", " << n5 << "." << std::endl;
    }
    struct Foo {
        int  data = 10;
        void print_sum(int n1, int n2) { std::cout << n1 + n2 << std::endl; }
    };

    static void Run_bind()
    {
        using namespace std::placeholders;

        // bind with std::placeholders and reference (cref)
        int  n  = 5;
        auto f1 = std::bind(f, _2, _1, 8, std::cref(n), n);
        n       = 9;
        f1(1, 2, 100);

        // bind with class's member function
        Foo  foo;
        auto f2 = std::bind(&Foo::print_sum, &foo, 20, _1);
        f2(3);
        auto f3  = std::bind(&Foo::print_sum, _2, _1, 3);
        foo.data = 15;
        f3(1, foo);
        auto f4  = std::bind(&Foo::print_sum, &foo, std::cref(foo.data), 2);
        foo.data = 20;
        f4();

        // bind with random uniform distribution
        std::default_random_engine      rndEng;
        std::uniform_int_distribution<> uniDist(0, 10);
        std::function<int()>            rnd = std::bind(uniDist, rndEng);
        for (int i = 0; i < 10; ++i)
            std::cout << rnd() << ", ";
        std::cout << std::endl;

        std::linear_congruential_engine<uint64_t, 10000, 200, 30000> generator(10000);
        int                                                          rn   = uniDist(generator);
        auto                                                         dice = std::bind(uniDist, generator);
        std::cout << dice() << ", " << dice() << ", " << dice() << std::endl;

        auto mean = uniDist(rndEng);

        // bind with normal distribution
        std::random_device         rndDev;
        std::mt19937               mtEng(rndDev());
        std::normal_distribution<> norm_dist(mean, 2);

        std::map<int, int> hist;
        for (int i = 0; i < 100; ++i)
            ++hist[round(norm_dist(mtEng))];

        std::cout << "Normal distribution around " << mean << ":" << std::endl;
        for (auto p : hist) {
            std::cout << std::fixed << std::setprecision(1) << std::setw(2) << p.first << ' '
                      << std::string(p.second, '*') << std::endl;
        }

        // bind with std algorithm
        int arr[] = {3, 2, 5, 1, 0};
        /*
        transform(arr, arr+sizeof(arr)/sizeof(arr[0]), arr, bind(std::pow, _1, 2));
        for_each(arr, arr+sizeof(arr)/sizeof(arr[0]), bind(std::sqrt, _1));
        for_each(arr, arr+sizeof(arr)/sizeof(arr[0]), std::sqrt);
        */

        auto output = [](std::ostream& os, int x) {
            os << x << std::endl;
            // return os;
        };
        std::for_each(arr, arr + sizeof(arr) / sizeof(arr[0]), std::bind(output, std::ref(std::cout), _1));
    }

    // 016: std::tuple, std::tie
    static void Run_tuple()
    {
        std::tuple<int, double, std::string> t0(2, 4.1, "t0 tuple");
        auto                                 s = std::get<2>(t0);
        std::get<0>(t0)                        = 5;
        int    m;
        double db;
        tie(m, db, s) = t0;

        std::tuple<int, char> t1;
        t1                  = std::make_tuple(11, 'm');
        tie(m, std::ignore) = t1;

        std::cout << "t0 size: " << std::tuple_size<decltype(t0)>::value << std::endl;
        std::cout << "t1 size: " << std::tuple_size<decltype(t1)>::value << std::endl;

        std::cout << std::is_const<std::tuple_element<0, decltype(t0)>::type>::value << std::endl;

        int                    a, b;
        std::tuple<int&, int&> test(a, b);
        std::get<0>(test) = 1;
        std::get<1>(test) = 2;
        std::cout << a << ":" << b << std::endl;

        int& ar    = a;
        int& br    = b;
        auto test2 = std::forward_as_tuple(ar, br);
        // auto test3 = std::make_tuple(ar,br); // this cannot make tuple of int& reference
        std::get<0>(test2) = -1;
        std::get<1>(test2) = -2;
        std::cout << a << ":" << b << std::endl;
    }

    // 017: initializer_list
    class E
    {
    public:
        E() {}
        E(std::vector<int>) {}
        E(int) {}
        E(B) {}
        // E(const B&) {}
        // E(B&&) {}
        void f() {}
        // E(initializer_list<int>) {}
        // E(std::initializer_list<int double>) {}
    };

    static void Run_initializer_list()
    {
        std::vector<int>           v0(3);    // 3 elements: 0
        std::vector<int>           v1{3};    // 1 element: 3
        std::map<int, std::string> mp;
        mp = {
            {4, "1st"},
            {2, "2nd"},
            {5, "3rd"},
            {1, "4th"},
        };    // call initializer_list
        std::for_each(begin(mp), end(mp), [](std::pair<int, std::string> p) {
            std::cout << p.first << ", " << p.second << "; ";
        });
        std::cout << std::endl;
        auto fun = [](std::vector<int> v) -> std::vector<int> {
            for (auto o : v)
                std::cout << o << ",";
            std::cout << std::endl;
            return {2, 1, 8};    // call initializer_list
        };
        auto arr = fun({5, 6});    // call initializer list
        for (auto s : {"1st", "2nd"})
            std::cout << s << ",";
        std::cout << std::endl;
        // for (pair<int, string> p : {{2, "1st"}, {1, "2nd"}}) std::cout << p.first << ", " << p.second << "; ";
        // std::cout << std::endl;

        E e0(B());    // most vexing parse! declares a function named e0
        B b;
        E e2(b);    // call E(B) ctor
        e2.f();
        E e1{B()};    // call E(B)
        // e0.f();
        e1.f();
        E e3({1, 2});
        // E e3{1, 2}; // need E(int, int) ctor or declare E(initizlier_list<int>)
        E e4{};    // need default ctor E(). NOTE: if there's any non-default constructor created,
                   // compiler won't generate default ctor until you explictly point it out.
    }
    // 018: default & delete
    static void isLucky(int num) {}
    static void isLucky(char)   = delete;
    static void isLucky(bool)   = delete;
    static void isLucky(double) = delete;
    class F
    {
    public:
        F()                    = default;
        F(const F&)            = delete;
        F& operator=(const F&) = delete;
        virtual ~F()           = default;
    };

    static void Run_default_delete()
    {
        Buffer buf1;
        Buffer buf2("2nd buf");
        auto   buf3 = Buffer("3rd");

        F f1;
        // auto f2 = f1; // error, no allowed to use 'delete' function
        // F f3(f1); // error, same reason as above

        isLucky(10);
        // below calls are not allowed. If they are not "delete", they will be
        // converted to use isLucky(int)
        // isLucky(1.0);
        // isLucky(true);
        // isLucky('b');
    }

    // 019: constexpr
    static constexpr int Fib(int n) { return n <= 2 ? 1 : (Fib(n - 1) + Fib(n - 2)); }
    static constexpr int Pow(int base, int exp)
    {
        // pow(..) is not constexpr since c++17
        // return exp == 0 ? 1 : base * pow(base, exp-1);

        // below codes are supposed to be ok under c++14
        auto ret = 1;
        for (int i = 0; i < exp; ++i)
            ret *= base;
        return ret;
    }
    class Point
    {
    public:
        constexpr Point(double xVal = 0, double yVal = 0) : x(xVal), y(yVal)
        {
            // std::cout << "constexpr Point(double, double)" << std::endl;
        }
        constexpr double GetX() const { return x; }
        constexpr double GetY() const { return y; }
        void             SetX(double xVal) { x = xVal; }
        void             SetY(double yVal) { y = yVal; }

    private:
        double x = 0.0;
        double y = 0.0;
    };
    static constexpr Point MidPoint(const Point& p1, const Point& p2)
    {
        return {(p1.GetX() + p2.GetX()) / 2, (p1.GetY() + p2.GetY()) / 2};
    }

    static void Run_constexpr()
    {
        std::cout << Fib(5) << std::endl;    // compile time
        int i = 3;
        std::cout << Fib(i) << std::endl;    // run time
        constexpr int j = 3;
        std::cout << Fib(j) << std::endl;    // compile time

        [[maybe_unused]] auto           m = Fib(3);    // run time
        [[maybe_unused]] constexpr auto n = Fib(3);    // compile time

        std::array<int, j> arr;
        arr = {2, 1, 5};
        arr = {{4, 3, 6}};

        [[maybe_unused]] std::array<int, Fib(3)>         arr0;    // compile time
        [[maybe_unused]] std::array<int, Pow(2, Fib(3))> arr1;    // compile time

        std::cout << Pow(i, j) << std::endl;    // compile time
        std::cout << Pow(2, j) << std::endl;    // compile time

        constexpr Point                  p1(11.0, 5.2);
        constexpr Point                  p2{4.2, 3.2};
        [[maybe_unused]] constexpr Point p3{3, 7};
        [[maybe_unused]] constexpr auto  mid = MidPoint(p1, p2);
    }

    // 020: variadic templates
    static void myPrintf(const char* s)
    {
        while (*s) {
            if (*s == '%') {
                if (*(s + 1) == '%')
                    ++s;
                else
                    throw std::runtime_error("invalid format string: missing arguments");
            }
            std::cout << *s++;
        }
    }
    template <class T, class... Args>
    static void myPrintf(const char* s, T value, Args... args)
    {
        while (*s) {
            if (*s == '%') {
                if (*(s + 1) == '%')
                    ++s;
                else {
                    std::cout << value;
                    myPrintf(s + 1, args...);
                    return;
                }
            }
            std::cout << *s++;
        }
        throw std::logic_error("extra arguments provided to myPrintf");
    }

    template <class... T>
    static void dummy_wrapper(T...)
    {
        std::cout << "dummy_wrapper(T...)" << std::endl;
    }
    template <class T>
    static T unpacker(const T t)
    {
        std::cout << "[" << t << "]";
        return t;
    }
    template <class T, class... Args>
    static void dummy_wrapper(T t, Args... args)
    {
        unpacker(t);
        dummy_wrapper(args...);
    }

    static int maximum(int n) { return n; }
    template <class... Args>
    static int maximum(int n, Args... args)
    {
        return std::max(n, maximum(args...));
    }

    static void Run_variadic()
    {
        // array, vector
        auto c_style_f = [](int* x) {
            std::cout << *x << std::endl;
        };
        std::array<std::string, 2> arr_str{"1st", "2nd"};
        std::array<int, 2>         arr_int{0};
        std::vector<int>           v_int = {2, 1, 3};
        c_style_f(arr_int.data());
        c_style_f(v_int.data());
        v_int.reserve(10);        // allocate resource but not init values, capacity() changes but size() has no change
        v_int.resize(15);         // allocate resource and init with default values, size() changes
        v_int.reserve(5);         // no change if new space is smaller than current space
        v_int.resize(2);          // keep size() equal to required number by removing objects in the end
        v_int.shrink_to_fit();    // size() doesn't change, capacity() will be reduced to same as size()
        v_int.clear();            // remove all values, size()==0 while capacity() has no change

        std::vector<Point> v_point;
        v_point.emplace(begin(v_point), 3.1, 2.2);
        v_point.emplace_back(1.2, 2.3);

        // stoi, stol, stoll, stoul, stoull, stof, stod, stold, to_string, to_wstring
        size_t                sz   = 0;
        std::string           str  = "0123456789A";
        auto                  s_i0 = std::stoi(str);
        [[maybe_unused]] auto s_i1 = std::stoi(str, &sz);
        // auto s_i2 = stoi(str, &sz, 16); out of range
        [[maybe_unused]] auto s_ll0 = std::stoll(str);
        [[maybe_unused]] auto s_ll1 = std::stoll(str, &sz, 16);
        [[maybe_unused]] auto s_ul0 = std::stoul(str);
        [[maybe_unused]] auto s_ul1 = std::stoul(str, nullptr, 16);
        [[maybe_unused]] auto s_ull = std::stoull(str, &sz, 16);
        [[maybe_unused]] auto s_f0  = std::stof(str);
        [[maybe_unused]] auto s_f1  = std::stof(str, &sz);
        [[maybe_unused]] auto s_d0  = std::stod(str);
        [[maybe_unused]] auto s_d1  = std::stod(str, &sz);
        [[maybe_unused]] auto s_ld  = std::stold(str, nullptr);

        auto to_s0  = std::to_string(s_i0);
        auto to_s1  = std::to_string(s_f1);
        auto to_s2  = std::to_string(s_d1);
        auto to_ws0 = std::to_wstring(s_i0);
        auto to_ws1 = std::to_wstring(s_f1);
        auto to_ws2 = std::to_wstring(s_d1);

        maximum(4, 3, 6, 1);
        dummy_wrapper(1, "--", "2.3.4", "--", 3.1);
        myPrintf("a%bcde%fghij%kl%mn\n", 11, "interesting", 998, "very good");
    }

    // 021: STL updates
    static void Run_STL()
    {
        /*** stl new algorithms ***/
        // std::all_of, std::none_of, std::any_of
        std::array<int, 10> arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        std::cout << "[std::all_of] are all numbers odd? "
                  << (std::all_of(
                          begin(arr), end(arr),
                          [](int i) {
                              std::cout << i << ", ";
                              return i % 2 == 0;
                          })
                          ? "yes"
                          : "no")
                  << std::endl;
        std::cout << "[std::none_of] is none of number > 10? "
                  << (std::none_of(
                          begin(arr), end(arr),
                          [](int i) {
                              std::cout << i << ", ";
                              return i > 10;
                          })
                          ? "yes"
                          : "no")
                  << std::endl;
        std::cout << "[std::any_of] is any number times of 5?"
                  << (std::any_of(
                          begin(arr), end(arr),
                          [](int i) {
                              std::cout << i << ", ";
                              return i > 0 && i % 5 == 0;
                          })
                          ? "yes"
                          : "no")
                  << std::endl;

        // std::find_if, std::find_if_not
        auto f3 = [&]() {
            auto itr = std::find_if(std::begin(arr), std::end(arr), [](int i) {
                std::cout << i << ", ";
                return i > 0 && i % 3 == 0;
            });
            return itr == std::end(arr) ? "no" : std::to_string(*itr);
        };
        auto f10 = [&]() {
            auto itr = std::find_if_not(std::begin(arr), std::end(arr), [](int i) {
                std::cout << i << ", ";
                return i <= 10;
            });
            return itr == std::end(arr) ? "no" : std::to_string(*itr);
        };
        std::cout << "[std::find_if] first number times of 3: " << f3() << std::endl;
        std::cout << "[std::find_if_not] first number greater than 10: " << f10() << std::endl;

        // string movement is implemented as swap
        std::string str0 = "i'm str0";
        std::string str1 = "i'm str1";
        str1             = std::move(str0);

        // std::move_backward
        std::array<int, arr.size()> dest{0};
        std::move_backward(std::begin(arr), std::end(arr), std::end(dest));
        std::cout << "[std::move_backward]";
        std::cout << "\n\tinitial array: ";
        std::copy(std::begin(arr), std::end(arr), std::ostream_iterator<int>(std::cout, ", "));
        std::cout << "\n\tdest array: ";
        std::copy(std::begin(dest), std::end(dest), std::ostream_iterator<int>(std::cout, ", "));
        std::cout << std::endl;

        // std::shuffle, std::random_shuffle
        std::random_device rd;
        std::mt19937       g(rd());
        std::copy(std::begin(arr), std::end(arr), std::begin(dest));
        std::shuffle(std::begin(dest), std::end(dest), g);
        std::cout << "[std::shuffle] (0:9): ";
        std::copy(std::begin(dest), std::end(dest), std::ostream_iterator<int>(std::cout, ", "));
        std::cout << std::endl;

        std::copy(std::begin(arr), std::end(arr), std::begin(dest));
        // std::random_shuffle(begin(dest), end(dest));
        // std::cout << "[std::random_shuffle] (0:9): ";
        std::copy(std::begin(dest), std::end(dest), std::ostream_iterator<int>(std::cout, ", "));
        std::cout << std::endl;

        std::copy(std::begin(arr), std::end(arr), std::begin(dest));
        std::srand(unsigned(std::time(0)));
        [[maybe_unused]] auto myrandom = [](int i) {
            return std::rand() % i;
        };
        // std::random_shuffle(begin(dest), end(dest), myrandom);
        // std::cout << "[std::random_shuffle, myrandom] (0:9): ";
        std::copy(std::begin(dest), std::end(dest), std::ostream_iterator<int>(std::cout, ", "));
        std::cout << std::endl;

        // std::is_partitioned, std::partition_copy, std::partition_point
        std::copy(std::begin(arr), std::end(arr), std::begin(dest));
        auto isOdd = [](int i) {
            return i % 2 == 1;
        };
        auto f_part = [&]() {
            std::cout
                << (std::is_partitioned(begin(dest), end(dest), isOdd) ? "(partitioned)\n" : "(not partitioned)\n");
        };
        std::cout << "original dest (0:9): ";
        f_part();
        std::partition(std::begin(dest), std::end(dest), isOdd);
        std::cout << "[std::partition] dest (0:9): ";
        f_part();
        std::copy(std::begin(dest), std::end(dest), std::ostream_iterator<int>(std::cout, ", "));
        std::cout << std::endl;

        std::copy(std::begin(arr), std::end(arr), std::begin(dest));
        std::vector<int> odd(dest.size());
        std::vector<int> even(dest.size());
        std::partition_copy(std::begin(dest), std::end(dest), std::begin(odd), std::begin(even), isOdd);
        odd.shrink_to_fit();
        odd.shrink_to_fit();
        std::cout << "odd: ";
        std::copy(std::begin(odd), std::end(odd), std::ostream_iterator<int>(std::cout, ", "));
        std::cout << "\neven: ";
        std::copy(std::begin(even), std::end(even), std::ostream_iterator<int>(std::cout, ", "));
        std::cout << std::endl;

        std::copy(std::begin(arr), std::end(arr), std::begin(dest));
        auto it = std::partition_point(std::begin(dest), std::end(dest), isOdd);
        odd.clear();
        odd.assign(std::begin(dest), it);
        std::cout << "odd: ";
        std::copy(std::begin(odd), std::end(odd), std::ostream_iterator<int>(std::cout, ", "));
        std::cout << std::endl;

        // std::minmax, std::minmax_element
        std::prev_permutation(std::begin(dest), std::end(dest));
        std::cout << "[std::prev_permutation] (0:9): ";
        std::copy(std::begin(dest), std::end(dest), std::ostream_iterator<int>(std::cout, ", "));
        std::cout << std::endl;
        // auto v_minmax = std::minmax(dest);
        // std::cout << "[std::minmax] (" << v_minmax.first << ", " << v_minmax.second << ")" << std::endl;
        auto iter_minmax = std::minmax_element(std::begin(dest), std::end(dest));
        std::cout << "[std::minmax_element] (" << *iter_minmax.first << ", " << *iter_minmax.second << ")" << std::endl;

        /*** stl regular expressions ***/
        // std::regex
        bool        equals   = std::regex_match("subject", std::regex("(sub)(.*)"));
        std::string fnames[] = {"foo.txt", "bar.txt", "baz.dat", "zoidberg"};

        // simple regular expression matching
        std::regex txt_regex("[a-z]+\\.txt");
        std::cout << "simple regular expression match: \n";
        for (const auto& fname : fnames) {
            std::cout << fname << ": " << std::regex_match(fname, txt_regex) << std::endl;
        }

        // extraction of a sub-match
        std::regex  base_regex("([a-z]+)\\.txt");
        std::smatch base_match;
        std::cout << "extraction of a sub-match: \n";
        for (const auto& fname : fnames) {
            if (std::regex_match(fname, base_match, base_regex)) {
                if (base_match.size() == 2) {
                    std::ssub_match base_sub_match = base_match[1];
                    std::string     base           = base_sub_match.str();
                    std::cout << fname << " has a base of " << base << std::endl;
                }
            }
        }

        // std::ratio
        std::ratio<2, 3>                                   two_thirds;
        std::ratio_add<std::ratio<1, 2>, std::ratio<2, 6>> sum;
        std::cout << "2/3: " << two_thirds.num << " / " << two_thirds.den << "\n1/2+2/6: " << sum.num << " / "
                  << sum.den << std::endl;
        //<< "\n2/3 > (1/2+2/6): " << std::ratio_greater<two_thirds, sum>::value << std::endl;
    }

    // 022: thread
    struct Task {
        int         _index;
        std::string _name;

        Task() { std::cout << "Task()" << std::endl; }
        ~Task() { std::cout << "~Task()" << std::endl; }
        Task(Task& t)
        {
            std::cout << "Task(Task&)" << std::endl;
            _index = t._index;
            _name  = t._name;
        }
        Task(const Task& t) : Task(const_cast<Task&>(t)) { std::cout << "Task(const Task&)" << std::endl; }
        void operator()() const
        {
            std::cout << "Task::operator() [thread id: " << std::this_thread::get_id() << "]" << std::endl;
        }
        void operator()(int i) const { std::cout << "Task::operator()(int) [i: " << i << "]" << std::endl; }

        /* cannot use overload function here for std::thread, WHY???
         */
        void Do(int i) { std::cout << "Task::Do(int) - " << i << std::endl; }
        /*void Do(string str)
        {
            std::cout << "Task::Do(string) - " << str << " [thread id: " << std::this_thread::get_id() << "]" <<
        std::endl;
        }*/
    };

    // test std::condition_variable and std::unique_lock
    struct CV_Shipment {
        std::mutex              _mtx;
        std::condition_variable _cv;
        int                     _cargo = 0;

        void consume(int n)
        {
            for (int i = 0; i < n; ++i) {
                std::unique_lock<std::mutex> lk(_mtx);
                _cv.wait(lk, [&]() {
                    return _cargo != 0;
                });
                // start consuming...
                std::cout << _cargo << std::endl;
                _cargo = 0;
            }
        }
        void run()
        {
            int         num_cargos = 5;
            std::thread consumer_thread(&CV_Shipment::consume, this, num_cargos);

            // produce cargos...
            for (int i = 0; i < num_cargos; ++i) {
                // TODO: use a different cv notified by consume to avoid loop
                while (_cargo != 0) {
                    std::this_thread::yield();
                }
                std::unique_lock<std::mutex> lk(_mtx);
                _cargo = i + 1;
                // should notify after unlock
                _cv.notify_one();
            }

            consumer_thread.join();
        }
    };
    struct CV_consumer_producer {
        std::mutex              _mtx;
        std::condition_variable _produce, _consume;
        int                     _cargo = 0;

        void consumer()
        {
            std::unique_lock<std::mutex> lk(_mtx);
            _consume.wait(lk, [&]() {
                return _cargo != 0;
            });
            std::cout << "consume cargo " << _cargo << std::endl;
            _cargo = 0;
            _produce.notify_one();
        }
        void producer(int id)
        {
            std::unique_lock<std::mutex> lk(_mtx);
            _produce.wait(lk, [&]() {
                return _cargo == 0;
            });
            _cargo = id;
            std::cout << "product cargo " << _cargo << std::endl;
            _consume.notify_one();
        }

        void run()
        {
            int                      num = 5;
            std::vector<std::thread> consumers, producers;
            for (int i = 0; i < num; ++i) {
                consumers.push_back(std::thread(&CV_consumer_producer::consumer, this));
                producers.push_back(std::thread(&CV_consumer_producer::producer, this, i + 1));
            }
            for (int i = 0; i < num; ++i) {
                consumers[i].join();
                producers[i].join();
            }
        }
    };

    static void Run_thread()
    {
        /*
         * thead();
         * template<class Fn, class...Args> explicit thread(Fn&& fn, Args&&... args);
         * thread(const thread&) = delete;
         * thread(thread&&)
         * */

        int        test = 0;
        std::mutex mu;
        auto       print = [&](int i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            // std::lock_guard<std::mutex> locker(mu);
            std::cout << "print(int): " << i << std::endl;
        };
        auto f_add = [&]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::lock_guard<std::mutex> locker(mu);
            ++(++test);
            std::cout << "f_add: " << test << std::endl;
            print(test);
        };
        [[maybe_unused]] auto f_minus = [&]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::lock_guard<std::mutex> locker(mu);
            --test;
            std::cout << "f_minus: " << test << std::endl;
        };

        std::vector<std::thread> v_ths;
        for (int i = 0; i < 1; ++i) {
            v_ths.push_back(std::thread(f_add));
            // v_ths.push_back(std::thread(f_minus));
        }
        std::for_each(std::begin(v_ths), std::end(v_ths), [](std::thread& o) {
            o.join();
        });

        std::thread th0{Task()};
        Task        tk;
        // std::thread works as callable object, same as std::async, std::bind, std::call_once
        std::thread th1(&Task::Do, &tk, 10);    // tk.Do in the different thread
        std::thread th2(&Task::Do, tk, 11);     // copy_of_tk.Do in the different thread
        // std::thread th3(&Task::Do, std::ref(tk), 12); // compile error, cannot use std::ref(tk)!
        std::thread th4(std::ref(tk), 13);    // tk.()(int) in the different thread
        std::thread th5(tk, 14);              // copy_of_tk.()(int) in the different thread
        // std::thread th2{&Task::Do, &tk, "th2, char*"}; // not safe
        // std::thread th3{&Task::Do, &tk, string("th3, string")}; // safe

        th0.join();
        th1.join();
        th2.join();
        th4.join();
        th5.join();

        /*
        // std::condition_variable and std::unique_lock
        CV_Shipment cvObj;
        cvObj.run();

        CV_consumer_producer cv2;
        cv2.run();

        // std::async
        auto print_10 = [](char c, int ms) {
            for (int i = 0; i < 10; ++i)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(ms));
                std::cout << c << std::endl;
            }
        };
        std::cout << "With launch::async:\n";
        std::future<void> foo = std::async(std::launch::async, print_10, '*', 100);
        std::future<void> bar = std::async(std::launch::async, print_10, '@', 200);
        // async 'get' (wait for foo and bar to be ready)
        foo.get(); // will finish print_10 but won't block other threads
        bar.get();
        std::cout << "\n\n";

        std::cout << "With launch::deferred:\n";
        foo = std::async(std::launch::deferred, print_10, '*', 100);
        bar = std::async(std::launch::deferred, print_10, '@', 200);
        // deferred 'get' (perform the actual calls)
        foo.get(); // will finish print_10 and occupy resource until complete
        bar.get();
        std::cout << "\n\n";

        auto print_ret = [](char c) {
            for (int i = 1; i < 10; ++i)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                std::cout << c << std::endl;
            }
            Task t;
            t._index = 10;
            t._name = "this is a task";
            return t;
        };

        std::future<Task> ft1 = std::async(print_ret, '$');
        std::future<void> ft2 = std::async(print_10, '#', 50);
        auto v_ft1 = ft1.get();
        ft2.get();
        std::cout << v_ft1._name << std::endl;
        */
        /*
                // std:: promise
                std::promise<int> pro;
                std::future<int> fu = pro.get_future();
                auto print_pro = [](std::future<int>& f) {
                    std::cout << "print_pro starts..." << std::endl;
                    auto v = f.get();
                    std::cout << v << std::endl;
                    std::cout << "print_pro ends... " << std::endl;
                };

                std::thread th(print_pro, std::ref(fu));
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                pro.set_value(100);
                th.join();

                //
                auto print_future = [](std::future<char>& f) {
                    std::cout << "print_future starts..." << std::endl;
                    auto v = f.get();
                    for (int i = 1; i < 10; ++i)
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                        std::cout << v << std::endl;
                    }
                    Task t;
                    t._index = 10;
                    t._name = "this is a task";
                    std::cout << "print_future ends..." << std::endl;
                    return t;
                };
                std::promise<char> pro1;
                std::future<char> fu1 = pro1.get_future();
                std::future<Task> fu_1 = std::async(std::launch::async, print_future, std::ref(fu1));
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                pro1.set_value('%');
                auto t = fu_1.get();
                std::cout << t._name << std::endl;

                //
                auto print_ff = [](std::shared_future<Task&> sf) {
                    std::cout << "print_ff starts..." << std::endl;
                    // strange thing! If do auto t = sf.get(); then will do copy
                    // If do Task& t = sf.get(); then will do ref and no copy. Why auto
                    // doesn't work for ref automatically?
                    Task& t = sf.get();
                    t._index += 1;
                    std::cout << t._name << " " << t._index << std::endl;
                    std::cout << "print_ff ends..." << std::endl;
                };
                std::promise<Task&> pro2;
                std::future<Task&> fu2 = pro2.get_future();
                std::shared_future<Task&> sf = fu2.share();
                Task tk;
                tk._name = "task for promise";
                tk._index = 0;
                std::thread th1(print_ff, sf);
                pro2.set_value(tk);
                std::thread th2(print_ff, sf);
                std::thread th3(print_ff, sf);
                th1.join();
                th2.join();
                th3.join();
                // strange thing! If do auto t = sf.get(); then will do copy
                // If do Task& t = sf.get(); then will do ref and no copy. Why auto
                // doesn't work for ref automatically?
                auto tt = sf.get();
                std::cout << tt._name << " " << tt._index << std::endl;
                */
    }

    // 023: defalut move constructor
    struct MoveTest {
        MoveTest() { std::cout << "MoveTest()" << std::endl; }
        MoveTest(const MoveTest& o) { std::cout << "MoveTest(const MoveTest&)" << std::endl; }
        MoveTest& operator=(const MoveTest& o)
        {
            std::cout << "operator=(const MoveTest&)" << std::endl;
            return *this;
        }
        MoveTest(MoveTest&&) { std::cout << "MoveTest(MoveTest&&)" << std::endl; }
        MoveTest& operator=(MoveTest&&)
        {
            std::cout << "operator=(MoveTest&&)" << std::endl;
            return *this;
        }
        ~MoveTest() { std::cout << "~MoveTest()" << std::endl; }
        std::string _str;
    };

    class StringTable
    {
    public:
        StringTable() {}
        // StringTable(const StringTable&) {}
        // StringTable(StringTable&&) { std::cout << "StringTable(StringTable&&)" << std::endl; }
        MoveTest _oMove;
    };
    class StringTableEx
    {
    public:
        StringTableEx() {}
        ~StringTableEx() {}
        MoveTest _oMove;
    };
    static void Run_default_move()
    {
        auto getStringTable = []() {
            StringTable oMove;
            oMove._oMove._str = "this is StringTable should have defalut move";
            return std::move(oMove);
        };
        auto getStringTableEx = []() {
            StringTableEx oCopy;
            oCopy._oMove._str = "this is StringTableEx has no defalut move";
            return std::move(oCopy);
        };

        StringTable   o1 = getStringTable();
        StringTableEx o2 = getStringTableEx();
        std::cout << o1._oMove._str << " " << o2._oMove._str << std::endl;
    }

    // 024: std::ref, std::cref
    static void ref_cref(int n0, int& n1, int& n2, const int& n3)
    {
        std::cout << "In function: " << n0 << ' ' << n1 << ' ' << n2 << ' ' << n3 << '\n';
        ++n0;    // increments temporary n0 in the function object
        ++n1;    // increments the copy of n1 stored in the function object
        ++n2;    // increments the main()'s n2
        // ++n3; // compile error
    }

    static void Run_ref_cref()
    {
        int                   n0 = 0, n1 = 1, n2 = 2, n3 = 3;
        std::function<void()> bound_f = std::bind(ref_cref, n0, n1, std::ref(n2), std::cref(n3));
        n0                            = 9;
        n1                            = 10;
        n2                            = 11;
        n3                            = 12;

        // 9, 10, 11, 12
        std::cout << "Before function: " << n0 << ' ' << n1 << ' ' << n2 << ' ' << n3 << '\n';
        // 0, 1, 11, 12
        bound_f();

        // 9, 10, 12, 12
        std::cout << "After function 1: " << n0 << ' ' << n1 << ' ' << n2 << ' ' << n3 << '\n';
        // 0, 2, 12, 12
        bound_f();

        // 9, 10, 13, 12
        std::cout << "After function 2: " << n0 << ' ' << n1 << ' ' << n2 << ' ' << n3 << '\n';

        // A std::reference_wrapper is a C++ object able to hold a reference. It can be used to
        // create an array of references which was not possible with T&!
        // std::ref is a standard function that returns a std::reference_wrapper on its argument.
        // std::cref returns std::reference_wrapper to a const reference.

        // You cannot do int& arr[] = {n0, n1, n2, n3}; error: declaration of 'arr' as array of references
        // But it works by using std::reference_wrapper as below
        std::reference_wrapper<int> arr[] = {n0, n1, n2, n3};

        n1 = 22;

        std::cout << &n0 << " " << n0 << std::endl;
        std::cout << &n1 << " " << n1 << std::endl;
        std::cout << &n2 << " " << n2 << std::endl;
        std::cout << &n3 << " " << n3 << std::endl;

        for (auto& a : arr) {
            // a.get() is int& which is reference of original variable, (int&)a has same effect.
            // &a is not address of origianl variable but address of std::reference_wrapper object.
            std::cout << &a << " " << &a.get() << " " << a << std::endl;
        }
    }

    // 025: std::chrono
    static unsigned long long get_nano_seconds_since_midnight()
    {
        // current time
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();

        // get midnight time_point
        time_t tnow   = std::chrono::system_clock::to_time_t(now);
        tm*    date   = std::localtime(&tnow);
        date->tm_hour = 0;
        date->tm_min  = 0;
        date->tm_sec  = 0;
        auto midnight = std::chrono::system_clock::from_time_t(std::mktime(date));

        // number of nano seconds between midnight and now
        // The same technique can be used for time since epoch
        return std::chrono::duration_cast<std::chrono::nanoseconds>(now - midnight).count();
    }

    static std::string get_localtime_now()
    {
        // current time
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();

        // get datetime
        time_t dt   = std::chrono::system_clock::to_time_t(now);
        tm*    date = std::localtime(&dt);
        // tm*    date = std::gmtime(&dt); // UTC time

        // convert time_t(datetime) to time_point, and then get nano seconds
        auto dt_point = std::chrono::system_clock::from_time_t(dt);
        auto nano_sec = std::chrono::duration_cast<std::chrono::nanoseconds>(now - dt_point).count();

        char time[128] = {0};
        sprintf(
            time, "%d-%.2d-%.2d %.2d:%.2d:%.2d.%09llu", date->tm_year + 1900, date->tm_mon + 1, date->tm_mday,
            date->tm_hour, date->tm_min, date->tm_sec, nano_sec);
        // strftime(time, sizeof(buf), "%Y-%m-%d %X", date); // use tm/time_t but only to seconds
        return std::string(time);
    }

    static void Run_chrono()
    {
        /*** stl chrono ***/
        // std::chrono
        auto start = std::chrono::high_resolution_clock::now();
        sleep(2);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

        auto st = std::chrono::system_clock::now();
        sleep(2);
        auto ed              = std::chrono::system_clock::now();
        auto elapsed_seconds = ed - st;
        auto end_time        = std::chrono::system_clock::to_time_t(ed);
        std::cout << "finished computation at " << std::ctime(&end_time)
                  << "\nelapsed time: " << elapsed_seconds.count() << std::endl;

        std::cout << "localtime: " << get_localtime_now() << std::endl;
        std::cout << "nano seconds since midnight: " << get_nano_seconds_since_midnight() << std::endl;
    }
};

