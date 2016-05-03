#ifndef OPERATORNEWDELETE_H
#define OPERATORNEWDELETE_H

#include "../main/header.h"

// http://en.cppreference.com/w/cpp/memory/new/operator_new
// https://www.relisoft.com/book/tech/9new.html
// http://stackoverflow.com/questions/583003/overloading-new-delete

// override global new
void* operator new(size_t s) {
    void* p = std::malloc(s);
    cout << "global operator new called, size = " << s << endl;
    return p;
}
// override global new[]
void* operator new[](size_t s) {
    void* p = std::malloc(s);
    cout << "global operator new[] called, size = " << s << endl;
    return p;
}
// this version provide additional parameter as init value
void* operator new[](size_t s, char v) {
    void* p = ::operator new[](s);
    std::fill_n(reinterpret_cast<char*>(p), s, v);
    return p;
}
// override global delete
void operator delete(void* p) noexcept {
    cout << "global operator delete called: " << p << endl;
    std::free(p);
}
// override global delete[]
void operator delete[](void* p) noexcept {
    cout << "global operator delete[] called: " << p << endl;
    std::free(p);
}
class OpNewDelete {
public:
    static void* operator new(size_t s) {
        // will call default global new
        void* p = ::operator new(s);
        cout << "OpNewDelete::operator new called, size = " << s << endl;
        return p;
    }
    static void operator delete(void* p) {
        cout << "OpNewDelete::operator delete called: " << p << endl;
        // will call default global delete
        ::operator delete(p);
    }
    virtual ~OpNewDelete() {};
    virtual void foo() {};
};

#endif


