#ifndef META_H
#define META_H

#include "../main/header.h"

//
// Template Meta Programming (TMP)
// 
// Resource: https://monoinfinito.wordpress.com/series/introduction-to-c-template-metaprogramming/
//

// 
// FACTORIAL
// 
template<unsigned int N>
struct FACTORIAL {
    enum { VALUE = N * FACTORIAL<N-1>::VALUE };
    // Or,
    // static const unsigned int VALUE = N * FACTORIAL<N-1>::VALUE;
    
    // Having as a constraint the fact that all your code must resolve on compile time means
    // only const vars make sense. Also, as you’re working with classes (templates are applied
    // to classes, not objects) only static objects make sense.
};
template<>
struct FACTORIAL<1> {
    enum { VALUE = 1  };
    // Or,
    // static const unsigned int VALUE = 1;
};

// 
// FACTORIAL_SAFE: guarantee input argument positive, otherwise give compile error
// 
template<int N>
struct FACTORIAL_SAFE
{
    template<int M, bool positive>
    struct ENSURE_POSITIVE
    {
        enum { VALUE = FACTORIAL<M>::VALUE };
    };
    // NO implementaton that results in compile error when using
    template<int M>
    struct ENSURE_POSITIVE<M, false>;

    enum { VALUE = ENSURE_POSITIVE<N, N >= 0>::VALUE };
};

// 
// C++11 version: use constexpr
// 
constexpr unsigned int
factorial(int N)
{
    return N ? (N * factorial(N-1)) : 1;
}
constexpr unsigned int
factorial_safe(int N)
{
    // the exception is thrown at compile time (TODO: need to verify this really happens!!)
    return N < 0 ? throw std::invalid_argument("factorial_safe(int N) - N cannot be negative: " + to_string(N)) :
                   factorial(N);
}

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
// MCD
// 
template<int X, int Y>
struct MCD {
    static const int VALUE = MCD<Y, X % Y>::VALUE;
};
template<int X>
struct MCD<X, 0> {
    static const int VALUE = X;
};

// 
// FRACTION
// 
template<int N, int D>
struct FRACTION {
    static const int NUMERATOR   = N / MCD<N, D>::VALUE;
    static const int DENOMINATOR = D / MCD<N, D>::VALUE;
};

// 
// FRACTION_SUM
// 
template<typename FRACT_A, typename FRACT_B>
struct FRACTION_SUM {
    static const int internal_mcd = MCD<FRACT_A::DENOMINATOR, FRACT_B::DENOMINATOR>::VALUE;
    typedef FRACTION<FRACT_A::NUMERATOR * (FRACT_B::DENOMINATOR/internal_mcd) +
                     (FRACT_A::DENOMINATOR/internal_mcd) * FRACT_B::NUMERATOR,
                     FRACT_A::DENOMINATOR/internal_mcd*FRACT_B::DENOMINATOR>
                    VALUE;
};

// 
// E
// 
template<unsigned short NUM_TERMS>
struct E {
    // e = S(1/n!) = 1/0! + 1/1! + 1/2! + ...
    typedef FRACTION<1, FACTORIAL<NUM_TERMS>::VALUE> TERM;
    typedef E<NUM_TERMS-1> LAST_E;
    typedef typename FRACTION_SUM<TERM, typename LAST_E::VALUE>::VALUE VALUE;
};
template<>
struct E<0> {
    typedef FRACTION<1, 1> VALUE;
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
        cout << "SWITCH: default ";
    }
};
template<>
class SWITCH< 1 > {
public:
    static inline void EXEC() {
        cout << "SWITCH: 1 ";
    }
};
template<>
class SWITCH< 2 > {
public:
    static inline void EXEC() {
        cout << "SWITCH: 2 ";
    }
};

void run_meta()
{
    // 
    // FACTORIAL
    std::cout << "\n[FACTORIAL]" << std::endl;
    std::cout << "FACTORIAL<5>::VALUE: " << FACTORIAL<5>::VALUE << '\n';
    // Because calculations are done at compile-time, they can be used for things such as array sizes.
    char arr0[FACTORIAL<6>::VALUE] = {0};
    std::cout << "arr0 lenght: " << sizeof(arr0)/sizeof(arr0[0]) << '\n';

    std::cout << "FACTORIAL_SAFE<10>::VALUE: " << FACTORIAL_SAFE<10>::VALUE << '\n';
    // this line gives compile error since no implementation for ENSURE_POSITIVE<M, false>
    //std::cout << FACTORIAL_SAFE<-2>::VALUE << '\n';

    std::cout << "factorial(5): " << factorial(5) << '\n';
    std::cout << "factorial_safe(5): " << factorial_safe(5) << '\n';
    // this line gives exception at compile time (TODO: need to verify!!)
    //std::cout << "factorial_safe(-5): " << factorial_safe(-5) << '\n';

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
    // FRACTION
    std::cout << "\n[FRACTION]" << std::endl;
    std::cout << "FRACTION<1, 2>: " << FRACTION<1, 2>::NUMERATOR << "/"
                                    << FRACTION<1, 2>::DENOMINATOR << '\n';
    std::cout << "FRACTION<3, 6>: " << FRACTION<3, 6>::NUMERATOR << "/"
                                    << FRACTION<3, 6>::DENOMINATOR << '\n';
    typedef FRACTION<36, 54> frac_36_54;
    std::cout << "FRACTION<36, 54>: " << frac_36_54::NUMERATOR << "/"
                                      << frac_36_54::DENOMINATOR << '\n';
    typedef FRACTION<54, 36> frac_54_36;
    std::cout << "FRACTION<54, 36>: " << frac_54_36::NUMERATOR << "/"
                                      << frac_54_36::DENOMINATOR << '\n';

    // 
    // FRACTION_SUM
    std::cout << "\n[FRACTION_SUM]" << std::endl;
    typedef FRACTION_SUM<FRACTION<1, 2>, FRACTION<2, 3>>::VALUE frac_sum_1;
    std::cout << "1/2 + 2/3: " << frac_sum_1::NUMERATOR << "/"
                               << frac_sum_1::DENOMINATOR << '\n';
    typedef FRACTION_SUM<FRACTION<2, 6>, FRACTION<4, 9>>::VALUE frac_sum_2;
    std::cout << "2/6 + 4/9: " << frac_sum_2::NUMERATOR << "/"
                               << frac_sum_2::DENOMINATOR << '\n';
    typedef FRACTION_SUM<FRACTION<-1, 2>, FRACTION<4, 6>>::VALUE frac_sum_3;
    std::cout << "-1/2 + 4/6: " << frac_sum_3::NUMERATOR << "/"
                                << frac_sum_3::DENOMINATOR << '\n';

    // 
    // E
    std::cout << "\n[E]" << std::endl;
    using e_0  = E<0>::VALUE;
    using e_1  = E<1>::VALUE;
    using e_2  = E<2>::VALUE;
    using e_3  = E<3>::VALUE;
    using e_4  = E<4>::VALUE;
    using e_5  = E<5>::VALUE;
    using e_10 = E<10>::VALUE;
    std::cout << "E<0>: " << (double)e_0::NUMERATOR/e_0::DENOMINATOR << ", "
                          << e_0::NUMERATOR << '/' << e_0::DENOMINATOR << '\n';
    std::cout << "E<1>: " << (double)e_1::NUMERATOR/e_1::DENOMINATOR << ", "
                          << e_1::NUMERATOR << '/' << e_1::DENOMINATOR << '\n';
    std::cout << "E<2>: " << (double)e_2::NUMERATOR/e_2::DENOMINATOR << ", "
                          << e_2::NUMERATOR << '/' << e_2::DENOMINATOR << '\n';
    std::cout << "E<3>: " << (double)e_3::NUMERATOR/e_3::DENOMINATOR << ", "
                          << e_3::NUMERATOR << '/' << e_3::DENOMINATOR << '\n';
    std::cout << "E<4>: " << (double)e_4::NUMERATOR/e_4::DENOMINATOR << ", "
                          << e_4::NUMERATOR << '/' << e_4::DENOMINATOR << '\n';
    std::cout << "E<5>: " << (double)e_5::NUMERATOR/e_5::DENOMINATOR << ", "
                          << e_5::NUMERATOR << '/' << e_5::DENOMINATOR << '\n';
    std::cout << "E<10>: " << (double)e_10::NUMERATOR/e_10::DENOMINATOR << ", "
                           << e_10::NUMERATOR << '/' << e_10::DENOMINATOR << '\n';


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

