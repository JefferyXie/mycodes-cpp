#ifndef FILEREADER_H
#define FILEREADER_H

#include "./include.h"

class FileReader {
public:
    ~FileReader() {
        if (_fs.is_open()) _fs.close();
    }
    bool Open(char* file) {
        _fs.open(file);
        return _fs.is_open();
    }
    template<class T>
    bool Read(T& buf) {
        return Read(buf, sizeof(buf));
    }
    template<class T>
    bool Read(T& buf, int length) {
        if (_fs.is_open()) {
            _fs.read((char*)&buf, length);
            return IsGood();
        }
        return false;
    }
    bool IsGood() {
        return _fs.good();
    }

private:
    ifstream _fs;
};

#endif

