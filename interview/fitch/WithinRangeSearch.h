#ifndef WITHINRANGESEARCH_H
#define WITHINRANGESEARCH_H

#include "./include.h"
#include "./ISearch.h"

class WithinRangeSearch : public ISearch {
public:
    int Search(const vector<string>& words, const string& w1, const string& w2) {
        if (words.empty()) return 0;

        string target;
        const string& firstWd = words.at(0);
        if (firstWd == w1) target = w2;
        else if (firstWd == w2) target = w1;
        else return 0;

        return count(begin(words)+1, end(words), target);
    }
};
class OtherSearchStrategy : public ISearch {
    int Search(const vector<string>&, const string&, const string&) { return 0; }
};


#endif

