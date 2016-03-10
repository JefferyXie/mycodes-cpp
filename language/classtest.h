#ifndef CLASSTEST_H
#define CLASSTEST_H

#include "../header.h"

class A {
protected:
	int _n = 10;
	string _name = "object A";
public:
	A() = default;
	A(A&& ref) {
		cout << "A(A&&)" << endl;
		_n = ref._n;
		_name = move(ref._name);
	}
	A& operator=(A&& obj) = delete;
	virtual void Fo() { }
    virtual string Process(const string&, const string&) { 
        cout << "A::Process(const string&, const string&)" << endl;
        return ""; 
    }
	void Go() {
        cout << "A::Go()" << endl;
    }
};

class B : public A {
private:
	string m_nameB = "class B name";
public:
	void Fo() final {
        cout << "B::Fo() final" << endl;
    }
	void Go() {
        cout << "B::Go()" << endl;
    }
	int Calc(int a, int b) {
        cout << "B::Calc" << endl;
        Go();
        return a+b;
    }
};

struct MyMember {
    static int s_count;
	int value;
	string name;
    //list<int> ls;
    MyMember() {
        value = s_count++;
        name = "name[" + to_string(value) + "]";
        cout << "MyMember() " << value << endl;
    }
    ~MyMember() {
        cout << "~MyMember() " << value << endl;
    }
    string ToString() {
        return "value:" + to_string(value) + ",name:" + name;
    }
    MyMember& operator=(const MyMember&) = delete;
};
int MyMember::s_count = 0;

struct Plain {
    Plain() {
        std::cout << "construct a Plain object" << std::endl;
    }
    Plain(const Plain&) {
        std::cout << "copy construct a Plain object" << std::endl;
    }
    Plain& operator=(const Plain&) {
        std::cout << "copy assignment a Plain object" << std::endl;
        return *this;
    }
    Plain(Plain&&) {
        std::cout << "move construct a Plain object" << std::endl;
    }
    Plain& operator=(Plain&&) {
        std::cout << "move assignment a Plain object" << std::endl;
        return *this;
    }
    ~Plain() {
        std::cout << "destruct a Plain object" << std::endl;
    }
};

#endif

