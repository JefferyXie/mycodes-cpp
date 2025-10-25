#ifndef STDFUNCTION_H
#define STDFUNCTION_H

#include "../main/formatter.h"
#include "../main/header.h"
#include "../main/utility.h"

using namespace std::placeholders;

typedef int (*PFUN)(char);

int fun_fo(char ch) {
    cout << "fun_fo(ch): " << ch << endl;
    return 10;
}

class A_function {
public:
    int Go(char ch) {
        cout << "A::Go(char): " << ch << endl;
        return 5;
    }
    void Test(function<int(char)> fun) {
        cout << "<<Test(function)" << endl;
        cout << fun('8') << endl;
        cout << ">>Test(function)" << endl;
    }
    void Test(PFUN pf) {
        cout << "<<Test(PFUN): " << endl;
        cout << pf('t') << endl;
        cout << ">>Test(PFUN)" << endl;
    }
    void Use() {
        cout << "<<Use" << endl;
        
        // 5-1) call Test(PFUN) since it best fits, without Test(PFUN), it calls Test(function)
        Test(fun_fo);
        
        // 5-2) call Test(function) since it best fits
        function<int(char)> fun = fun_fo;
        Test(fun);
        
        // 5-3) construct function<..> with bind, taking 'this' object
        function<int(char)> fun_1 = std::bind(&A_function::Go, this, _1);
        Test(fun_1);
        
        // 5-4) construct function<..> with bind, taking 'this' object, fixing para with 'p'
        function<int(char)> fun_2 = std::bind(&A_function::Go, this, 'p');
        Test(fun_2);
        
        cout << ">>Use" << endl;
    }
};

void run_std_function_1()
{
    FUNC_BEGIN(run_std_function_1);

    // 0) function pointer
    PFUN pf = fun_fo;
    cout << pf('u') << endl;

    // 1) function<..> of function
    function<int(char)> f_fo = fun_fo;
    cout << f_fo('x') << endl;
    
    // 2) function<..> of lambda
    function<int(char)> f_go = [](char c) {
        cout << "<<lambda" << endl;
        fun_fo(c);
        cout << "lambda>>" << endl;
        return 1;
    };
    cout << f_go('m') << endl;
    
    // 3) function<..> of function pointer
    function<int(char)> pf_go = pf;
    cout << pf_go('q') << endl;
    
    // 5) use function<..> inside class member function
    A_function a, b;
    a.Use();
    
    // 6) bind with class member function with object fixed
    function<int(char)> fun_obj = std::bind(&A_function::Go, b, _1);
    cout << fun_obj('b') << endl;

    // 7) bind with class memeber function with object fixed, function-self's para fixed too
    function<int()> fun_oo = std::bind(&A_function::Go, b, 'y');
    cout << fun_oo() << endl;
    
    // 8) bind with class member function with object as input para
    function<int(A_function&,char)> fun_A = std::bind(&A_function::Go, _1, _2);
    cout << "<<function<int(A_function&,char)>" << endl;
    cout << fun_A(b, 'q') << endl;
    cout << ">>function<int(A_function&,char)>" << endl;
    
    // 9) bind with class member function with object as input para, fix function-self's para
    function<int(A_function&)> fun_B = std::bind(&A_function::Go, _1, 'w');
    cout << "<<function<int(A_function&)>" << endl;
    cout << fun_B(b) << endl;
    cout << ">>function<int(A_function&)>" << endl;
}

// 
// a cleaner example for real case
class func_caller {
public:
    // 1) add_callback: class member method as input
    template<class T> void
    add_callback(T* const object, char(T::* const mf)(bool,int))
    {
        using namespace std::placeholders; 
        callbacks_.emplace_back(std::bind(mf, object, _1, _2));
    }
 
    // 2) add_callback: regular function or class static method as input
    using PFunc = char(*)(bool,int);
//  void addCallback(char * const fun(bool,int))// NO! void* const (*)(bool, int)
//  void addCallback(char * fun(bool,int))      // NO! void* (*)(bool, int)
//  void addCallback(char fun(bool,int))        // works
//  void addCallback(PFunc fun)                 // works
//  void addCallback(char (*fun)(bool, int))    // works
    void add_callback(char(* const fun)(bool, int)) // works
    {
        callbacks_.emplace_back(fun);
    }

    // 3) add_callback: function object as input
    void add_callback(std::function<char(bool,int)>&& func) {
        callbacks_.emplace_back(std::move(func));
    }

    void call_callbacks(bool firstval, int secondval) 
    {
        for (const auto& cb : callbacks_)
            cb(firstval, secondval);
    }
private:
    std::vector<std::function<char(bool,int)>> callbacks_;
};

struct func_class {
    char in_class_func(bool,int) {
        cout << "in_class_func" << endl; return '1';
    }
};
char regular_func(bool,int) {
    cout << "regular_func" << endl; return '2';
}
struct functor_class {
    char operator()(bool,int) {
        cout << "functor_class operator()" << endl; return 'c';
    }
};

void run_std_function_2()
{
    FUNC_BEGIN(run_std_function_1);

    func_caller caller;
    func_class func_o;
    //then, somewhere in Callee, to add the callback, given a pointer to Caller `ptr`
    caller.add_callback(&func_o, &func_class::in_class_func);
    
    //or to add a call back to a regular function
    caller.add_callback(regular_func);
    caller.add_callback(&regular_func);
    
    caller.call_callbacks(true, 10);
    
    // std::function can take input with any kind of - 
    // 1) a function
    // 2) a function pointer
    // 3) a pointer to member
    // 4) lambda
    // 5) any kind of copy-constructible function object 
    //    (i.e., an object whose class defines operator(), 
    //    including closures and other instantiations of function).
    std::function<char(bool,int)> fn1 = regular_func;   // function
    std::function<char(bool,int)> fn2 = &regular_func;  // function pointer
    std::function<char(bool,int)> fn3 = functor_class();// function object
    std::function<char(bool,int)> fn4 = [](bool,int) {  // lambda expression
        cout << "lambda" << endl; return 'd';
    };
    caller.add_callback(std::move(fn1));
    caller.add_callback(std::move(fn2));
    caller.add_callback(std::move(fn3));
    caller.add_callback(std::move(fn4));
    caller.call_callbacks(true, 10);
}

#endif

