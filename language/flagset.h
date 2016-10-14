#ifndef FLAGSET_H
#define FLAGSET_H

#include "../main/header.h"

// 
// http://codereview.stackexchange.com/questions/96146/c-flagset-typesafe-usage-of-enumeration-as-bitset-bitmask
//
// Below link includes multiple solutions to support enum bitwise operations by overriding operators
// http://stackoverflow.com/questions/1448396/how-to-use-enums-as-flags-in-c

/**
 * FlagSet implements a _bitset usable with `enum` and `enum class`.
 *
 * It provides a typesafe interface for manipulating the _bitset. This helps
 * to avoid mistake as the various operator and function will refuse a
 * parameter that doesn't match the expected enum type.
 *
 * A flagset supports one user-defined enumeration. The number of flags
 * (i.e. the member of the user enumeration) is not limited, as the underlying
 * _bitset (std::_bitset) can have an arbitrary large size.
 * 
 * REQUIREMENTS:
 *      * This code source required C++14 to compile.
 *      * The user enumeration shall not explicitely set any value.
 *      * The last enumeration member shall be: "LAST_VALUE"
 *
 */
template<typename T>
struct FlagSet
{
    FlagSet() = default;

    // &(T)
    FlagSet operator&(const T& val)
    {
        FlagSet ret(*this);
        ret &= val;

        assert(ret._bitset.count() <= 1);
        return ret;
    }

    // &=(T)
    FlagSet& operator&=(const T& val)
    {
        bool tmp = _bitset[static_cast<utype>(val)];
        _bitset.reset();
        _bitset[static_cast<utype>(val)] = tmp;
        return *this;
    }

    // &(FlagSet)
    FlagSet operator&(const FlagSet& val)
    {
        FlagSet ret(*this);
        ret._bitset &= val._bitset;

        return ret;
    }

    // &=(FlagSet)
    FlagSet& operator&=(const FlagSet& o)
    {
        _bitset &= o._bitset;
        return *this;
    }

    // |(T)
    FlagSet operator|(const T& val)
    {
        FlagSet ret(*this);
        ret |= val;

        assert(ret._bitset.count() >= 1);
        return ret;
    }

    // |=(T)
    FlagSet& operator|=(const T& val)
    {
        set_true(val);
        return *this;
    }

    // |(FlagSet)
    FlagSet operator|(const FlagSet& val)
    {
        FlagSet ret(*this);
        ret._bitset |= val._bitset;

        return ret;
    }

    // |=(FlagSet)
    FlagSet& operator|=(const FlagSet& o)
    {
        _bitset |= o._bitset;
        return *this;
    }

    // ~
    FlagSet operator~()
    {
        FlagSet cp(*this);
        cp._bitset.flip();

        return cp;
    }

    // bool()
    explicit operator bool() const
    {
        return _bitset.any();
    }

    // ==(FlagSet)
    bool operator==(const FlagSet& o) const
    {
        return _bitset == o._bitset;
    }

    bool operator[](const T& val)
    {
        return _bitset[static_cast<utype>(val)];
    }

    std::size_t size() const
    {
        return _bitset.size();
    }

    std::size_t count() const
    {
        return _bitset.count();
    }

    FlagSet& set()
    {
        _bitset.set();
        return *this;
    }

    FlagSet& reset()
    {
        _bitset.reset();
        return *this;
    }

    FlagSet& flip()
    {
        _bitset.flip();
        return *this;
    }

    FlagSet& set(const T& val, bool value = true)
    {
        _bitset.set(static_cast<utype>(val), value);
        return *this;
    }

    FlagSet &reset(const T& val)
    {
        _bitset.reset(static_cast<utype>(val));
        return *this;
    }

    FlagSet& flip(const T& val)
    {
        _bitset.flip(static_cast<utype>(val));
        return *this;
    }

    /**
     * Overload for std::ostream
     */
    friend std::ostream& operator<<(std::ostream& stream, const FlagSet& me)
    {
        return stream << me._bitset;
    }

    private:
    using utype = typename std::underlying_type<T>::type;
    std::bitset<static_cast<utype>(T::LAST_VALUE)> _bitset;
    // using below line doesn't need the LAST_VALUE declaration but may bring compile error
    //std::bitset<std::numeric_limits<utype>::max()> _bitset;

    void set_true(const T& val)
    {
        _bitset[static_cast<utype>(val)] = 1;
    }
};

/**
 * Provide a free operator allowing to combine two enumeration
 * member into a FlagSet.
 */
template<typename T>
typename std::enable_if<std::is_enum<T>::value, FlagSet<T>>::type
operator|(const T& lhs, const T& rhs)
{
    FlagSet<T> bs;
    bs |= lhs;
    bs |= rhs;

    return bs;
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

/**
 * Some random enum to use in tests.
 */ 
enum class Options : uint64_t
{
    FULLSCREEN,
    INVERT_MOUSE,
    BLA,
    RED_BACKGROUND,
    RED_FOREGROUND,
    LAST_VALUE
};


void test_AND()
{
    FlagSet<Options> red(Options::RED_FOREGROUND | Options::RED_BACKGROUND);

    auto ret = red & Options::RED_BACKGROUND;
    assert(ret);
    assert(ret.count() == 1);

    ret = red & Options::RED_FOREGROUND;
    assert(ret);
    assert(ret.count() == 1);

    ret = red & (Options::RED_FOREGROUND | Options::RED_BACKGROUND);
    assert(ret);
    assert(ret.count() == 2);

    ret = ~red & Options::RED_BACKGROUND;
    assert((bool)ret == false);
    assert(ret.count() == 0);
}

void test_OR()
{
    FlagSet<Options> red;
    red |= Options::RED_FOREGROUND | Options::RED_BACKGROUND;
    assert(red.count() == 2);

    FlagSet<Options> opt;
    opt |= (Options::FULLSCREEN | Options::BLA);

    // FULLSCREEN and BLA match, so this evaluates to true.
    assert(opt & (Options::FULLSCREEN | Options::BLA | Options::RED_FOREGROUND | Options::RED_BACKGROUND));

    // Ensure that a group of flag is set
    FlagSet<Options> expected;
    expected |= (Options::FULLSCREEN);
    assert((opt & expected) == expected);

    assert((bool)(opt & (Options::RED_FOREGROUND | Options::RED_BACKGROUND)) == false);
    assert((bool)(opt & red) == false);

    assert(!(bool)(opt & Options::INVERT_MOUSE));
    opt |= ~red;
    assert((bool)(opt & Options::INVERT_MOUSE));
}

void test_set_reset()
{
    FlagSet<Options> opt;

    assert(opt.count() == 0);
    opt.set();
    assert(opt.count() == opt.size() && opt.size() == 5);
    opt.reset();
    assert(opt.count() == 0);

    opt.set(Options::BLA);
    assert(opt.count() == 1 && opt[Options::BLA]);
    opt.set(Options::BLA, false);
    assert(opt.count() == 0);  
}

void test_type_safety()
{
    // The following will not compile.
    FlagSet<Options> bs;

    // bs & 42;
    // bs &= 42;
    // bs |= 42;
    // bs | 42;
}

#endif

