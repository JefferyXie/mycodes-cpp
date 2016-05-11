#ifndef MYSMARTPOINTER_H
#define MYSMARTPOINTER_H

#include "../main/header.h"

template<class T> class MySmartPointer {
public:
    MySmartPointer() : _object(nullptr), _refCount(nullptr) { }
    MySmartPointer(T* p) : _object(p), _refCount(new int(1)) { }
    MySmartPointer(const MySmartPointer& o) : MySmartPointer() {
        _copy(o);
    }
    MySmartPointer& operator=(const MySmartPointer& o) {
        _copy(o);
        return *this;
    }
    MySmartPointer(MySmartPointer&& o) : MySmartPointer() {
        _object = o._object;
        _refCount = o._refCount;
        o._object = nullptr;
        o._refCount = nullptr;
    }
    MySmartPointer& operator=(MySmartPointer&& o) {
        _decreaseRef();
        _object = o._object;
        _refCount = o._refCount;
        o._object = nullptr;
        o._refCount = nullptr;
        return *this;
    }
    ~MySmartPointer() {
        _decreaseRef();
    }

    T* operator->() { return get(); }
    T& operator*() { return *get(); }
    // the returned T* shouldn't be used to create another instance with MySmartPointer(T*)
    // or reset(T*)!!
    T* get() { return _object; } 
    // p shouldn't be used to create another instance with MySmartPointer(T*)
    // or reset(T*)!!
    void reset(T* p) {
        _decreaseRef();
        _object = p;
        _refCount = new int(1);
    }
    int use_count() { return nullptr == _refCount ? 0 : *_refCount; }

private:
    void _copy(const MySmartPointer& o) {
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


