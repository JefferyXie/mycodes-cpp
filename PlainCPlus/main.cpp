#include "../header.h"
#include "../language/stlClass.h"
#include "../language/sizeofClass.h"
#include "../language/MyList.h"
#include "../language/MyDictionary.h"
#include "../language/C++11.h"
#include "../algorithm/sherlock.h"
#include "../language/stl.h"

struct NodePair {
    char parent;
    char child;
};

class Node {
public:
    char value;
    Node* left;
    Node* right;

    string GetSExpression() {
        return getSExpression(this);
    }
private:
    string getSExpression(Node* pNode) {
        string exp;
        if (pNode == NULL) {
            exp += ')';
            return exp;
        }
        exp += '(';
        exp += pNode->value;
        exp += getSExpression(pNode->left);
        exp += getSExpression(pNode->right);
        return exp;
    }
};

class Parser {
public:
    static bool Parse(const string& input, vector<NodePair>& pairs) {
        if (input.length() < 5) return false;
        int pos = 0;
        pairs.clear();
        while (true) {
            int pos1 = input.find(' ', pos);
            string strNode = input.substr(pos, pos1-pos);

            // parse and get a Node info
            if (strNode.length() != 5 || strNode.at(0) != '(' ||
               strNode.at(1) > 'Z' || strNode.at(1) < 'A' ||
               strNode.at(2) != ',' ||
               strNode.at(3) > 'Z' || strNode.at(3) < 'A' ||
               strNode.at(4) != ')') {
                return false;
            }
            NodePair nodePair;
            nodePair.parent = strNode.at(1);
            nodePair.child = strNode.at(3);
            pairs.push_back(nodePair);
            pos = pos1+1;
            if (pos1 < 0) break;
        }
        return true;
    }

    static bool haveDuplicatePairs(const vector<NodePair>& pairs) {
        for (int i = 0; i < pairs.size(); ++i) {
            const NodePair& pair1 = pairs.at(i);
            for (int j = i+1; j < pairs.size(); ++j) {
                const NodePair& pair2 = pairs.at(j);
                if ((pair1.parent == pair2.parent && pair1.child == pair2.child) ||
                    (pair1.parent == pair2.child && pair1.child == pair2.parent)) {
                    return true;
                }
            }
        }
        return false;
    }
    static bool haveMoreChildren(const vector<NodePair>& pairs) {
        for (int i = 0; i < pairs.size(); ++i) {
            int numChildren = 1;
            const NodePair& pair1 = pairs.at(i);
            for (int j = i+1; j < pairs.size(); ++j) {
                const NodePair& pair2 = pairs.at(j);
                if (pair1.parent == pair2.parent) {
                    numChildren++;
                }
                if (numChildren > 2) return true;
            }
        }
        return false;
    }
    static bool haveMultipleRoots(const vector<NodePair>& pairs) {
        char root = 0;
        for (int i = 0; i < pairs.size(); ++i) {
            vector<NodePair> cp_pairs;
            char tempRoot = pairs.at(i).parent;
            copy(pairs.begin(), pairs.end(), back_inserter(cp_pairs));
            string path;
            tempRoot = findRoot(cp_pairs, tempRoot, path);
            if (root != 0 && root != tempRoot) return true;
            root = tempRoot;
        }
        return false;
    }
    static bool haveCycleChain(const vector<NodePair>& pairs) {
        vector<string> paths = getAllPaths(pairs);
        for (int i = 0; i < paths.size(); ++i) {
            string path = paths.at(i);
            char c1 = path.at(0);
            char c2 = path.at(path.size()-1);
            for (int j = i+1; j < paths.size(); ++j) {
                string path1 = paths.at(j);
                char d1 = path1.at(0);
                char d2 = path1.at(path1.size()-1);
                if (path == path1) continue;
                if ((path.find(d1) != string::npos && path.find(d2) != string::npos) ||
                    (path1.find(c1) != string::npos && path1.find(c2) != string::npos)) {
                    return true;
                }
            }
        }
        return false;
    }
    static Node* BuildTree(const vector<NodePair>& pairs) {
        vector<NodePair> cp_pairs = pairs;
        char tempRoot = pairs.at(0).parent;
        string path;
        char root = findRoot(cp_pairs, tempRoot, path);
        Node* pRoot = new Node();
        pRoot->value = root;
        buildNode(pairs, pRoot);
        return pRoot;
    }
    static void ReleaseNode(Node* p) {
        if (p != NULL) {
            ReleaseNode(p->left);
            ReleaseNode(p->right);
            delete p;
            p = NULL;
        }
    }

private:
    static char findRoot(vector<NodePair> &pairs, char root, string& path) {
        // update path
        path = root + path;
        for (vector<NodePair>::iterator it = pairs.begin(); it != pairs.end(); ++it) {
            if (root == it->child) {
                root = it->parent;
                pairs.erase(it);
                return findRoot(pairs, root, path);
            }
        }
        return root;
    }

   static vector<string> getAllPaths(const vector<NodePair>& pairs) {
        vector<string> paths;
        for (int i = 0; i < pairs.size(); ++i) {
            vector<NodePair> cp_pairs;
            const NodePair& curPair = pairs.at(i);
            char tempRoot = curPair.parent;
            copy(pairs.begin(), pairs.end(), back_inserter(cp_pairs));

            string path;
            findRoot(cp_pairs, tempRoot, path);
            cp_pairs.clear();
            copy(pairs.begin(), pairs.end(), back_inserter(cp_pairs));
            // since one parnet may have 2 children, we need to make sure
            // current pair's child is appended to the path!
            path += curPair.child;
            addChildPath(cp_pairs, path.at(path.length()-1), path);
            paths.push_back(path);
        }
        return paths;
    }
   static char addChildPath(vector<NodePair>& pairs, char child, string& path) {
        for (vector<NodePair>::iterator it = pairs.begin(); it != pairs.end(); ++it) {
            if (child == it->parent) {
                child = it->child;
                pairs.erase(it);
                path += child;
                return addChildPath(pairs, child, path);
            }
        }
        return child;
   }

   static void buildNode(const vector<NodePair>& pairs, Node* pRoot) {
        if (pRoot == NULL) return;
        for (int i = 0; i < pairs.size(); ++i) {
            const NodePair& pair = pairs.at(i);
            if (pRoot->value == pair.parent) {
                Node* p = new Node();
                p->value = pair.child;
                if (pRoot->left == NULL) {
                    pRoot->left = p;
                } else if (pRoot->right == NULL) {
                    if (p->value > pRoot->left->value) {
                        pRoot->right = p;
                    } else {
                        pRoot->right = pRoot->left;
                        pRoot->left = p;
                    }
                } else break;
            }
        }
        // boths children nodes are ready
        buildNode(pairs, pRoot->left);
        buildNode(pairs, pRoot->right);
    }
};

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    string input;
    getline(cin, input);
    //char input[1024];
    //memset(input, 0, 1024);
    //cin >> input;
    vector<NodePair> pairs;
    bool success = Parser::Parse(input, pairs);
    if (!success) {
        cout << "E1" << endl;
    }
    bool duplicatePairs = Parser::haveDuplicatePairs(pairs);
    bool moreChildren = Parser::haveMoreChildren(pairs);
    bool multipleRoots = Parser::haveMultipleRoots(pairs);
    bool cycleChain = Parser::haveCycleChain(pairs);
    if (duplicatePairs)
        cout << "E2" << endl;
    if (moreChildren)
        cout << "E3" << endl;
    if (multipleRoots)
        cout << "E4" << endl;
    if (cycleChain)
        cout << "E5" << endl;
    if (!duplicatePairs && !moreChildren && !multipleRoots && !cycleChain) {
        Node* pRoot = Parser::BuildTree(pairs);
        string exp = pRoot->GetSExpression();
        cout << exp << endl;
    }
    return 0;
}
void Test();
/*
int main()
{
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
*/
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
