#ifndef META_H
#define META_H

#include "../main/header.h"

//
// Template Meta Programming (TMP)
//
//

// 
// FACTORIAL
// 
template<unsigned int N>
struct FACTORIAL {
    enum { VALUE = N * FACTORIAL<N-1>::VALUE };
};
template<>
struct FACTORIAL<1> {
    enum { VALUE = 1  };
};

// 
// BINARY: convert binary number into decimal
// 
template< unsigned long long N >
struct BINARY {
    enum { VALUE = (N % 10) + 2 * BINARY< N / 10 >::VALUE };
};
template<>
struct BINARY<0> {
    enum { VALUE = 0 };
};

// 
// IS_PRIME
// 
template <int p, int i>
struct IS_PRIME {
	enum { VALUE = ( (p % i) && IS_PRIME<p, i - 1>::VALUE ) }; 
}; 
template <int p>
struct IS_PRIME<p, 1> {
    enum { VALUE = 1 };
}; 

//
// PRINT_PRIME
//
template <int i>
struct PRINT_PRIME {
// primary template for loop to print prime numbers
	PRINT_PRIME<i - 1> a; 
	enum { VALUE = IS_PRIME<i, i - 1>::VALUE };
	void f() {
		a.f();
		if (VALUE) {
			std::cout << i << " ";
		}
	} 
}; 
template<>
struct PRINT_PRIME<1> {
// full specialization to end the loop
	enum { VALUE = 0 };
	void f() {}
}; 

// 
// SAME_TYPE
// 
template<typename X, typename Y>
struct SAME_TYPE {
    enum { VALUE = 0 };
};
template<typename T>
struct SAME_TYPE<T, T> {
    enum { VALUE = 1 };
};

// 
// IF
// 
template<bool COND, typename TRUE_TYPE, typename FALSE_TYPE>
struct IF {
    typedef TRUE_TYPE TYPE;
};
template<typename TRUE_TYPE, typename FALSE_TYPE>
struct IF<false, TRUE_TYPE, FALSE_TYPE> {
    typedef FALSE_TYPE TYPE;
};

// 
// SWITCH
// 
template< int _case >
class SWITCH {
public:
    static inline void EXEC() {
        cout << " SWITCH - default ";
    }
};
template<>
class SWITCH< 1 > {
public:
    static inline void EXEC() {
        cout << " SWITCH - 1 ";
    }
};
template<>
class SWITCH< 2 > {
public:
    static inline void EXEC() {
        cout << " SWITCH - 2 ";
    }
};

void run_meta()
{
    // 
    // FACTORIAL
    std::cout << "\n[FACTORIAL]" << std::endl;
    std::cout << FACTORIAL<5>::VALUE << '\n';
    // Because calculations are done at compile-time, they can be used for things such as array sizes.
    char arr0[FACTORIAL<6>::VALUE] = {0};
    std::cout << "arr0 lenght: " << sizeof(arr0)/sizeof(arr0[0]) << '\n';

    // 
    // BINARY
    std::cout << "\n[BINARY]" << std::endl;
    std::cout << sizeof(unsigned long long) << std::endl;
    std::cout << "max(unsigned long long): " << std::numeric_limits<unsigned long long>::max() << '\n';
    std::cout << "min(unsigned long long): " << std::numeric_limits<unsigned long long>::min() << '\n';

    std::cout << std::fixed << "pow(2, 19): " << pow(2, 19) << std::endl;
    std::cout << "BINARY<10..>: " << BINARY<10000000000000000000>::VALUE << std::endl;
    
    // ull max lenght is 20, so cannot convert it correctly
    std::cout << std::fixed << "pow(2, 20): " << pow(2, 20) << std::endl;
    std::cout << "BINARY<10..>: " << BINARY<100000000000000000000>::VALUE << std::endl;

    // 
    // IS_PRIME & PRINT_PRIME
    std::cout << "\n[IS_PRIME & PRINT_PRIME]" << std::endl;
    std::cout << "print primes (0, 5): ";
    PRINT_PRIME<5> pri_5;
    pri_5.f();
    std::cout << "\nprint primes (0, 15): ";
    PRINT_PRIME<15> pri_15;
    pri_15.f();
    std::cout << '\n';

    // 
    // SAME_TYPE
    std::cout << "\n[SAME_TYPE]" << std::endl;
    if (SAME_TYPE<int, long>::VALUE)
    {
        std::cout << "<int, long> is same type." << '\n';
    }
    else
    {
        std::cout << "<int, long> is NOT same type." << '\n';
    }
    if (SAME_TYPE<std::map<int, string>::key_type, int>::VALUE)
    {
        std::cout << "<std::map<int, string>::key_type, int> is same type." << '\n';
    }
    else
    {
        std::cout << "<std::map<int, string>::key_type, int> is NOT same type." << '\n';
    }

    // 
    // IF
    std::cout << "\n[IF]" << std::endl;
    IF<true, int, string>::TYPE i = 10;
    IF<false,int, string>::TYPE s = "20";

    // 
    // SWITCH
    std::cout << "\n[SWITCH]" << std::endl;
    SWITCH<0> sw0;
    SWITCH<1> sw1;
    SWITCH<2> sw2;
    SWITCH<3> sw3;
    sw0.EXEC();
    sw1.EXEC();
    sw2.EXEC();
    sw3.EXEC();

    std::cout << '\n';
}

#endif

