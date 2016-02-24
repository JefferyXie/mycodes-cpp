#include <gtest/gtest.h>
#include "../header.h"
#include "../algorithm/FindCommonAncestor.h"
#include "../algorithm/traversetree.h"
#include "../algorithm/judgebitree.h"
#include "../algorithm/numdaysbetween.h"

// ASSERT_XXX(): If assertion fails then processing of test terminate.
// EXPECT_XXX(): nonfatal failure, allowing processing to continue.

TEST(algorithm, FindCommonAncestor) {
    auto_ptr<TreeNode<int> > root(new TreeNode<int>(10));
    auto_ptr<TreeNode<int> > n1(new TreeNode<int>(2));
    auto_ptr<TreeNode<int> > n2(new TreeNode<int>(4));
    auto_ptr<TreeNode<int> > n3(new TreeNode<int>(8));
    auto_ptr<TreeNode<int> > n4(new TreeNode<int>(1));
    auto_ptr<TreeNode<int> > n5(new TreeNode<int>(12));
    auto_ptr<TreeNode<int> > n6(new TreeNode<int>(111));
    auto_ptr<TreeNode<int> > n7(new TreeNode<int>(43));
    auto_ptr<TreeNode<int> > n8(new TreeNode<int>(40));
    auto_ptr<TreeNode<int> > n9(new TreeNode<int>(14));
    auto_ptr<TreeNode<int> > n0(new TreeNode<int>(11));
    
    root->left = n1.get();
    root->right = n2.get();
    n1->left = n3.get();
    n1->right = n4.get();
    n2->left = n5.get();
    n2->right = n6.get();
    n3->left = n7.get();
    n3->right = n8.get();
    n4->right = n9.get();
    n9->left = n0.get();
    
    auto ancestor1 = FindCommonAncestor<int>(root.get(), n7.get(), n0.get());
    EXPECT_EQ(ancestor1, n1.get());
    auto ancestor2 = FindCommonAncestor<int>(root.get(), n5.get(), n9.get());
    EXPECT_EQ(ancestor2, root.get());
    auto ancestor3 = FindCommonAncestor<int>(root.get(), n3.get(), n0.get());
    EXPECT_EQ(ancestor3, n1.get());
    auto ancestor4 = FindCommonAncestor<int>(root.get(), n1.get(), n7.get());
    EXPECT_EQ(ancestor4, n1.get());
    auto ancestor5 = FindCommonAncestor<int>(root.get(), n0.get(), n6.get());
    EXPECT_EQ(ancestor5, root.get());
    auto ancestor6 = FindCommonAncestor<int>(root.get(), n9.get(), n5.get());
    EXPECT_EQ(ancestor6, root.get());
}

TEST(algorithm, traverse) {
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

TEST(algorithm, judgeBiTree) {
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

    string tc16 = JudgeBiTree("   (P,B) (E,X) (P,D) (E,Y) (P,A)");
    EXPECT_EQ(tc16, "E1");
}

TEST(algorithm, numdaysbetween) {
    int a = NumDaysBetween(2010,5,1,2011,5,1);
    EXPECT_EQ(a, 365);
    int b = NumDaysBetween(1900,1,10,1901,3,1);
    EXPECT_EQ(b, 415);
    int c = NumDaysBetween(1999,10,1,2015,12,10);
    EXPECT_EQ(c, 5914);
    int d = NumDaysBetween(1644,3,21,2016,2,23);
    EXPECT_EQ(d, 135843);
    cout << a << ", " << b << ", " << c << ", " << d << endl;
 }


