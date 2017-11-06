#ifndef ISEARCH_H
#define ISEARCH_H

#include "./include.h"

class ISearch {
public:
    virtual int Search(const vector<string>& words, const string& w1, const string& w2) = 0; 
    virtual ~ISearch() {};
};

#endif


