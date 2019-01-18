#ifndef MY_SMARTPOINTER_H
#define MY_SMARTPOINTER_H 

#include "../main/header.h"

template<class T> class my_smartpointer {
public:
    my_smartpointer() : _object(nullptr), _refCount(nullptr) { }
    my_smartpointer(T* p) : _object(p), _refCount(new int(1)) { }
    my_smartpointer(const my_smartpointer& o) : my_smartpointer() {
        _copy(o);
    }
    my_smartpointer& operator=(const my_smartpointer& o) {
        _copy(o);
        return *this;
    }
    my_smartpointer(my_smartpointer&& o) : my_smartpointer() {
        _object = o._object;
        _refCount = o._refCount;
        o._object = nullptr;
        o._refCount = nullptr;
    }
    my_smartpointer& operator=(my_smartpointer&& o) {
        _decreaseRef();
        _object = o._object;
        _refCount = o._refCount;
        o._object = nullptr;
        o._refCount = nullptr;
        return *this;
    }
    ~my_smartpointer() {
        _decreaseRef();
    }

    T* operator->() { return get(); }
    T& operator*() { return *get(); }
    // the returned T* shouldn't be used to create another instance with my_smartpointer(T*)
    // or reset(T*)!!
    T* get() { return _object; } 
    // p shouldn't be used to create another instance with my_smartpointer(T*)
    // or reset(T*)!!
    void reset(T* p) {
        _decreaseRef();
        _object = p;
        _refCount = new int(1);
    }
    int use_count() { return nullptr == _refCount ? 0 : *_refCount; }

private:
    void _copy(const my_smartpointer& o) {
        _decreaseRef();
        _object = o._object;
        _refCount = o._refCount;
        _increaseRef();
    }
    int _increaseRef() { if (nullptr != _refCount) ++(*_refCount); return use_count(); }
    int _decreaseRef() {
        if (nullptr != _refCount && !--(*_refCount)) {
            delete _object;
            _object = nullptr;
            delete _refCount;
            _refCount = nullptr;
        }
        return use_count();
    }

private:
    T* _object;
    int* _refCount;
};

#endif


