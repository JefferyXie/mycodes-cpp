#ifndef IFETCHWORDS_H
#define IFETCHWORDS_H

#include "./include.h"

class IFetchWords {
public:
    virtual bool SetSource(const void* src) = 0;
    virtual vector<string> Fetch(int count) = 0;
    virtual ~IFetchWords() {};
};

#endif

