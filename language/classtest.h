#ifndef CLASSTEST_H
#define CLASSTEST_H

#include "../main/header.h"

class A
{
protected:
    int         _n    = 10;
    std::string _name = "object A";

public:
    A() = default;
    A(int n, std::string name) : _n(n), _name(name) {}
    A(A&& ref)
    {
        std::cout << "A(A&&)" << std::endl;
        _n    = ref._n;
        _name = std::move(ref._name);
    }
    A&                  operator=(A&& obj) = delete;
    virtual void        Fo() {}
    virtual std::string Process(const std::string&, const std::string&)
    {
        std::cout << "A::Process(const string&, const string&)" << std::endl;
        return "";
    }
    void                Go() { std::cout << "A::Go()" << std::endl; }
    virtual std::string ToString() { return "_n:" + std::to_string(_n) + ",_name:" + _name; }
};

class B : public A
{
private:
    std::string m_nameB = "class B name";

public:
    void Fo() final { std::cout << "B::Fo() final" << std::endl; }
    void Go() { std::cout << "B::Go()" << std::endl; }
    int  Calc(int a, int b)
    {
        std::cout << "B::Calc" << std::endl;
        Go();
        return a + b;
    }
};

struct Plain {
    Plain() { std::cout << "Plain(): construct a Plain object" << std::endl; }
    Plain(const Plain&) { std::cout << "Plain(const Plain&): copy construct a Plain object" << std::endl; }
    Plain& operator=(const Plain&)
    {
        std::cout << "operator=(const Plain&): copy assignment a Plain object" << std::endl;
        return *this;
    }
    Plain(Plain&&) { std::cout << "Plain(Plain&&): move construct a Plain object" << std::endl; }
    Plain& operator=(Plain&&)
    {
        std::cout << "operator=(Plain&&): move assignment a Plain object" << std::endl;
        return *this;
    }
    ~Plain() { std::cout << "~Plain(): destruct a Plain object" << std::endl; }
};

#endif

