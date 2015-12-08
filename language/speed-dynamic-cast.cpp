/*
* Copyright 2010 Tino Didriksen <tino@didriksen.cc>
* http://tinodidriksen.com/
*/

// 
// C++ dynamic_cast performance
// http://tinodidriksen.com/2010/04/14/cpp-dynamic-cast-performance/
// http://tinodidriksen.com/uploads/code/cpp/speed-dynamic-cast.cpp
//
// How expensive is RTTI?
// http://stackoverflow.com/questions/579887/how-expensive-is-rtti
//
#ifdef _MSC_VER
    #define _SECURE_SCL 0
    #define _CRT_SECURE_NO_DEPRECATE 1
    #define WIN32_LEAN_AND_MEAN
    #define VC_EXTRALEAN
    #define NOMINMAX
#endif

#include <cstdlib>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <limits>
#include <vector>
#include <iomanip>
#include "cycle.h"

const size_t N = 1000000;
const size_t R = 7;
std::vector<uint32_t> numbers;

void PrintStats(std::vector<double> timings) {
    double fastest = std::numeric_limits<double>::max();

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "[";
    for (size_t i = 1 ; i<timings.size()-1 ; ++i) {
        fastest = std::min(fastest, timings[i]);
        std::cout << timings[i] << ",";
    }
    std::cout << timings.back();
    std::cout << "]";

    double sum = 0.0;
    for (size_t i = 1 ; i<timings.size() ; ++i) {
        sum += timings[i];
    }
    double avg = sum / double(timings.size()-1);

    sum = 0.0;
    for (size_t i = 1 ; i<timings.size() ; ++i) {
        timings[i] = pow(timings[i]-avg, 2);
        sum += timings[i];
    }
    double var = sum/(timings.size()-2);
    double sdv = sqrt(var);

    std::cout << " with fastest " << fastest << ", average " << avg << ", stddev " << sdv;
}

enum TYPES {
    T_ONE_BASE,
    T_ONE_LEVEL1,
    T_ONE_LEVEL2,
    T_ONE_LEVEL3,
    T_TWO_BASE,
    T_TWO_LEVEL1,
};

struct OneBase {
    TYPES type;

    OneBase() : type(T_ONE_BASE) {}
    virtual ~OneBase() {}

    virtual TYPES getType() {
        return T_ONE_BASE;
    }
};

struct OneLevel1 : OneBase {
    OneLevel1() {
        type = T_ONE_LEVEL1;
    }
    virtual TYPES getType() {
        return T_ONE_LEVEL1;
    }
};

struct OneLevel2 : OneLevel1 {
    OneLevel2() {
        type = T_ONE_LEVEL2;
    }
    virtual TYPES getType() {
        return T_ONE_LEVEL2;
    }
};

struct OneLevel3 : OneLevel2 {
    OneLevel3() {
        type = T_ONE_LEVEL3;
    }
    TYPES getType() {
        return T_ONE_LEVEL3;
    }
};

struct TwoBase {
    TYPES type;

    TwoBase() : type(T_TWO_BASE) {}
    virtual ~TwoBase() {}

    virtual TYPES getType() {
        return T_TWO_BASE;
    }
};

struct TwoLevel1 : TwoBase {
    TwoLevel1() {
        type = T_TWO_LEVEL1;
    }
    virtual TYPES getType() {
        return T_TWO_LEVEL1;
    }
};


