#pragma once

#include "../core/header.h"

// CRTP:  client class inherits from CRTP base, so impacts existing code
// Mixin: inherits from existing class and provides extra functionality, leave existing code unchanged
template <typename Derived>
struct CRTP {
    template <typename... Args>
    void do_work(Args&&... args)
    {
        // you many have extra work here before the call ...
        static_cast<Derived*>(this)->some_fun(std::forward<Args>(args)...);
    }
};
struct CRTP_Client : public CRTP<CRTP_Client> {
    void some_fun(int, std::string)
    {
        //...
    }
};

template <typename Impl>
struct Mixin : public Impl {
    template <typename... Args>
    void do_work(Args... args)
    {
        // you many have extra work here before the call ...
        static_cast<Impl*>(this)->some_fun(args...);
    }
};
struct Existing_Client {
    void some_fun(int, std::string)
    {
        //...
    }
};
using Mixin_Client = Mixin<Existing_Client>;

