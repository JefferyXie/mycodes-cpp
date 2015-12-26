#ifndef C11_H
#define C11_H

#include "../header.h"

/******************************************************************************
 * A few valuable resource:
 * http://stackoverflow.com/questions/9299101/what-c-idioms-are-deprecated-in-c11
 * http://www.cnblogs.com/catch/p/3507883.html
 *
 *
 * ***************************************************************************/

namespace legacy
{
    template<class T> auto begin(T& t) -> decltype(t.Legacy_begin())
    {
        return t.Legacy_begin();
    }
    template<class T> auto end(T& t) -> decltype(t.Legacy_end())
    {
        return t.Legacy_end();
    }
    // classes definition
    template<class T> class Container_vector
    {
    private:
        vector<T> m_vec;
    public:
        Container_vector(const vector<T>& src)
        {
            copy(src.begin(), src.end(), std::back_inserter(m_vec));
        }

        auto Legacy_begin() -> decltype(m_vec.begin())
        {
            return m_vec.begin();
        }
        typename vector<T>::iterator Legacy_end() // need typename, otherwise, compile error
        {
            return m_vec.end();
        }
    };
    template<class T> class Container_map
    {
    private:
        map<int, T> m_map;
    public:
        Container_map(const map<int, T>& src)
        {
            copy(src.begin(), src.end(), std::inserter(m_map, m_map.end()));
        }

        auto Legacy_begin() -> decltype(m_map.begin())
        {
            return m_map.begin();
        }
        auto Legacy_end() -> decltype(m_map.begin())
        {
            return m_map.end();
        }
    };
}

class C11
{
public:

    // 001: auto
    template <class T1, class T2> static
            auto compose(T1 t1, T2 t2) -> decltype(t1 + t2)
    {
        return t1 + t2;
    }

    // decltype: https://msdn.microsoft.com/en-us/library/dd537655.aspx
    static auto fo(int n) -> decltype(compose(n, n)) // decltype(n)
    {
        cout << n << endl;
        return 10;
    }

    static void Run_auto()
    {
        auto i = 42;
        auto l = 42LL;
        cout << compose(i, l) << endl;
        std::map<string, int> oMap = {
            { "1st", 1 },
            { "2nd", 2 },
            { "3rd", 3 },
        };
        oMap.insert(make_pair<string, int>("4th", 4));
        for (auto itr = oMap.begin(); itr != oMap.end(); ++itr)
            cout << itr->first << "  " << itr->second << endl;

        for (auto itr = std::begin(oMap); itr != std::end(oMap); ++itr)
            cout << itr->first << "  " << itr->second << endl;
    }

    // 002: nullptr, nullptr_t
    static void Run_nullptr()
    {
        auto fun_pint = [] (int* p) { cout << "fun_pInt(int*):" << *p << endl; };
        auto fun_pshared_ptr = [] (shared_ptr<int> p) { cout << "fun_pshared_ptr(shared_ptr<int>):" << *p << endl; };
        auto fun_nullptr_t = [] (nullptr_t) { cout << "fun_nullptr_t(nullptr_t):nullptr as input only" << endl; };

        int* p1 = NULL;
        int* p2 = nullptr;
        //shared_ptr<int> p3 = new int(10);

        //bool b = nullptr; // nullptr > bool but cannot nullptr > int
        //long i = static_cast<long>(nullptr); // compile error, static_cast doesn't work
        auto l = reinterpret_cast<long>(nullptr); // reinterpret_cast works
        fun_pint(p1);
        //fun_pshared_ptr(p3);
        auto p4 = nullptr;
        fun_nullptr_t(p4);
    }

    // 003: range-based for loops
    static void Run_loops()
    {
        // will call initializer_list<int> for {...}
        for (auto x : {4, 2, 6, 9, 0})
            cout << x << ", ";
        cout << endl;

        vector<int> vec1 = {0, 1, 2, 3};
        vector<int> vec2 = {3, 4, 2};
        map<string, void*> oMap = {
            {"1st", &vec1},
            {"2nd", &vec2},
        };
        for (const auto& kp : oMap)
        {
            cout << kp.first << " {";
            for (auto& v : *(static_cast<vector<int>*>(kp.second)))
                cout << v << "  ";
            cout << "}" << endl;
        }

        string arr[] = {"1st str", "2nd str", "3rd str"};
        for (auto& str : arr)
            cout << str << "  ";
        cout << endl;

        vector<string> vecStr = {"1st Str", "2nd Str", "3rd Str"};
        copy(begin(vecStr), end(vecStr), ostream_iterator<string>(std::cout, ", "));
        cout << endl;

        legacy::Container_vector<string> con_vec(vecStr);
        copy(begin(con_vec), end(con_vec), ostream_iterator<string>(std::cout, ", "));
        cout << endl;

        map<int, string> mapStr = {
            {1, "1st pair"},
            {2, "2nd pair"},
            {3, "3rd pair"},
        };
        legacy::Container_map<string> con_map(mapStr);
        for (auto& vp : con_map)
            cout << vp.first << " " << vp.second << ", ";
        cout << endl;
    }

    // 004: override and final
    class B
    {
    public:
        B()
        {
            cout << "B::B()" << endl;
        }
        virtual ~B()
        {
            cout << "B::~B()" << endl;
        }
        virtual void f(int) const { cout << "B::f(int) const" << endl; }
        virtual void f(int) { cout << "B::f(int)" << endl; }
        virtual void f(B*) { cout << "B::f(B*)" << endl; }

        void f() { cout << "B::f()" << endl; }
        // error: cannot overload only by return type
        //int f() { cout << "int B::f()" << endl; }
    };
    class D : public B
    {
    public:
        virtual void f(int) const override { cout << "D::f(int) const" << endl; }
        // child class cannot override f(B*) anymore because of keyword "final"
        virtual void f(B*) final { cout << "D::f(B*)" << endl; }
        virtual void f(D*) { cout << "D::f(D*)" << endl; }

        // no error: this is in different scope than B::f()
        int f() { cout << "int D::f()" << endl; return 0; }
    };
    class G : D
    {
    public:
        void f() { cout << "G::f()" << endl; }
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

        shared_ptr<D> s_ptr(new D());
        s_ptr.get()->f();
        B* pd2 = s_ptr.get();
        pd2->f(pd2);
        pd2->f();
    }

    // 005: strongly-typed enums
    enum RGB_98 { RED, GREEN, BLUE };
    // error since this is 98 style and duplicate names in the same scope is prevented
    //enum RGB_98s { RED, GREEN, BLUE };
    enum class RGB_11 { RED, GREEN, BLUE, NONE };
    enum class RGB_11s { RED, GREEN, BLUE };
    enum class RGB_char : char { RED = 1, GREEN, BLUE };
    enum class RGB_int : int64_t { RED = 1, GREEN, BLUE };
    static void f_enum(int) { cout << "f_enum(int)" << endl; }
    static void f_enum(char) { cout << "f_enum(char)" << endl; }
    static void f_enum(RGB_98) { cout << "f_enum(RGB_98)" << endl; }
    static void f_enum(RGB_11) { cout << "f_enum(RGB_11)" << endl; }
    static void f_enum(RGB_char) { cout << "f_enum(RGB_char)" << endl; }
    static void Run_enum()
    {
        f_enum(RED); // if has f_enum(RGB_98) defined, won't call f_enum(int)
        f_enum((int)GREEN);
        f_enum(RGB_98::BLUE);
        f_enum(RGB_11::RED);
        f_enum(RGB_char::BLUE);
        // error since no corresponding function
        //f_enum(RGB_int::GREEN);

        cout << "sizeof(RGB_98::RED): " << sizeof(RGB_98::RED) << endl;
        cout << "sizeof(RGB_11::RED): " << sizeof(RGB_11::RED) << endl;
        cout << "sizeof(RGB_char::RED): " << sizeof(RGB_char::RED) << endl;
        cout << "sizeof(RGB_int::RED): " << sizeof(RGB_int::RED) << endl;

        auto i = (int)RGB_11::RED;
        auto j = (int)RGB_char::BLUE;
        auto k = (char)RGB_char::GREEN;
        cout << i << " " << j << " " << k << endl;
    }

    // 006: smart pointers
    template<class T> static void f_smart(T* p)
    {
        cout << "f_smart(T*): " << (nullptr == p ? "nullptr" : "not null") << endl;
    }
    struct Fo
    {
        int rnd = rand();
        Fo() { cout << "Fo(), @rnd:" << rnd << endl; }
        ~Fo() { cout << "~Fo(), @rnd:" << rnd << endl; }
    };
    struct Fo_D
    {
        int count = 0;
        void goo() { cout << "Fo_D::goo()..." << endl; }
        void operator()(Fo* p)
        {
            cout << "Fo_D::operator(), Fo.rnd:" << p->rnd << ", [count]:" << ++count << endl;
            delete p;
        }
    };
    struct B_share;
    struct A_share
    {
        ~A_share() { cout << "~A_share()" << endl; }
        // if we use shared_ptr here, both A_share and B_share objects wont' destory
        // the solution is to use weak_ptr. Use in either A_share or B_share.
        //shared_ptr<B_share> b;
        weak_ptr<B_share> b;
    };
    struct B_share
    {
        ~B_share() { cout << "~B_share()" << endl; }
        shared_ptr<A_share> a;
    };

    static void Run_smartpointers()
    {
        // these codes explain how weak_ptr work with shared_ptr
        {
            shared_ptr<A_share> p_a(new A_share());
            shared_ptr<B_share> p_b(new B_share());
            p_a.get()->b = p_b;
            p_b.get()->a = p_a;
        }

        unique_ptr<Fo, Fo_D> up(new Fo(), Fo_D());
        auto del = up.get_deleter();
        del.goo();

        Fo_D fd;
        unique_ptr<Fo, Fo_D> up1(new Fo(), up.get_deleter());
        unique_ptr<Fo, Fo_D&> down(new Fo(), fd);
        unique_ptr<Fo, Fo_D&> down1(new Fo(), down.get_deleter());

        cout << "reset up..."; up.reset(new Fo());
        cout << "reset up1..."; up1.reset(new Fo());
        cout << "reset down..."; down.reset(new Fo());
        cout << "reset down1..."; down1.reset(new Fo());

        cout << "call down1 deleter..."; down1.get_deleter()(new Fo());

        auto fun_del = [](int* p) { cout << "fun_del, *p:" << *p << endl; delete p; p = nullptr; };
        unique_ptr<int, decltype(fun_del)> u0(new int(10), fun_del);
        auto f_del = u0.get_deleter();

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

        // although the memory cost is same, make_shared performs better since it only allocate memory once
        // while shared_ptr will do twice: one is the 'new' and the other happens inside ref control
        shared_ptr<B> p5(new B());
        auto p6 = make_shared<B>();
        f_smart(p6.get());

        weak_ptr<B> wp = p5;
        {
            auto sp = wp.lock();
            f_smart(sp.get());
        }
        p5.reset();

        if (wp.expired())
            cout << "wp is expired" << endl;
        */
    }

    // 007: lambdas
    static void Run_lambdas()
    {
        int arr[] = {3, 4, -2, 8, -5};
        std::sort(arr, arr+sizeof(arr)/sizeof(arr[0]), [](int a, int b) { return abs(a) > abs(b); });

        int num_positive = 0;
        int num_negative = 0;
        // error: for_each(arr, arr+sizeof(arr)/sizeof(arr[0]), [](int a) { if (a > 0) num_positive++; });
        for_each(arr, arr+sizeof(arr)/sizeof(arr[0]), [&](int a) { if (a > 0) num_positive++; });
        for_each(arr, arr+sizeof(arr)/sizeof(arr[0]), [&num_positive](int a) { if (a > 0) num_positive++; });
        // error: read-only! for_each(arr, arr+sizeof(arr)/sizeof(arr[0]), [=](int a) { if (a > 0) num_positive++; });

        num_positive = 0;
        num_negative = 0;
        for_each(arr, arr+sizeof(arr)/sizeof(arr[0]), [&, &num_positive](int a) { a > 0 ? ++num_positive : ++num_negative; });
        for_each(arr, arr+sizeof(arr)/sizeof(arr[0]), [&num_positive, &num_negative](int a) { a > 0 ? ++num_positive : ++num_negative; });
        for_each(arr, arr+sizeof(arr)/sizeof(arr[0]), [&](int a) { a > 0 ? ++num_positive : ++num_negative; });

        int i = 1, j = 1;
        vector<int> vec_int(5, 1);
        generate_n(vec_int.begin()+2, vec_int.size()-2, [&] {
            int n = i+j;
            i = j;
            j = n;
            return n;
        });

        auto fo = [=, &j] { return i+j; };
        auto go = [&, j] { return i+j; };
        auto ho = [&] { return i+j; };
        auto jo = [=] { return i+j; };
        auto ko = [=, j] { return i+j; };
        i = j = 10;
        cout << fo() << endl;
        cout << go() << endl;
        cout << ho() << endl;
        cout << jo() << endl;
        cout << ko() << endl;

        int t = 5;
        // higher order lambda expression, embedded lambda
        auto addtwo = [](int x) -> function<int(int)> {
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
                return x+y;
            };
        };
        auto higherorder = [](const function<int(int)>& ff, int k) {
            return ff(k) * 2;
        };
        cout << higherorder(addtwo(5), 3) << endl;
        cout << higherorder(addtwo(t), 3) << endl;

        cin >> num_negative;
    }

    // 008: non-member begin() and end()
    template<class T> static void bar(T begin, T end)
    {
        for_each(begin, end, [](int n) { cout << n << endl; });
        auto is_odd = [](int n) { return n%2 == 1; };
        auto pos = find_if(begin, end, is_odd);
        if (pos != end) cout << *pos << endl;
    }
    template<class T> static void foo(T& t)
    {
        bar(begin(t), end(t));
    }
    // below foo is not necessary since above foo has T reference as para
    // not sure why template has "size_t N" character?
    template<class T, size_t N> static void foo(T (&arr)[N])
    {
        bar(begin(arr), end(arr));
    }

    static void Run_beginend()
    {
        int arr[] = {3, 5, 2, 1};
        foo(arr);

        vector<int> v = {3, 2, 6, 7, 8};
        foo(v);
    }

    // 009: static_assert and type traits
    // static_assert is compile-time check
    template<class T, size_t N>
    class VV
    {
        static_assert(N < 3, "size is too big");
    };
    template<class T1, class T2>
    static auto add(T1 t1, T2 t2) -> decltype(t1+t2)
    {
        //static_assert(typeid(T1) == typeid(T2), "t1 and t2 are different types");
        static_assert(is_integral<T1>::value, "t1 should be integral");
        static_assert(is_integral<T2>::value, "t2 should be integral");
        return t1 + t2;
    }
    static void Run_static_assert()
    {
        VV<int, 2> v1;
        // compile error from static_assert
        //VV<int, 5> v2;

        add(1, 2);
        // compile error
        //add(1, 5.1);
        //add("ab", 2);
        //add(2, "ab");
    }

    // 010: move semantics
    class Buffer
    {
    private:
        string m_name;
        unique_ptr<int> m_buffer_int;
        vector<B> m_buffer_B;
    public:
        Buffer() { cout << "Buffer()" << endl; }
        Buffer(const string& name) : m_name(name) { cout << "Buffer(const string&)" << endl; }
        Buffer(const Buffer& buf) : m_name(buf.m_name)
        {
            cout << "Buffer(const Buffer&)" << endl;
            if (nullptr != buf.m_buffer_int.get())
                m_buffer_int = make_unique<int>(*buf.m_buffer_int.get());
                //m_buffer_int = move(unique_ptr<int>(new int(*buf.m_buffer_int.get())));
            copy(buf.m_buffer_B.begin(), buf.m_buffer_B.end(), back_inserter(m_buffer_B));
        }
        Buffer& operator=(const Buffer& buf)
        {
            cout << "operator=(const Buffer&)" << endl;
            if (this != &buf)
            {
                m_name = buf.m_name;
                m_buffer_int.reset(); // equal to m_buffer_int = nullptr;
                if (buf.m_buffer_int.get() != nullptr)
                {
                    m_buffer_int = make_unique<int>(*buf.m_buffer_int.get());
                    //m_buffer_int = move(unique_ptr<int>(new int(*buf.m_buffer_int.get())));
                }
                copy(begin(buf.m_buffer_B), end(buf.m_buffer_B), back_inserter(m_buffer_B));
            }
            return *this;
        }
        // move constructor
        Buffer(Buffer&& temp) = default;
        /*{
            cout << "Buffer(Buffer&&)" << endl;
            m_name = temp.m_name;
            //m_name = move(temp.m_name);
            m_buffer_int = move(temp.m_buffer_int);
            m_buffer_B = move(temp.m_buffer_B);
            //m_buffer_B.swap(temp.m_buffer_B);
        }*/
        // move assignment
        Buffer& operator=(Buffer&& temp) = default;
        /*{
            cout << "operator=(Buffer&&)" << endl;
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
            m_buffer_int = make_unique<int>(100);
            //m_buffer_int = move(unique_ptr<int>(new int(100)));
        }
    };
    static void Run_move()
    {
        auto createBuffer = [](const string& name) {
            // use "move" will force to use move constructor
            return move(Buffer(name));
            // return object directly won't call move constructor or
            // copy constructor because of "copy elision"
            // http://en.cppreference.com/w/cpp/language/copy_elision
            // http://blog.csdn.net/zwvista/article/details/6845020
            //return Buffer(name);
        };
        Buffer buf1;
        Buffer buf2("2nd buf");
        auto buf3 = buf2;
        auto buf4 = createBuffer("4th buf");
        buf1 = createBuffer("overwrite buf1");

        // use "move" will force to use move constructor
        auto buf5 = move(buf4);
    }

    // 011: forward
    static void overloaded (const int& x) {std::cout << "[lvalue]: " << x << endl;}
    static void overloaded (int&& x) {std::cout << "[rvalue]: " << x << endl;}

    // function template taking rvalue reference to deduced type:
    template <class T> static void fn (T&& x) {
      overloaded (x);                   // always an lvalue, why fn(0) also call lvalue???
      overloaded (std::forward<T>(x));  // rvalue if argument is rvalue
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
        fon(10);

        int i = 2;
        int& j = i;
        overloaded(3);
        overloaded(i);
        overloaded(forward<int>(3));
        // Question: forward is supposed to not convert i to rvalue
        // since i is lvalue, however it DOES. Why? Is this because
        // forward is only meaningful at function template when type
        // is uncerntain?
        // Answer: here should use forward<int&>(i) since i is treated as reference!
        // http://stackoverflow.com/questions/17539282/stdforward-test-in-template-and-non-template-function
        // http://blog.csdn.net/zwvista/article/details/6848582
        overloaded(forward<int>(i));
        overloaded(move(i));

        auto ffn = [](string&& v) {
            //auto m = move(v); cout << m << " " << v << endl;
            cout << "ffn(string&&): " << v << endl;
        };
        string s = "10";
        //ffn(s);
        ffn(forward<string>(s));
        ffn(move(s));
        ffn("abc");

        int a = 2;
        std::cout << "calling fn with lvalue: " << endl;
        fn(a);
        std::cout << '\n';

        std::cout << "calling fn with rvalue: " << endl;
        fn(5);
        std::cout << '\n';

        const int& ref = a;
        int&& b = 10; // b is lvalue, int type
        fn(ref); // treat ref as lvalue
        fn(b); // treat b as lvalue
        auto gn = [](int&& v) {
            fn(v);
            fn(forward<int>(v));
            fn(move(v));
        };
        gn(5);
        gn([](){return 11;}());
    }

    // 012: in-class member initializer
    class InClass
    {
        int a = 10;
        string s = "in class init.";
        unique_ptr<int> p = make_unique<int>(15);
    };
    static void Run_inclass_initializer()
    {
        int i = 10;
        InClass ic;
        cin >> i;
    }

    // 013: delegating constructor
    class Delegating
    {
        int a = 11;
    public:
        Delegating(int m) : a(m)
        {
            cout << "Delegating(int)" << endl;
        }
        Delegating() : Delegating(5) // delegating constructor
        {
            cout << "Delegating()" << endl;
        }
        Delegating(string s) : Delegating(stoi(s)) // delegating constructor
        {
            cout << "Delegating(string)" << endl;
        }
    };

    static void Run_delegating_constructor()
    {
        Delegating del1;
        Delegating del2(2);
        Delegating del3("a");
        int i = 10;
        cin >> i;
    }

    // 014: type traits
    struct A {};
    struct C { virtual void f() {} };
    static void Run_type_traits()
    {
        cout << "has_virtual_destructor<int>: " << has_virtual_destructor<int>::value << endl;
        cout << "is_polymorphic<int>: " << is_polymorphic<int>::value << endl;
        cout << "is_polymorphic<A>: " << is_polymorphic<A>::value << endl;
        cout << "is_polymorphic<C>: " << is_polymorphic<C>::value << endl;
        cout << "is_abstract<C>: " << is_abstract<C>::value << endl;
        typedef int mytype[][10][20];
        cout << "0 dim: " << extent<mytype, 0>::value << endl;
        cout << "1 dim: " << extent<mytype, 1>::value << endl;
        cout << "2 dim: " << extent<mytype, 2>::value << endl;

        cout << "is_void(A): " << is_void<A>::value << endl;
        cout << "is_class(A): " << is_class<A>::value << endl;

        int a;
        const int b = 5;
        int c = 10;
        cout << "is_const<int>: " << is_const<int>::value << endl;
        cout << "is_const<const int>: " << is_const<const int>::value << endl;
        cout << "is_const<const int&>: " << is_const<const int&>::value << endl;
        cout << "is_const<decltype(a)>: " << is_const<decltype(a)>::value << endl;
        cout << "is_const<decltype(b)>: " << is_const<decltype(b)>::value << endl;
        cout << "is_const<decltype(c)>: " << is_const<decltype(c)>::value << endl;
    }

    // 015: bind function
    static void f(int n1, int n2, int n3, const int& n4, int n5)
    {
        cout << n1 << ", " << n2 << ", " << n3 << ", "
             << n4 << ", " << n5 << "." << endl;
    }
    struct Foo
    {
        int data = 10;
        void print_sum(int n1, int n2)
        {
            cout << n1 + n2 << endl;
        }
    };

    static void Run_bind()
    {
        using namespace std::placeholders;

        // bind with std::placeholders and reference (cref)
        int n = 5;
        auto f1 = bind(f, _2, _1, 8, cref(n), n);
        n = 9;
        f1(1, 2, 100);

        // bind with class's member function
        Foo foo;
        auto f2 = bind(&Foo::print_sum, &foo, 20, _1);
        f2(3);
        auto f3 = bind(&Foo::print_sum, _2, _1, 3);
        foo.data = 15;
        f3(1, foo);
        auto f4 = bind(&Foo::print_sum, &foo, cref(foo.data), 2);
        foo.data = 20;
        f4();

        // bind with random uniform distribution
        default_random_engine rndEng;
        uniform_int_distribution<> uniDist(0, 10);
        function<int()> rnd = bind(uniDist, rndEng);
        for (int i = 0; i < 10; ++i)
            cout << rnd() << ", ";
        cout << endl;

        linear_congruential_engine<uint64_t, 10000, 200, 30000> generator(10000);
        int rn = uniDist(generator);
        auto dice = bind(uniDist, generator);
        cout << dice() << ", " << dice() << ", " << dice() << endl;

        auto mean = uniDist(rndEng);

        // bind with normal distribution
        random_device rndDev;
        mt19937 mtEng(rndDev());
        normal_distribution<> norm_dist(mean, 2);

        map<int, int> hist;
        for (int i = 0; i < 100; ++i)
            ++hist[round(norm_dist(mtEng))];

        cout << "Normal distribution around " << mean << ":" << endl;
        for (auto p : hist)
        {
            cout << std::fixed << std::setprecision(1) << std::setw(2)
                    << p.first << ' ' << std::string(p.second, '*') << endl;
        }

        // bind with std algorithm
        int arr[] = {3, 2, 5, 1, 0};
        /*
        transform(arr, arr+sizeof(arr)/sizeof(arr[0]), arr, bind(std::pow, _1, 2));
        for_each(arr, arr+sizeof(arr)/sizeof(arr[0]), bind(std::sqrt, _1));
        for_each(arr, arr+sizeof(arr)/sizeof(arr[0]), std::sqrt);
        */

        auto output = [](ostream& os, int x) {
            os << x << endl;
            //return os;
        };
        for_each(arr, arr+sizeof(arr)/sizeof(arr[0]), bind(output, std::ref(cout), _1));
    }

    // 016: std::tuple, std::tie
    static void Run_tuple()
    {
        tuple<int, double, string> t0(2, 4.1, "t0 tuple");
        auto s = get<2>(t0);
        get<0>(t0) = 5;
        int m;
        double db;
        tie(m, db, s) = t0;

        tuple<int, char> t1;
        t1 = make_tuple(11, 'm');
        tie(m, ignore) = t1;

        cout << "t0 size: " << tuple_size<decltype(t0)>::value << endl;
        cout << "t1 size: " << tuple_size<decltype(t1)>::value << endl;

        cout << is_const<tuple_element<0, decltype(t0)>::type>::value << endl;

        int a,b;
        std::tuple<int&,int&> test(a,b);
        std::get<0>(test) = 1;
        std::get<1>(test) = 2;
        std::cout << a << ":" << b << std::endl;

        int &ar=a;
        int &br=b;
        auto test2 = std::forward_as_tuple(ar, br);
        //auto test3 = std::make_tuple(ar,br); // this cannot make tuple of int& reference
        std::get<0>(test2) = -1;
        std::get<1>(test2) = -2;
        std::cout << a << ":" << b << std::endl;
    }

    // 017: initializer_list
    class E
    {
    public:
        E() {}
        E(vector<int>) {}
        E(int) {}
        E(B) {}
        //E(const B&) {}
        //E(B&&) {}
        void f() {}
        //E(initializer_list<int>) {}
        //E(std::initializer_list<int double>) {}
    };

    static void Run_initializer_list()
    {
        vector<int> v0(3); // 3 elements: 0
        vector<int> v1{3}; // 1 element: 3
        map<int, string> mp;
        mp = {
            {4, "1st"},
            {2, "2nd"},
            {5, "3rd"},
            {1, "4th"},
        }; // call initializer_list
        for_each(begin(mp), end(mp), [](pair<int, string> p) {
            cout << p.first << ", " << p.second << "; ";
        });
        cout << endl;
        auto fun = [](vector<int> v) -> vector<int> {
            for (auto o : v) cout << o << ",";
            cout << endl;
            return {2, 1, 8}; // call initializer_list
        };
        auto arr = fun({5, 6}); // call initializer list
        for (auto s : {"1st", "2nd"}) cout << s << ",";
        cout << endl;
        //for (pair<int, string> p : {{2, "1st"}, {1, "2nd"}}) cout << p.first << ", " << p.second << "; ";
        //cout << endl;

        E e0(B()); // most vexing parse! declares a function named e0
        B b;
        E e2(b); // call E(B) ctor
        e2.f();
        E e1{B()}; // call E(B)
        //e0.f();
        e1.f();
        E e3({1, 2});
        //E e3{1, 2}; // need E(int, int) ctor or declare E(initizlier_list<int>)
        E e4{}; // need default ctor E(). NOTE: if there's any non-default constructor created,
                // compiler won't generate default ctor until you explictly point it out.
    }
    // 018: default & delete
    static void isLucky(int num) {}
    static void isLucky(char) = delete;
    static void isLucky(bool) = delete;
    static void isLucky(double) = delete;
    class F
    {
    public:
        F() = default;
        F(const F&) = delete;
        F& operator=(const F&) = delete;
        virtual ~F() = default;
    };

    static void Run_default_delete()
    {
        Buffer buf1;
        Buffer buf2("2nd buf");
        auto buf3 = Buffer("3rd");
        auto buf5 = move(Buffer("5th"));

        F f1;
        //auto f2 = f1; // error, no allowed to use 'delete' function
        //F f3(f1); // error, same reason as above

        isLucky(10);
        // below calls are not allowed. If they are not "delete", they will be
        // converted to use isLucky(int)
        //isLucky(1.0);
        //isLucky(true);
        //isLucky('b');
    }

    // 019: constexpr
    static constexpr int Fib(int n)
    {
        return n <= 2 ? 1 : (Fib(n-1) + Fib(n-2));
    }
    static constexpr int Pow(int base, int exp)
    {
        return exp == 0 ? 1 : base * pow(base, exp-1);
        // below codes are supposed to be ok under c++14
        /*auto ret = 1;
        for (int i = 0; i < exp; ++i) ret *= base;
        return ret;*/
    }
    class Point
    {
    public:
        constexpr Point(double xVal = 0, double yVal = 0) : x(xVal), y(yVal)
        {
            //cout << "constexpr Point(double, double)" << endl;
        }
        constexpr double GetX() const { return x; }
        constexpr double GetY() const { return y; }
        void SetX(double xVal) { x = xVal; }
        void SetY(double yVal) { y = yVal; }
    private:
        double x = 0.0;
        double y = 0.0;
    };
    static constexpr Point MidPoint(const Point& p1, const Point& p2)
    {
        return {
            (p1.GetX()+p2.GetX())/2,
            (p1.GetY()+p2.GetY())/2
        };
    }

    static void Run_constexpr()
    {
        cout << Fib(5) << endl; // compile time
        int i = 3;
        cout << Fib(i) << endl; // run time
        constexpr int j = 3;
        cout << Fib(j) << endl; // compile time

        auto m = Fib(3); // run time
        constexpr auto n = Fib(3); // compile time

        array<int, j> arr;
        arr = {2, 1, 5};
        arr = {{4, 3, 6}};

        array<int, Fib(3)> arr0; // compile time
        array<int, Pow(2, Fib(3))> arr1; // compile time

        cout << Pow(i, j) << endl; // compile time
        cout << Pow(2, j) << endl; // compile time

        constexpr Point p1(11.0, 5.2);
        constexpr Point p2{4.2, 3.2};
        constexpr Point p3{3, 7};
        constexpr auto mid = MidPoint(p1, p2);
    }

    // 020: variadic templates
    static void myPrintf(const char* s)
    {
        while (*s)
        {
            if (*s == '%')
            {
                if (*(s+1) == '%')
                    ++s;
                else
                    throw std::runtime_error("invalid format string: missing arguments");
            }
            cout << *s++;
        }
    }
    template<class T, class... Args> static void myPrintf(const char* s, T value, Args... args)
    {
        while (*s)
        {
            if (*s == '%')
            {
                if (*(s+1) == '%')
                    ++s;
                else
                {
                    cout << value;
                    myPrintf(s+1, args...);
                    return;
                }
            }
            cout << *s++;
        }
        throw std::logic_error("extra arguments provided to myPrintf");
    }

    template<class... T> static void dummy_wrapper(T...)
    {
        cout << "dummy_wrapper(T...)" << endl;
    }
    template<class T> static T unpacker(const T t)
    {
        cout << "[" << t << "]";
        return t;
    }
    template<class T, class... Args> static void dummy_wrapper(T t, Args... args)
    {
        unpacker(t);
        dummy_wrapper(args...);
    }

    static int maximum(int n)
    {
        return n;
    }
    template<class... Args> static int maximum(int n, Args... args)
    {
        return max(n, maximum(args...));
    }

    static void Run_variadic()
    {
        // array, vector
        auto c_style_f = [](int* x) {
            cout << *x << endl;
        };
        array<string, 2> arr_str{"1st", "2nd"};
        array<int, 2> arr_int{0};
        vector<int> v_int = {2, 1, 3};
        c_style_f(arr_int.data());
        c_style_f(v_int.data());
        v_int.reserve(10); // allocate resource but not init values, capacity() changes but size() has no change
        v_int.resize(15); // allocate resource and init with default values, size() changes
        v_int.reserve(5); // no change if new space is smaller than current space
        v_int.resize(2); // keep size() equal to required number by removing objects in the end
        v_int.shrink_to_fit(); // size() doesn't change, capacity() will be reduced to same as size()
        v_int.clear(); // remove all values, size()==0 while capacity() has no change

        vector<Point> v_point;
        v_point.emplace(begin(v_point), 3.1, 2.2);
        v_point.emplace_back(1.2, 2.3);

        // stoi, stol, stoll, stoul, stoull, stof, stod, stold, to_string, to_wstring
        size_t sz = 0;
        string str = "0123456789A";
        auto s_i0 = stoi(str);
        auto s_i1 = stoi(str, &sz);
        //auto s_i2 = stoi(str, &sz, 16); out of range
        auto s_ll0 = stoll(str);
        auto s_ll1 = stoll(str, &sz, 16);
        auto s_ul0 = stoul(str);
        auto s_ul1 = stoul(str, nullptr, 16);
        auto s_ull = stoull(str, &sz, 16);
        auto s_f0 = stof(str);
        auto s_f1 = stof(str, &sz);
        auto s_d0 = stod(str);
        auto s_d1 = stod(str, &sz);
        auto s_ld = stold(str, nullptr);

        auto to_s0 = to_string(s_i0);
        auto to_s1 = to_string(s_f1);
        auto to_s2 = to_string(s_d1);
        auto to_ws0 = to_wstring(s_i0);
        auto to_ws1 = to_wstring(s_f1);
        auto to_ws2 = to_wstring(s_d1);

        maximum(4, 3, 6, 1);
        dummy_wrapper(1, "--", "2.3.4", "--", 3.1);
        myPrintf("a%bcde%fghij%kl%mn\n", 11, "interesting", 998, "very good");
    }

    // 021: STL updates
    static void Run_STL()
    {
        /*** stl new algorithms ***/
        // std::all_of, std::none_of, std::any_of
        array<int, 10> arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        cout << "[std::all_of] are all numbers odd? "
             << (std::all_of(begin(arr), end(arr),
                             [](int i) { cout << i << ", "; return i % 2 == 0; })
                 ? "yes" : "no")
             << endl;
        cout << "[std::none_of] is none of number > 10? "
             << (std::none_of(begin(arr), end(arr),
                              [](int i) { cout << i << ", "; return i > 10; })
                ? "yes" : "no")
             << endl;
        cout << "[std::any_of] is any number times of 5?"
             << (std::any_of(begin(arr), end(arr),
                             [](int i) { cout << i << ", "; return i > 0 && i % 5 == 0; })
                 ? "yes" : "no")
             << endl;

        // std::find_if, std::find_if_not
        auto f3 = [&]() {
            auto itr = std::find_if(begin(arr), end(arr), [](int i) { cout << i << ", "; return i > 0 && i % 3 == 0; });
            return itr == end(arr) ? "no" : to_string(*itr);
        };
        auto f10 = [&]() {
            auto itr = std::find_if_not(begin(arr), end(arr), [](int i) { cout << i << ", "; return i <= 10; });
            return itr == end(arr) ? "no" : to_string(*itr);
        };
        cout << "[std::find_if] first number times of 3: " << f3() << endl;
        cout << "[std::find_if_not] first number greater than 10: " << f10() << endl;

        // string movement is implemented as swap
        string str0 = "i'm str0";
        string str1 = "i'm str1";
        str1 = move(str0);

        // std::move_backward
        array<int, arr.size()> dest{0};
        std::move_backward(begin(arr), end(arr), end(dest));
        cout << "[std::move_backward]";
        cout << "\n\tinitial array: ";
        std::copy(begin(arr), end(arr), std::ostream_iterator<int>(std::cout, ", "));
        cout << "\n\tdest array: ";
        std::copy(begin(dest), end(dest), std::ostream_iterator<int>(std::cout, ", "));
        cout << endl;

        // std::shuffle, std::random_shuffle
        std::random_device rd;
        std::mt19937 g(rd());
        std::copy(begin(arr), end(arr), begin(dest));
        std::shuffle(begin(dest), end(dest), g);
        cout << "[std::shuffle] (0:9): ";
        std::copy(begin(dest), end(dest), std::ostream_iterator<int>(std::cout, ", "));
        cout << endl;

        std::copy(begin(arr), end(arr), begin(dest));
        std::random_shuffle(begin(dest), end(dest));
        cout << "[std::random_shuffle] (0:9): ";
        std::copy(begin(dest), end(dest), std::ostream_iterator<int>(std::cout, ", "));
        cout << endl;

        std::copy(begin(arr), end(arr), begin(dest));
        std::srand(unsigned(std::time(0)));
        auto myrandom = [](int i) { return std::rand() % i; };
        std::random_shuffle(begin(dest), end(dest), myrandom);
        cout << "[std::random_shuffle, myrandom] (0:9): ";
        std::copy(begin(dest), end(dest), std::ostream_iterator<int>(std::cout, ", "));
        cout << endl;

        // std::is_partitioned, std::partition_copy, std::partition_point
        std::copy(begin(arr), end(arr), begin(dest));
        auto isOdd = [](int i) { return i % 2 == 1; };
        auto f_part = [&]() {
            cout << (std::is_partitioned(begin(dest), end(dest), isOdd) ?
                        "(partitioned)\n" : "(not partitioned)\n");
        };
        cout << "original dest (0:9): ";
        f_part();
        std::partition(begin(dest), end(dest), isOdd);
        cout << "[std::partition] dest (0:9): ";
        f_part();
        std::copy(begin(dest), end(dest), std::ostream_iterator<int>(std::cout, ", "));
        cout << endl;

        std::copy(begin(arr), end(arr), begin(dest));
        vector<int> odd(dest.size());
        vector<int> even(dest.size());
        std::partition_copy(begin(dest), end(dest), begin(odd), begin(even), isOdd);
        odd.shrink_to_fit();
        odd.shrink_to_fit();
        cout << "odd: ";
        std::copy(begin(odd), end(odd), std::ostream_iterator<int>(std::cout, ", "));
        cout << "\neven: ";
        std::copy(begin(even), end(even), std::ostream_iterator<int>(std::cout, ", "));
        cout << endl;

        std::copy(begin(arr), end(arr), begin(dest));
        auto it = std::partition_point(begin(dest), end(dest), isOdd);
        odd.clear();
        odd.assign(begin(dest), it);
        cout << "odd: ";
        std::copy(begin(odd), end(odd), std::ostream_iterator<int>(std::cout, ", "));
        cout << endl;

        // std::minmax, std::minmax_element
        std::prev_permutation(begin(dest), end(dest));
        cout << "[std::prev_permutation] (0:9): ";
        std::copy(begin(dest), end(dest), std::ostream_iterator<int>(std::cout, ", "));
        cout << endl;
        //auto v_minmax = std::minmax(dest);
        //cout << "[std::minmax] (" << v_minmax.first << ", " << v_minmax.second << ")" << endl;
        auto iter_minmax = std::minmax_element(begin(dest), end(dest));
        cout << "[std::minmax_element] (" << *iter_minmax.first << ", " << *iter_minmax.second << ")" << endl;

        /*** stl regular expressions ***/
        // std::regex
        bool equals = regex_match("subject", regex("(sub)(.*)"));
        string fnames[] = {"foo.txt", "bar.txt", "baz.dat", "zoidberg"};

        // simple regular expression matching
        regex txt_regex("[a-z]+\\.txt");
        cout << "simple regular expression match: \n";
        for (const auto& fname : fnames)
        {
            cout << fname << ": " << regex_match(fname, txt_regex) << endl;
        }

        // extraction of a sub-match
        regex base_regex("([a-z]+)\\.txt");
        smatch base_match;
        cout << "extraction of a sub-match: \n";
        for (const auto& fname : fnames)
        {
            if (std::regex_match(fname, base_match, base_regex))
            {
                if (base_match.size() == 2)
                {
                    std::ssub_match base_sub_match = base_match[1];
                    string base = base_sub_match.str();
                    cout << fname << " has a base of " << base << endl;
                }
            }
        }

        /*** stl chrono ***/
        // std::chrono
        auto start = std::chrono::high_resolution_clock::now();
        sleep(2);
        auto end = std::chrono::high_resolution_clock::now();
        cout << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << endl;

        auto st = std::chrono::system_clock::now();
        sleep(2);
        auto ed = std::chrono::system_clock::now();
        auto elapsed_seconds = ed - st;
        auto end_time = std::chrono::system_clock::to_time_t(ed);
        cout << "finished computation at " << std::ctime(&end_time)
                << "\nelapsed time: " << elapsed_seconds.count() << endl;

        // std::ratio
        std::ratio<2, 3> two_thirds;
        std::ratio_add<ratio<1, 2>, ratio<2, 6> > sum;
        cout << "2/3: " << two_thirds.num << " / " << two_thirds.den
                <<"\n1/2+2/6: " << sum.num << " / " << sum.den << endl;
                //<< "\n2/3 > (1/2+2/6): " << std::ratio_greater<two_thirds, sum>::value << endl;
    }

    // 022: thread
    struct Task
    {
        int _index;
        string _name;

        Task() { cout << "Task()" << endl; }
        ~Task(){ cout << "~Task()" << endl; }
        Task(Task& t) { cout << "Task(Task&)" << endl; _index = t._index; _name = t._name; }
        Task(const Task& t) : Task(const_cast<Task&>(t)) { cout << "Task(const Task&)" << endl; }
        void operator()() const
        {
            cout << "Task::operator() [thread id: " << std::this_thread::get_id() << "]" << endl;
        }
        void operator()(int i) const
        {
            cout << "Task::operator()(int) [i: " << i << "]" << endl;
        }

        /* cannot use overload function here for std::thread, WHY???
         */
        void Do(int i)
        {
            cout << "Task::Do(int) - " << i << endl;
        }
        /*void Do(string str)
        {
            cout << "Task::Do(string) - " << str << " [thread id: " << std::this_thread::get_id() << "]" << endl;
        }*/
    };

    // test std::condition_variable and std::unique_lock
    struct CV_Shipment
    {
        std::mutex _mtx;
        std::condition_variable _cv;
        int _cargo = 0;

        void consume(int n)
        {
            for (int i = 0; i < n; ++i)
            {
                std::unique_lock<std::mutex> lk(_mtx);
                _cv.wait(lk, [&]() { return _cargo != 0; });
                // start consuming...
                cout << _cargo << endl;
                _cargo = 0;
            }
        }
        void run()
        {
            int num_cargos = 5;
            std::thread consumer_thread(&CV_Shipment::consume, this, num_cargos);

            // produce cargos...
            for (int i = 0; i < num_cargos; ++i)
            {
                while (_cargo != 0)
                    std::this_thread::yield();
                std::unique_lock<mutex> lk(_mtx);
                _cargo = i + 1;
                _cv.notify_one();
            }

            consumer_thread.join();
        }
    };
    struct CV_consumer_producer
    {
        std::mutex _mtx;
        std::condition_variable _produce, _consume;
        int _cargo = 0;

        void consumer()
        {
            std::unique_lock<std::mutex> lk(_mtx);
            _consume.wait(lk, [&]() { return _cargo != 0; });
            cout << "consume cargo " << _cargo << endl;
            _cargo = 0;
            _produce.notify_one();
        }
        void producer(int id)
        {
            std::unique_lock<std::mutex> lk(_mtx);
            _produce.wait(lk, [&]() { return _cargo == 0; });
            _cargo = id;
            cout << "product cargo " << _cargo << endl;
            _consume.notify_one();
        }

        void run()
        {
            int num = 5;
            vector<std::thread> consumers, producers;
            for (int i = 0; i < num; ++i)
            {
                consumers.push_back(std::thread(&CV_consumer_producer::consumer, this));
                producers.push_back(std::thread(&CV_consumer_producer::producer, this, i+1));
            }
            for (int i = 0; i < num; ++i)
            {
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

        int test = 0;
        std::mutex mu;
        auto print = [&](int i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            //std::lock_guard<std::mutex> locker(mu);
            cout << "print(int): " << i << endl;
        };
        auto f_add = [&]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::lock_guard<std::mutex> locker(mu);
            ++(++test);
            cout << "f_add: " << test << endl;
            print(test);
        };
        auto f_minus = [&]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::lock_guard<std::mutex> locker(mu);
            --test;
            cout << "f_minus: "<< test << endl;
        };

        vector<thread> v_ths;
        for (int i = 0; i < 1; ++i)
        {
            v_ths.push_back(thread(f_add));
            //v_ths.push_back(thread(f_minus));
        }
        for_each (begin(v_ths), end(v_ths), [](thread& o) { o.join(); });

        std::thread th0{Task()};
        Task tk;
        // std::thread works as callable object, same as std::async, std::bind, std::call_once
        std::thread th1(&Task::Do, &tk, 10); // tk.Do in the different thread
        std::thread th2(&Task::Do, tk, 11); // copy_of_tk.Do in the different thread
        //std::thread th3(&Task::Do, std::ref(tk), 12); // compile error, cannot use std::ref(tk)!
        std::thread th4(std::ref(tk), 13); // tk.()(int) in the different thread
        std::thread th5(tk, 14); // copy_of_tk.()(int) in the different thread
        //std::thread th2{&Task::Do, &tk, "th2, char*"}; // not safe
        //std::thread th3{&Task::Do, &tk, string("th3, string")}; // safe

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
                cout << c << endl;
            }
        };
        cout << "With launch::async:\n";
        std::future<void> foo = std::async(std::launch::async, print_10, '*', 100);
        std::future<void> bar = std::async(std::launch::async, print_10, '@', 200);
        // async 'get' (wait for foo and bar to be ready)
        foo.get(); // will finish print_10 but won't block other threads
        bar.get();
        cout << "\n\n";

        cout << "With launch::deferred:\n";
        foo = std::async(std::launch::deferred, print_10, '*', 100);
        bar = std::async(std::launch::deferred, print_10, '@', 200);
        // deferred 'get' (perform the actual calls)
        foo.get(); // will finish print_10 and occupy resource until complete
        bar.get();
        cout << "\n\n";

        auto print_ret = [](char c) {
            for (int i = 1; i < 10; ++i)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                cout << c << endl;
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
        cout << v_ft1._name << endl;
        */
/*
        // std:: promise
        std::promise<int> pro;
        std::future<int> fu = pro.get_future();
        auto print_pro = [](std::future<int>& f) {
            cout << "print_pro starts..." << endl;
            auto v = f.get();
            cout << v << endl;
            cout << "print_pro ends... " << endl;
        };

        std::thread th(print_pro, std::ref(fu));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        pro.set_value(100);
        th.join();

        //
        auto print_future = [](std::future<char>& f) {
            cout << "print_future starts..." << endl;
            auto v = f.get();
            for (int i = 1; i < 10; ++i)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                cout << v << endl;
            }
            Task t;
            t._index = 10;
            t._name = "this is a task";
            cout << "print_future ends..." << endl;
            return t;
        };
        std::promise<char> pro1;
        std::future<char> fu1 = pro1.get_future();
        std::future<Task> fu_1 = std::async(std::launch::async, print_future, std::ref(fu1));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        pro1.set_value('%');
        auto t = fu_1.get();
        cout << t._name << endl;

        //
        auto print_ff = [](std::shared_future<Task&> sf) {
            cout << "print_ff starts..." << endl;
            // strange thing! If do auto t = sf.get(); then will do copy
            // If do Task& t = sf.get(); then will do ref and no copy. Why auto
            // doesn't work for ref automatically?
            Task& t = sf.get();
            t._index += 1;
            cout << t._name << " " << t._index << endl;
            cout << "print_ff ends..." << endl;
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
        cout << tt._name << " " << tt._index << endl;
        */
    }

    // 023: defalut move constructor
    struct MoveTest
    {
        MoveTest() { cout << "MoveTest()" << endl; }
        MoveTest(const MoveTest& o) { cout << "MoveTest(const MoveTest&)" << endl; }
        MoveTest& operator=(const MoveTest& o) { cout << "operator=(const MoveTest&)" << endl; }
        MoveTest(MoveTest&&) { cout << "MoveTest(MoveTest&&)" << endl; }
        MoveTest& operator=(MoveTest&&) { cout << "operator=(MoveTest&&)" << endl; }
        ~MoveTest() { cout << "~MoveTest()" << endl; }
        string _str;
    };

    class StringTable
    {
    public:
        StringTable() {}
        //StringTable(const StringTable&) {}
        //StringTable(StringTable&&) { cout << "StringTable(StringTable&&)" << endl; }
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
            return move(oMove);
        };
        auto getStringTableEx = []() {
            StringTableEx oCopy;
            oCopy._oMove._str = "this is StringTableEx has no defalut move";
            return move(oCopy);
        };

        StringTable o1 = getStringTable();
        StringTableEx o2 = getStringTableEx();
        cout << o1._oMove._str << " "<< o2._oMove._str << endl;
    }
};

#endif // C11_H
