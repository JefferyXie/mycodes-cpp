#ifndef FILEFETCHER_H
#define FILEFETCHER_H

#include "./include.h"
#include "IFetchWords.h"

class FileFetcher: public IFetchWords {
public:
    ~FileFetcher() {
        if (_fs.is_open()) _fs.close();
    }

    bool SetSource(const void* src) {
        return open(static_cast<const char*>(src));
    }
    vector<string> Fetch(int count) {
        vector<string> vec;
        while (count-- && isGood()) {
            vec.push_back(readWord());
        }
        return vec;
    }
private:
    bool open(const char* file) {
        _fs.open(file);
        return _fs.is_open();
    }
    string readWord() {
        string word;
        if (_fs.is_open() && isGood()) {
            _fs >> word;
        }
        return word;
    }
    bool isGood() {
        return _fs.good();
    }

    ifstream _fs;
};

class OtherWordsGenerator : public IFetchWords {
public:
    bool SetSource(const void* /*src*/) { return true; }
    vector<string> Fetch(int count) { return vector<string>(count); }
};


#endif


