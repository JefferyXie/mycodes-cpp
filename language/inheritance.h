#pragma once

#include "../core/header.h"

struct WWW {
    int          w = 10;
    virtual void foo() {}
};

struct XXX : virtual public WWW {
    int x = 20;
};
struct YYY : virtual public WWW {
    int y = 30;
};
struct ZZZ : public XXX, public YYY {
    int z = 40;
};

void run_inheritance()
{
    // static_assert(sizeof(WWW) == 4);
    // static_assert(sizeof(XXX) == 16);
    // static_assert(sizeof(ZZZ) == 40);

    static_assert(sizeof(WWW) == 16);
    static_assert(sizeof(XXX) == 32);
    static_assert(sizeof(ZZZ) == 48);
}

