#ifndef JUDGEBITREE_H
#define JUDGEBITREE_H

#include "../../core/header.h"

// this question is online coding quiz from Optiver
struct NodePair {
    char parent;
    char child;
};

class Node
{
public:
    char        value;
    Node*       left;
    Node*       right;
    std::string GetSExpression()
    {
        // return getSExpression(this);
        return '(' + getSExpression(this) + ')';
    }

private:
    std::string getSExpression(Node* pNode)
    {
        if (pNode == NULL)
            return "";
        std::string exp;
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
        /*std::string exp;
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

class Parser
{
private:
    std::string           _input;
    std::vector<NodePair> _pairs;
    Node*                 _TreeRoot;

public:
    Parser(std::string input) : _input(input), _TreeRoot(NULL) {}
    ~Parser() { releaseNode(_TreeRoot); }

    bool Parse()
    {
        std::string input = _input;
        if (input.length() < 5)
            return false;
        int pos = 0;
        _pairs.clear();
        while (true) {
            int         pos1    = input.find(' ', pos);
            std::string strNode = input.substr(pos, pos1 - pos);
            // parse and get node info
            if (strNode.length() != 5 || strNode.at(0) != '(' || strNode.at(1) > 'Z' || strNode.at(1) < 'A' ||
                strNode.at(2) != ',' || strNode.at(3) > 'Z' || strNode.at(3) < 'A' || strNode.at(4) != ')') {
                return false;
            }
            NodePair nodePair;
            nodePair.parent = strNode.at(1);
            nodePair.child  = strNode.at(3);
            _pairs.push_back(nodePair);
            pos = pos1 + 1;
            if (pos1 < 0)
                break;
        }
        return true;
    }

    bool HaveDuplicatePairs()
    {
        for (unsigned i = 0; i < _pairs.size(); ++i) {
            const NodePair& pair1 = _pairs.at(i);
            for (unsigned j = i + 1; j < _pairs.size(); ++j) {
                const NodePair& pair2 = _pairs.at(j);
                if ((pair1.parent == pair2.parent && pair1.child == pair2.child) ||
                    (pair1.parent == pair2.child && pair1.child == pair2.parent)) {
                    return true;
                }
            }
        }
        return false;
    }

    bool HaveMoreChildren()
    {
        for (unsigned i = 0; i < _pairs.size(); ++i) {
            int             numChildren = 1;
            const NodePair& pair1       = _pairs.at(i);
            for (unsigned j = i + 1; j < _pairs.size(); ++j) {
                const NodePair& pair2 = _pairs.at(j);
                if (pair1.parent == pair2.parent) {
                    numChildren++;
                }
                if (numChildren > 2)
                    return true;
            }
        }
        return false;
    }

    bool HaveMultipleRoots()
    {
        char root = 0;
        for (unsigned i = 0; i < _pairs.size(); ++i) {
            char        tempRoot = _pairs.at(i).parent;
            std::string path;
            tempRoot = findRoot(_pairs, tempRoot, path);
            if (root != 0 && root != tempRoot) {
                NodePair node;
                node.parent = root;
                node.child  = tempRoot;
                // if two nodes connect, they make cycle instead of multiple roots
                if (!isNodeExisted(node))
                    return true;
            }
            root = tempRoot;
        }
        return false;
    }

    bool HaveCycleChain()
    {
        std::vector<std::string> paths = getAllPaths(_pairs);
        for (unsigned i = 0; i < paths.size(); ++i) {
            std::string path = paths.at(i);
            char        c1   = path.front();
            char        c2   = path.back();
            for (unsigned j = i + 1; j < paths.size(); ++j) {
                std::string path1 = paths.at(j);
                char        d1    = path1.front();
                char        d2    = path1.back();
                if (path == path1)
                    continue;
                if ((path.find(d1) != std::string::npos && path.find(d2) != std::string::npos) ||
                    (path1.find(c1) != std::string::npos && path1.find(c2) != std::string::npos)) {
                    return true;
                }
            }
        }
        return false;
    }

    Node* BuildTree()
    {
        std::string path;
        char        tempRoot = _pairs.at(0).parent;
        char        root     = findRoot(_pairs, tempRoot, path);
        Node*       pRoot    = new Node();
        pRoot->value         = root;
        buildNode(_pairs, pRoot);
        _TreeRoot = pRoot;
        return pRoot;
    }

private:
    void releaseNode(Node* p)
    {
        if (p != NULL) {
            releaseNode(p->left);
            releaseNode(p->right);
            delete p;
            p = NULL;
        }
    }
    char findRoot(const std::vector<NodePair>& pairs, char root, std::string& path)
    {
        // avoid cycle path
        if (path.find(root) != std::string::npos)
            return root;

        path    = root + path;
        auto it = pairs.end();
        while (it-- != pairs.begin()) {
            if (root == it->child) {
                root = it->parent;
                return findRoot(pairs, root, path);
            }
        }
        return root;
    }

    bool isNodeExisted(const NodePair& node)
    {
        for (const NodePair& ele : _pairs) {
            if ((node.parent == ele.parent && node.child == ele.child) ||
                (node.parent == ele.child && node.child == ele.parent))
                return true;
        }
        return false;
    }

    std::vector<std::string> getAllPaths(const std::vector<NodePair>& pairs)
    {
        std::vector<std::string> paths;
        for (unsigned i = 0; i < pairs.size(); ++i) {
            const NodePair& curPair   = pairs.at(i);
            char            curParent = curPair.parent;
            char            curChild  = curPair.child;
            std::string     path(1, curChild);
            findRoot(pairs, curParent, path);

            addChildPath(pairs, path.back(), path);

            paths.push_back(path);
        }
        return paths;
    }
    char addChildPath(const std::vector<NodePair>& pairs, char child, std::string& path)
    {
        for (auto it = pairs.begin(); it != pairs.end(); ++it) {
            if (child == it->parent) {
                child = it->child;
                // avoid cycle path
                if (path.find(child) != std::string::npos)
                    return child;
                path += child;
                return addChildPath(pairs, child, path);
            }
        }
        return child;
    }
    void buildNode(const std::vector<NodePair>& pairs, Node* pRoot)
    {
        if (pRoot == NULL)
            return;
        for (unsigned i = 0; i < pairs.size(); ++i) {
            const NodePair& pair = pairs.at(i);
            if (pRoot->value == pair.parent) {
                Node* p  = new Node();
                p->value = pair.child;
                if (pRoot->left == NULL) {
                    pRoot->left = p;
                } else if (pRoot->right == NULL) {
                    if (p->value > pRoot->left->value) {
                        pRoot->right = p;
                    } else {
                        pRoot->right = pRoot->left;
                        pRoot->left  = p;
                    }
                } else
                    break;
            }
        }
        // boths children nodes are ready
        buildNode(pairs, pRoot->left);
        buildNode(pairs, pRoot->right);
    }
};

std::string JudgeBiTree(std::string input)
{
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    /*std::string input;
    getline(cin, input);*/

    Parser parser(input);
    if (!parser.Parse()) {
        return "E1";
    }
    bool duplicatePairs = parser.HaveDuplicatePairs();
    bool moreChildren   = parser.HaveMoreChildren();
    bool multipleRoots  = parser.HaveMultipleRoots();
    bool cycleChain     = parser.HaveCycleChain();
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
    Node*       pRoot = parser.BuildTree();
    std::string exp   = pRoot->GetSExpression();
    return exp;
}

#endif

