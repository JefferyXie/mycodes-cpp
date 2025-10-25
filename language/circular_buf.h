#ifndef CIRCULAR_BUF_H
#define CIRCULAR_BUF_H

#include "../main/header.h"

//
// https://embeddedartistry.com/blog/2017/4/6/circular-buffers-in-cc
//
// A basic non-threading-safe circular buffer
//
class circular_buf
{
public:
    circular_buf() : _length(0), _offset(0) {}

    int write(const char* ch, int s)
    {
        int cap = get_buf_size();
        if (s + _length > cap)
            return -1;

        std::lock_guard<std::mutex> lg(_mu);
        // double check in case multiple writers
        if (s + _length > cap)
            return -1;

        for (int i = 0; i < s; ++i) {
            int pos    = (_length + _offset + i) % cap;
            _buff[pos] = *(ch + i);
        }
        _length += s;
        return s;
    }

    char read()
    {
        if (_length <= 0)
            return '\0';

        std::lock_guard<std::mutex> lg(_mu);
        // double check in case multiple readers
        if (_length <= 0)
            return '\0';

        _length--;
        char ch = _buff[_offset];
        _offset = (++_offset) % get_buf_size();
        return ch;
    }

private:
    int get_buf_size() { return BUFFERSIZE; }

private:
    // TODO: use array pointer than fixed size array
    // _capacity = size;
    // char* _buff = nullptr;
    // _buff = new T[_capacity];
    // delete[] _buff;
    static const int BUFFERSIZE        = 16;
    char             _buff[BUFFERSIZE] = {0};
    int              _length;
    int              _offset;
    std::mutex       _mu;
};

#endif

