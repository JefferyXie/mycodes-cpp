#include "header.h"
#include "./UseLib/LibClass.h"
#include "./language/classtest.h"
#include "./test/Test_language.h"
#include "./test/Test_algorithm.h"

void runLib();

int main() {

    Test_thread();
    //Test_auto();
    //Test_typeinfo();
    //Test_dynamic_cast();
    //Test_constructorOrder();
    //Test_FindCommonAncestor();
/*
    runLib();

	cout << "hello world, from CentOS." << endl;
	printf("hello\n");
	A a;
	A aa = A();
	
	auto lam = [](A* a) { a->Go(); };
	auto lam1 = [](A* a) { 
		auto temp = static_cast<B*>(a);
		if (nullptr == temp)
			a->Go();
		else
			temp->Go();
	};
	vector<int> v = { 2, 3, };
	v.push_back(10);
    list<string> ll;
    ll.push_back("1st ele");
    ll.push_back("2nd ele");
    ll.push_back("3rd ele");
	//A a = A();
	//a.foo();
	B b;
    b.Calc(4, 5);
	lam(&b);
	lam1(&b);
	lam1(&a);
*/
	return 0;
}
/*
void runLib() {
    SharedClass shObj;
    shObj.Fo();
    shObj.Process("first string", "second input string");

    A& aObj = shObj;
    aObj.Fo();
    aObj.Go();
}
*/

