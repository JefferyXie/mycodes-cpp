#include "../header.h"
#include "../language/stlClass.h"
#include "../language/sizeofClass.h"
#include "../language/MyList.h"
#include "../language/MyDictionary.h"
#include "../language/C++11.h"
#include "../algorithm/sherlock.h"

void Test();

int main()
{
    C11::Run_variadic();
    return 0;
    Sherlock();

    sizeofClass lso;
    lso.CheckClass(10);

    stlClass o1("o1", 1);
    auto lam_ref = [] (stlClass& o) {
        stlClass oo("oo", 10);
        o = oo;
    };
    auto lam_pt = [] (stlClass* p) {
        static stlClass pp("pp", 20);
        p = &pp;
    };
    lam_ref(o1);
    lam_pt(&o1);

    //const int& a = 0;
    C11::Run_forward();
    //Test();
    //sizeofClass::CheckBasicTypes();
    //cin >> a;
    return 0;
}

void Test()
{
    int i_0 = 0, i_1 = 1, i_2 = 2, i_3 = 3, i_4 = 4, i_5 = 5, i_6 = 6, i_7 = 7, i_8 = 8, i_10 = 10, i_11 = 11;
/*    stlClass obj1("obj1", 1);
    stlClass obj2("obj2", 2);
    stlClass obj3("obj3", 3);
    stlClass obj4("obj4", 4);
    stlClass obj5("obj5", 5);
    stlClass obj6("obj6", 6);
    stlClass obj7("obj7", 7);
    stlClass obj8("obj8", 8);
*/
    {
        /*
        MyDictionary<int> intDict;
        intDict.Add(i_0);
        intDict.Add(i_1);
        TreeNode<int>* p1 = intDict.Add(i_2);
        TreeNode<int>* p2 = intDict.Add(i_3);
        p1 = intDict.Find(i_0);
        p2 = intDict.Find(i_3);

        intDict.Remove(i_5);
        intDict.Remove(i_3);
        intDict.Remove(i_3);
        intDict.Remove(i_0);

        intDict.Display();

        intDict.Add(i_4);
        intDict.Add(i_5);
        intDict.Add(i_6);
        intDict.Add(i_7);

        intDict.Display();

        p1 = intDict.Find(i_0);
        p2 = intDict.Find(i_5);
        intDict.Remove(i_5);
        p2 = intDict.Find(i_5);

        intDict.Display();

        intDict.Remove(i_4);

        intDict.Display();
        */
    }

    sizeofClass::CheckBasicTypes();
    sizeofClass::CheckClass(stlClass("temp", 1000));

    {
        // try int
        MyList<int> olist;
        olist.Add(i_3);
        olist.Add(i_5);
        olist.Add(i_2);
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

        olist.Display();

        olist.Reverse();
        olist.Display();
/*
        // try stlClass objects
        MyList<stlClass> stlList;
        stlList.Add(obj1);
        stlList.Add(obj2);
        stlList.Add(obj3);
        stlList.Add(obj4);
        ListNode<stlClass>* pObj = stlList.Find(obj2);
        stlList.Insert(pObj, obj4);
        stlList.Insert(pObj, obj1);
        stlList.Insert2Head(obj8);
        stlList.Remove(obj4);

        stlList.Display();

        stlList.Reverse();
        ListNode<stlClass>* p5 = stlList.Add(obj5);
        stlList.Insert(p5, obj6);
        stlList.Insert2Head(obj8);
        stlList.Display();

        stlClass stlObj("test", 43);
        stlClass stlObj_1 = stlObj;
        cout << "**stlObj_1**" << endl;
        cout << stlObj_1;
        cout << "stlObj_1 == stlObj: " << (stlObj==stlObj_1) << endl;

        set<stlClass> data;
        data.insert(stlClass("hello", 5));
        data.insert(stlClass("any", 1));
        data.insert(stlClass("nobody", 11));
        data.insert(stlClass("hahah", 0));
        data.insert(stlObj);
        data.insert(stlObj_1);

        copy(data.begin(), data.end(), ostream_iterator<stlClass>(cout, " * "));
        */
    }

}
