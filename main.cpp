#include "header.h"
#include "./UseLib/LibClass.h"
#include "./language/classtest.h"
#include "./language/stlClass.h"
#include "./language/sizeofClass.h"
#include "./language/MyList.h"
#include "./language/matrix.h"
#include "./language/fill_fill_n.h"
#include "./algorithm/sherlock.h"
#include "./algorithm/biggerisgreater.h"
#include "./algorithm/gemstones.h"
#include "./algorithm/traversetree.h"

#include "./test/Test_language.h"
#include "./test/Test_algorithm.h"

using namespace std;

int main() {

    Test_copy_elision();
    //Test_thread();
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

void runMyList() {
    MyList<int> olist;
    int i_2 = 2;
    int i_3 = 3;
    int i_5 = 5;
    int i_8 = 8;
    int i_7 = 7;
    int i_0 = 0;
    int i_11 = 11;
    int i_10 = 10;
    olist.Add(i_3);
    olist.Add(i_5);
//    olist.Add(i_2);
    ListNode<int>* p = olist.Find(i_3);
    p = olist.Insert(p, i_8);
    olist.Insert(p, i_7);
    olist.Insert2Head(i_0);
    olist.Insert(p, i_11);
    olist.Insert2End(i_5);
    olist.Insert2End(i_3);
    olist.Remove(i_8);
    p = olist.Insert2Head(i_10);
    olist.Remove(i_10);

//    olist.Display();

    olist.Reverse();
    olist.Display();
}

void runsizeofClass() {
    sizeofClass::CheckBasicTypes();
    sizeofClass::CheckClass(stlClass("temp", 1000));

    stlClass stlObj("test", 43);
    stlClass stlObj_1 = stlObj;
    cout << "**stlObj_1**" << endl;
    //	cout << stlObj_1;
    //	cout << "stlObj_1 == stlObj: " << (stlObj==stlObj_1) << endl;

    set<stlClass> data;
    data.insert(stlClass("hello", 5));
    //	data.insert(stlClass("any", 1));
    //	data.insert(stlClass("nobody", 11));
    //	data.insert(stlClass("hahah", 0));
    //	data.insert(stlObj);
    //	data.insert(stlObj_1);

    //	copy(data.begin(), data.end(), ostream_iterator<stlClass>(cout, " * "));
}

void runTraverse() {
    tNode root(0);
    tNode n1(1);
    tNode n2(2);
    tNode n3(3);
    tNode n4(4);
    tNode n5(5);
    tNode n6(6);
    tNode n7(7);
    tNode n8(8);
    tNode n9(9);
    tNode n10(10);
    tNode n11(11);
    root.left = &n1;
    root.right = &n2;
    n1.left = &n3;
    n1.right = &n4;
    n2.left = &n5;
    n2.right = &n6;
    n3.right = &n7;
    n4.left = &n8;
    n4.right = &n9;
    n6.left = &n10;
    n8.left = &n11;

    TraverseInorder(&root);
    TraversePostorder(&root);
}

void runMatrix() {
    Matrix<int> m(2,3);
    m(0,0) = 1;
    m(1,1) = 3;
    
    cout << m[0][0] << "," << m[1][1] << endl;
}


