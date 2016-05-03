#ifndef MYQUEUE_H
#define MYQUEUE_H

#include "../main/header.h"

#define DEFAULT_QUEUE_SIZE 2

template <class T> class MyQueue {
public:
    MyQueue() {
        //_head = new T[DEFAULT_QUEUE_SIZE];
        _head = new T*[DEFAULT_QUEUE_SIZE];
        _offset = 0;
        _length = 0;
        _capacity = DEFAULT_QUEUE_SIZE;
    }    
    ~MyQueue() {
        for (int i = 0; i < _length+_offset; ++i) {
            delete _head[i];
            _head[i] = NULL;
        }
        reset();
    }

    void Push(const T& v);
    void Push(T&& v);
    T Pop();

    int Length() { return _length; }
    string ToString();

private:
    void enlarge();
    void reset();
    T** _head;
    int _offset;
    int _length;
    int _capacity;
    mutex _mu;
};

template<class T> void MyQueue<T>::Push(const T& v) {
    lock_guard<mutex> lg(_mu);
    if (_length + _offset >= _capacity) {
        enlarge();
    }
    //_head[_length+_offset] = v;
    _head[_length+_offset] = new T(v);
    _length++;
}
template<class T> void MyQueue<T>::Push(T&& v) {
    lock_guard<mutex> lg(_mu);
    if (_length + _offset >= _capacity) {
        enlarge();
    }
    //_head[_length+_offset] = v;
    //_head[_length+_offset] = new T(move(v));
    _head[_length+_offset] = new T(forward<T>(v));
    _length++;
}

template<class T> T MyQueue<T>::Pop() {
    while (_length <= 0) {
        cout << "yield" << endl;
        this_thread::sleep_for(chrono::seconds(1));
        //this_thread::yield();
    }
    lock_guard<mutex> lg(_mu);
    _length--;
    return *(_head[_offset++]);
}

template<class T> string MyQueue<T>::ToString() {
    string str;
    str += "capacity:" + to_string(_capacity);
    str += ",offset:" + to_string(_offset);
    str += ",length:" + to_string(_length);
    return str;
}

template<class T> void MyQueue<T>::enlarge() {
    if (_length+_offset < _capacity) return;

    _capacity *= 2;
    T** newHead = new T*[_capacity];
    memcpy(newHead, _head+_offset, _length*sizeof(T*));
    reset();
    _head = newHead;
    _offset = 0;
}

template<class T> void MyQueue<T>::reset() {
    delete[] _head;
    _head = NULL;
}

#endif


