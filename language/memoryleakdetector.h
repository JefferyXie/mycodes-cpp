#ifndef MEMORYLEAKDETECTOR_H
#define MEMORYLEAKDETECTOR_H

#include "../main/header.h"

#ifndef MEMORYLEAK_ENABLED
#define MEMORYLEAK_ENABLED 0
#endif

#if MEMORYLEAK_ENABLED > 0

struct mem_usage {
    size_t _codeline;
    size_t _size;
    string _type;
    string _file;
    string _function;
    string ToString() const {
        // http://stackoverflow.com/questions/4465872/why-does-typeid-name-return-weird-characters-using-gcc-and-how-to-make-it-prin
        // typeid.name() is implementation dependent, to make it output well,
        // use c++filt -t, for instance, ./a.out | c++filt -t
        return 
            string("[file] ") + _file +
            string(", [function] ") + _function +
            string(", [line] ") + to_string(_codeline) +
            string(", [type] ") + _type +
            string(", [size] ") + to_string(_size);
    }
};

std::unordered_map<unsigned long long, mem_usage> g_mem_usage;

void DumpMemUsage() {
    for_each(begin(g_mem_usage), end(g_mem_usage), [](const pair<unsigned long long, mem_usage>& mu) {
        cout << mu.second.ToString() << endl;
    });
}

// it's not encouraged to return value from macro, instead you should use inline function
// read this article if you really wanna macro return value:
// http://stackoverflow.com/questions/2679182/have-macro-return-a-value
#define NEW(tt) ({ \
        mem_usage mem = { __LINE__, sizeof(tt), string(typeid(tt).name()), __FILE__, __PRETTY_FUNCTION__ }; \
        auto p = new tt; \
        g_mem_usage.insert({(unsigned long long)p, mem}); \
        p; \
    })

#define DEL(p) { \
        if (is_pointer<decltype(p)>::value) { \
            if (p != nullptr) { \
                auto it = g_mem_usage.find((unsigned long long)p); \
                if (it != g_mem_usage.end()) { \
                    g_mem_usage.erase(it); \
                } \
                delete p; p = nullptr; \
            } \
        } \
    }

void* operator new(size_t s) {
    void* p = malloc(s);
    return p;
}

// override global delete
void operator delete(void* p) noexcept {
    free(p);
}

#endif

#endif