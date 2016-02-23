#include "../header.h"
#include "../language/stlClass.h"
#include "../language/sizeofClass.h"
#include "../language/MyList.h"
#include "../language/MyDictionary.h"
#include "../language/C++11.h"
#include "../language/stl.h"
#include "../algorithm/sherlock.h"
#include "../algorithm/judgeBiTree.h"

#define EXPECT_EQ(actual, expected) \
    if (actual == expected) { \
        cout << "[Success] "	<< expected << endl; \
    } else { \
        cout << "[Fail] " << actual << " [Expected] " << expected << endl; \
    }

void Test();
int main()
{
    string tc1 = JudgeBiTree("(B,D) (D,E) (A,B) (C,F) (E,G) (A,C)");
    EXPECT_EQ(tc1, "(A(B(D(E(G))))(C(F)))");

    string tc2 = JudgeBiTree("(A,B) (A,C) (B,D) (D,C)");
    EXPECT_EQ(tc2, "E5");

    string tc3 = JudgeBiTree("(A,B) (A,C) (B,D) (E,F) (F,G) (F,H)");
    EXPECT_EQ(tc3, "E4");
    
    string tc4 = JudgeBiTree("(A,B) (B,D) (B,C) (C,A)");
    EXPECT_EQ(tc4, "E5");

    string tc5 = JudgeBiTree("(A,B) (B,C) (A,B) (A,C)");
    EXPECT_EQ(tc5, "E2");

    string tc6 = JudgeBiTree("(A,B) (A,C) (A,D) (B,E)");
    EXPECT_EQ(tc6, "E3");
    
    string tc7 = JudgeBiTree("(U,P) (O,L) (A,S) (N,O) (S,T) (U,M) (A,N) (S,U) (L,D)");
    EXPECT_EQ(tc7, "(A(N(O(L(D))))(S(T)(U(M)(P))))");

    string tc8 = JudgeBiTree("(A,B) (B,C) (A,B)");
    EXPECT_EQ(tc8, "E2");

    string tc9 = JudgeBiTree("(A,C) (A,B) (B,Q) (B,P) (C,D) (Q,Z) (Q,X)");
    EXPECT_EQ(tc9, "(A(B(P)(Q(X)(Z)))(C(D)))");
    
    string tc10 = JudgeBiTree("(A,B)   (A,C) (B, E) (B,F)");
    EXPECT_EQ(tc10, "E1");

    string tc11 = JudgeBiTree("(P,J) (X,T) (R,E) (P,C) (X,B) (R,X)");
    EXPECT_EQ(tc11, "E4");

    string tc12 = JudgeBiTree("(X,Y) (R,D) (D,P) (R,A) (X,Z) (Z,T) (Z,B) (R,G)");
    EXPECT_EQ(tc12, "E3");

    string tc13 = JudgeBiTree("(R,N) (U,T) (C,H) (Y,B) (R,A) (Y,C) (K,Y) (B,R) (H,E) (H,S) (K,U) (T,M)");
    EXPECT_EQ(tc13, "(K(U(T(M)))(Y(B(R(A)(N)))(C(H(E)(S)))))");

    string tc14 = JudgeBiTree("(K,M) (P,L) (L,S) (Y,A) (T,P) (T,K) (L,E) (K,Y)");
    EXPECT_EQ(tc14, "(T(K(M)(Y(A)))(P(L(E)(S))))");

    string tc15 = JudgeBiTree("(A,B) (A,C (B,D) B,E)");
    EXPECT_EQ(tc15, "E1");

    string tc16 = JudgeBiTree("    (P,B) (E,X) (P,D) (E,Y) (P,A)");
    EXPECT_EQ(tc16, "E1");
    return 0;

    STL::Swap();
    STL::ostream_vector();
    STL::ostream_array();

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
