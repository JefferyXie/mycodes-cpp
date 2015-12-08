#include "../header.h"
#include "../algorithm/FindCommonAncestor.h"

void Test_FindCommonAncestor() {
    auto_ptr<Node<int> > root(new Node<int>(10));
    auto_ptr<Node<int> > n1(new Node<int>(2));
    auto_ptr<Node<int> > n2(new Node<int>(4));
    auto_ptr<Node<int> > n3(new Node<int>(8));
    auto_ptr<Node<int> > n4(new Node<int>(1));
    auto_ptr<Node<int> > n5(new Node<int>(12));
    auto_ptr<Node<int> > n6(new Node<int>(111));
    auto_ptr<Node<int> > n7(new Node<int>(43));
    auto_ptr<Node<int> > n8(new Node<int>(40));
    auto_ptr<Node<int> > n9(new Node<int>(14));
    auto_ptr<Node<int> > n0(new Node<int>(11));
    
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
    assert(ancestor1 == n1.get());
    auto ancestor2 = FindCommonAncestor<int>(root.get(), n5.get(), n9.get());
    assert(ancestor2 == root.get());
    auto ancestor3 = FindCommonAncestor<int>(root.get(), n3.get(), n0.get());
    assert(ancestor3 == n1.get());
    auto ancestor4 = FindCommonAncestor<int>(root.get(), n1.get(), n7.get());
    assert(ancestor4 == n1.get());
    auto ancestor5 = FindCommonAncestor<int>(root.get(), n0.get(), n6.get());
    assert(ancestor5 == root.get());
    auto ancestor6 = FindCommonAncestor<int>(root.get(), n9.get(), n5.get());
    assert(ancestor6 == root.get());
}


