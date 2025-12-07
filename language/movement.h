#pragma once

#include "../main/header.h"
#include "./classtest.h"

namespace movement {
// http://stackoverflow.com/questions/3106110/what-are-move-semantics
// http://en.cppreference.com/w/cpp/language/move_assignment
struct A {
    std::string s;
    A() : s("test") {}
    A(const A& o) : s(o.s) { std::cout << "move failed!\n"; }
    A(A&& o) : s(std::move(o.s)) {}
    A& operator=(const A& other)
    {
        s = other.s;
        std::cout << "copy assigned\n";
        return *this;
    }
    A& operator=(A&& other)
    {
        s = std::move(other.s);
        std::cout << "move assigned\n";
        return *this;
    }
    A operator+(const A& other)
    {
        A a;
        a.s += other.s;
        return a;
    }
};

A f(A a)
{
    return a;
}
Plain g()
{
    Plain p;
    return p;
}
Plain g(Plain)
{
    Plain p;
    return p;
}

struct B : A {
    std::string s2;
    int         n;
    // implicit move assignment operator B& B::operator=(B&&)
    // calls A's move assignment operator
    // calls s2's move assignment operator
    // and makes a bitwise copy of n
};

struct C : B {
    ~C() {}    // destructor prevents implicit move assignment
};

struct D : B {
    D() {}
    ~D() {}                         // destructor would prevent implicit move assignment
    D& operator=(D&&) = default;    // force a move assignment anyway
};

// swap under old standard
template <class T>
void swap_98(T& a, T& b)
{
    T tmp(a);    // now we have two copies of a
    a = b;       // now we have two copies of b (+ discarded a copy of a)
    b = tmp;     // now we have two copies of tmp (+ discarded a copy of b)
}
// swap under c++11
template <class T>
void swap_11(T& a, T& b)
{
    T tmp(std::move(a));
    a = std::move(b);
    b = std::move(tmp);
};
};    // namespace movement

void run_move()
{
    using namespace movement;

    using A = movement::A;
    using B = movement::B;

    A a1, a2;
    std::cout << "Trying to move-assign A from rvalue temporary\n";
    a1 = f(A());    // move-assignment from rvalue temporary
    std::cout << "Trying to move-assign A from xvalue\n";
    a2 = std::move(a1);    // move-assignment from xvalue

    std::cout << "Trying to move-assign B\n";
    B b1, b2;
    std::cout << "Before move, b1.s = \"" << b1.s << "\"\n";
    b2 = std::move(b1);    // calls implicit move assignment
    std::cout << "After move, b1.s = \"" << b1.s << "\"\n";

    std::cout << "Trying to move-assign C\n";
    C c1, c2;
    c2 = std::move(c1);    // calls the copy assignment operator

    std::cout << "Trying to move-assign D\n";
    D d1, d2;
    d2 = std::move(d1);

    // A a(x); no movement is used
    // A a(x+y); movement constructor used here since x+y gives a rvalue
    // A a(some_function_return_A()); movement constructor used here
    // A a = some_function_return_A(); no movement but copy elision
    A x, y;
    A a_1(x);
    A a_2(x + y);
    A a_3(f(x));

    std::cout << "#Plain p1 = g():" << std::endl;
    Plain p1 = g();
    std::cout << "#p1 = g():" << std::endl;
    p1 = g();
    std::cout << "#p1 = g(Plain()):" << std::endl;
    p1 = g(Plain());
    std::cout << "#Plain p(g()):" << std::endl;
    Plain p(g());
}

