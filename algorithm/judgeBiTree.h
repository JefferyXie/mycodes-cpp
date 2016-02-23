#ifndef JUDGEBITREE_H
#define JUDGEBITREE_H

#include "../header.h"

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
        //return getSExpression(this);
        return '(' + getSExpression(this) + ')';
    }
private:
    string getSExpression(Node* pNode) {
        if (pNode == NULL) return "";
        string exp;
        exp += pNode->value;
        if (pNode->left != NULL) {
            exp += '(';
            exp += getSExpression(pNode->left);
            exp += ')';
        }
        if (pNode->right != NULL) {
            exp += '(';
            exp += getSExpression(pNode->right);
            exp += ')';
        }
        return exp;
        /*string exp;
        if (pNode == NULL) {
            exp += ')';
            return exp;
        }
        exp += '(';
        exp += pNode->value;
        exp += getSExpression(pNode->left);
        exp += getSExpression(pNode->right);
        return exp;*/
    }
};

class Parser {
private:
    string _input;
    vector<NodePair> _pairs;
    Node* _TreeRoot;
    
public:
    Parser(string input) : _input(input), _TreeRoot(NULL) {}
    ~Parser() {
        releaseNode(_TreeRoot);
    }

    bool Parse(){
        string input = _input;
        if (input.length() < 5) return false;
        int pos = 0;
        _pairs.clear();
        while (true) {
            int pos1 = input.find(' ', pos);
            string strNode = input.substr(pos, pos1-pos);

            // parse and get node info
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
            _pairs.push_back(nodePair);
            pos = pos1+1;
            if (pos1 < 0) break;
        }
        return true;
    }

    bool HaveDuplicatePairs() {
        for (ulong i = 0; i < _pairs.size(); ++i) {
            const NodePair& pair1 = _pairs.at(i);
            for (ulong j = i + 1; j < _pairs.size(); ++j) {
                const NodePair& pair2 = _pairs.at(j);
                if ((pair1.parent == pair2.parent && pair1.child == pair2.child) ||
                   (pair1.parent == pair2.child && pair1.child == pair2.parent)) {
                    return true;
                }
            }
        }
        return false;
    }

    bool HaveMoreChildren() {
        for (ulong i = 0; i < _pairs.size(); ++i) {
            int numChildren = 1;
            const NodePair& pair1 = _pairs.at(i);
            for (ulong j = i+1; j < _pairs.size(); ++j) {
                const NodePair& pair2 = _pairs.at(j);
                if (pair1.parent == pair2.parent) {
                    numChildren++;
                }
                if (numChildren > 2) return true;
            }
        }
        return false;
    }

    bool HaveMultipleRoots() {
        char root = 0;
        for (ulong i = 0; i < _pairs.size(); ++i) {
            vector<NodePair> cp_pairs;
            char tempRoot = _pairs.at(i).parent;
            copy(_pairs.begin(), _pairs.end(), back_inserter(cp_pairs));
            string path;
            tempRoot = findRoot(cp_pairs, tempRoot, path);
            if (root != 0 && root != tempRoot) return true;
            root = tempRoot;
        }
        return false;
    }

    bool HaveCycleChain() {
        vector<string> paths = getAllPaths(_pairs);
        for (ulong i = 0; i < paths.size(); ++i) {
            string path = paths.at(i);
            char c1 = path.at(0);
            char c2 = path.at(path.size()-1);
            for (ulong j = i+1; j < paths.size(); ++j) {
                string path1 = paths.at(0);
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

    Node* BuildTree() {
        vector<NodePair> cp_pairs = _pairs;
        char tempRoot = _pairs.at(0).parent;
        string path;
        char root = findRoot(cp_pairs, tempRoot, path);
        Node* pRoot = new Node();
        pRoot->value = root;
        buildNode(_pairs, pRoot);
        _TreeRoot = pRoot;
        return pRoot;
    }

private:
    void releaseNode(Node* p) {
        if (p != NULL) {
            releaseNode(p->left);
            releaseNode(p->right);
            delete p;
            p = NULL;
        }
    }
    char findRoot(vector<NodePair>& pairs, char root, string& path) {
        // avoid cycle path
        if (path.find(root) != string::npos) return root;

        path = root + path;
        for (auto it = pairs.begin(); it != pairs.end(); ++it) {
            if (root == it->child) {
                root = it->parent;
                pairs.erase(it);
                return findRoot(pairs, root, path);
            }
        }
        return root;
    }

    vector<string> getAllPaths(const vector<NodePair>& pairs) {
        vector<string> paths;
        for (ulong i = 0; i < pairs.size(); ++i) {
            vector<NodePair> cp_pairs;
            const NodePair& curPair = pairs.at(i);
            char tempRoot = curPair.parent;
            copy (pairs.begin(), pairs.end(), back_inserter(cp_pairs));

            string path;
            findRoot(cp_pairs, tempRoot, path);
            cp_pairs.clear();
            copy(pairs.begin(), pairs.end(), back_inserter(cp_pairs));
            if (path.find(curPair.child) == string::npos) {
                // since one parent may have 2 children, we need to make
                // sure current pair's child is appended to the path!
                path += curPair.child;
            }
            addChildPath(cp_pairs, path.at(path.length()-1), path);
            paths.push_back(path);
        }
        return paths;
    }
    char addChildPath(vector<NodePair>& pairs, char child, string& path) {
        for (auto it = pairs.begin(); it != pairs.end(); ++it) {
            if (child == it->parent) {
                child = it->child;
                pairs.erase(it);
                // avoid cycle path
                if (path.find(child) != string::npos) return child;
                path += child;
                return addChildPath(pairs, child, path);
            }
        }
        return child;
    }
    void buildNode(const vector<NodePair>& pairs, Node* pRoot) {
        if (pRoot == NULL) return;
        for (ulong i = 0; i < pairs.size(); ++i) {
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

string JudgeBiTree(string input) {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    /*string input;
    getline(cin, input);*/

    Parser parser(input);
    if (!parser.Parse()) {
        return "E1";
    }
    bool duplicatePairs = parser.HaveDuplicatePairs();
    bool moreChildren = parser.HaveMoreChildren();
    bool multipleRoots = parser.HaveMultipleRoots();
    bool cycleChain = parser.HaveCycleChain();
    if (duplicatePairs) {
        return "E2";
    }
    if (moreChildren) {
        return "E3";
    }
    if (multipleRoots) {
        return "E4";
    }
    if (cycleChain) {
        return "E5";
    }
    Node* pRoot = parser.BuildTree();
    string exp = pRoot->GetSExpression();
    return exp;
}

#endif

