#pragma once

#include "../core/header.h"

#ifndef CUSTOM_NEW_ENABLED
#define CUSTOM_NEW_ENABLED 0
#endif

// http://en.cppreference.com/w/cpp/memory/new/operator_new
// https://www.relisoft.com/book/tech/9new.html
// http://stackoverflow.com/questions/583003/overloading-new-delete

#if CUSTOM_NEW_ENABLED > 0
// override global new
void* operator new(size_t s)
{
    void* p = std::malloc(s);
    std::cout << "global operator new called, size = " << s << std::endl;
    return p;
}
// override global new[]
void* operator new[](size_t s)
{
    void* p = std::malloc(s);
    std::cout << "global operator new[] called, size = " << s << std::endl;
    return p;
}
// this version provide additional parameter as init value
void* operator new[](size_t s, char v)
{
    void* p = ::operator new[](s);
    std::fill_n(reinterpret_cast<char*>(p), s, v);
    return p;
}
// override global delete
void operator delete(void* p) noexcept
{
    std::cout << "global operator delete called: " << p << std::endl;
    std::free(p);
}
// override global delete[]
void operator delete[](void* p) noexcept
{
    std::cout << "global operator delete[] called: " << p << std::endl;
    std::free(p);
}

class OpNewDelete
{
public:
    // 1) class-specific overload, the operator new is implicitly static even if not explicitly declared with static
    //    keyword;
    // 2) class-specific operator new functions are inherited by derived classes;

    static void* operator new(size_t s)
    {
        std::cout << "OpNewDelete::operator new called, size = " << s << std::endl;
        // will call default global new
        void* p = ::operator new(s);
        return p;
    }
    static void operator delete(void* p)
    {
        std::cout << "OpNewDelete::operator delete called: " << p << std::endl;
        // will call default global delete
        ::operator delete(p);
    }
    virtual ~OpNewDelete() { std::cout << "~OpNewDelete()" << std::endl; };
    virtual void foo() {};

    OpNewDelete() { std::cout << "OpNewDelete()" << std::endl; }
};

void run_OpNewDelete()
{
    // 1) call OpNewDelete::operator new(..)
    // 2) call ::operator new(..)
    // 3) call constructor OpNewDelete()
    auto p1 = new OpNewDelete;
    // 4) call destructor ~OpNewDelete()
    // 5) call OpNewDelete::operator delete(..)
    // 6) call ::operator delete(..)
    delete p1;
    std::cout << "delete p1" << std::endl;

    // 1) call ::operator new[](..)
    // 2) call constructor OpNewDelete() for twice
    auto p2 = new OpNewDelete[2];
    // 3) call destructor ~OpNewDelete() for twice
    // 4) call ::operator delete[](..)
    delete[] p2;
    std::cout << "delete[] p2" << std::endl;

    /*
    OpNewDelete::operator new called, size = 8
    global operator new called, size = 8
    OpNewDelete()
    ~OpNewDelete()
    OpNewDelete::operator delete called: 0x5a11bc4ce6c0
    global operator delete called: 0x5a11bc4ce6c0
    delete p1

    global operator new[] called, size = 24
    OpNewDelete()
    OpNewDelete()
    ~OpNewDelete()
    ~OpNewDelete()
    global operator delete[] called: 0x5a11bc4ce6c0
    delete[] p2
    */
}

#endif

