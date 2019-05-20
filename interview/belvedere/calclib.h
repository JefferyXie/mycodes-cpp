#ifndef CALCLIB_H
#define CALCLIB_H

#include "header.h"

namespace MathLibrary {

struct Math {
    static double IsPrime(int64_t n) {
        if(n <= 3) return true;

        if(((n % 2) == 0) || ((n % 3) == 0))
            return false;

        int64_t i = 5;
        while(i * i <= n) {
            if(((n % i) == 0) || ((n % (i + 2)) == 0))
                return false;

            i = i + 6;
        }
        return true;
    }
};

}

//
// additional test case
namespace calc_lib {

struct extra_data {
    char arr[10] = {'\0'};
};

std::pair<int, std::string>
my_calc(const std::string& s, int i, const extra_data& t) {
    char buf[256];
    snprintf(buf, sizeof(buf), "%s-%d-%s", s.c_str(), i, t.arr);
    return {i, buf};
}

}

#endif

