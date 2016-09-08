#ifndef STDFUNCTION_H
#define STDFUNCTION_H

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

void std_function()
{
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

#endif

