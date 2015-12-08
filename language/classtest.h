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
	int value;
	string name;
};

#endif

