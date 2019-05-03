#ifndef CIRCULARBUF_H
#define CIRCULARBUF_H

#include "../main/header.h"

// 
// https://embeddedartistry.com/blog/2017/4/6/circular-buffers-in-cc
// 
class CircularBuffer {
public:
    CircularBuffer() : _length(0), _offset(0) { }

    int Write(const char* ch, int s) {
        int cap = getBufSize();
        if (s + _length > cap) return -1;

        lock_guard<mutex> lg(_mu);
        // double check in case multiple writers
        if (s + _length > cap) return -1;

        for (int i = 0; i < s; ++i) {
            int pos = (_length+_offset+i) % cap;
            _buff[pos] = *(ch+i);
        }
        _length += s;
        return s;
    }

    char Read() {
        if (_length <= 0) return '\0';

        lock_guard<mutex> lg(_mu);
        // double check in case multiple readers
        if (_length <= 0) return '\0';

        _length--;
        char ch = _buff[_offset];
        _offset = (++_offset) % getBufSize();
        return ch;
    }

private:
    int getBufSize() { return BUFFERSIZE; }

private:
	// TODO: use array pointer than fixed size array
	// _capacity = size;
	// char* _buff = nullptr;
	// _buff = new T[_capacity];
	// delete[] _buff;
    static const int BUFFERSIZE = 16;
    char _buff[BUFFERSIZE] = {0};
    int _length;
    int _offset;
    mutex _mu;
};

#endif


