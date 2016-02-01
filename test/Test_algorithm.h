#include <gtest/gtest.h>
#include "../header.h"
#include "../algorithm/FindCommonAncestor.h"
#include "../algorithm/traversetree.h"

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



