#pragma once

#include "../core/header.h"

/**
 * Some random enum to use in tests.
 */
enum class Options : uint64_t { FULLSCREEN, INVERT_MOUSE, BLA, RED_BACKGROUND, RED_FOREGROUND, LAST_VALUE };

//
// http://codereview.stackexchange.com/questions/96146/c-flag_set_t-typesafe-usage-of-enumeration-as-bitset-bitmask
//
// Below link includes multiple solutions to support enum bitwise operations by overriding operators
// http://stackoverflow.com/questions/1448396/how-to-use-enums-as-flags-in-c

/**
 * flag_set_t implements a _bitset usable with `enum` and `enum class`.
 *
 * It provides a typesafe interface for manipulating the _bitset. This helps
 * to avoid mistake as the various operator and function will refuse a
 * parameter that doesn't match the expected enum type.
 *
 * A flag_set_t supports one user-defined enumeration. The number of flags
 * (i.e. the member of the user enumeration) is not limited, as the underlying
 * _bitset (std::_bitset) can have an arbitrary large size.
 *
 * REQUIREMENTS:
 *      * This code source required C++14 to compile.
 *      * The user enumeration shall not explicitely set any value.
 *      * The last enumeration member shall be: "LAST_VALUE"
 *
 */
template <typename T>
struct flag_set_t {
    flag_set_t() = default;

    // &(T)
    flag_set_t operator&(const T& val)
    {
        flag_set_t ret(*this);
        ret &= val;

        assert(ret._bitset.count() <= 1);
        return ret;
    }

    // &=(T)
    flag_set_t& operator&=(const T& val)
    {
        bool tmp = _bitset[static_cast<utype>(val)];
        _bitset.reset();
        _bitset[static_cast<utype>(val)] = tmp;
        return *this;
    }

    // &(flag_set_t)
    flag_set_t operator&(const flag_set_t& val)
    {
        flag_set_t ret(*this);
        ret._bitset &= val._bitset;

        return ret;
    }

    // &=(flag_set_t)
    flag_set_t& operator&=(const flag_set_t& o)
    {
        _bitset &= o._bitset;
        return *this;
    }

    // |(T)
    flag_set_t operator|(const T& val)
    {
        flag_set_t ret(*this);
        ret |= val;

        assert(ret._bitset.count() >= 1);
        return ret;
    }

    // |=(T)
    flag_set_t& operator|=(const T& val)
    {
        set_true(val);
        return *this;
    }

    // |(flag_set_t)
    flag_set_t operator|(const flag_set_t& val)
    {
        flag_set_t ret(*this);
        ret._bitset |= val._bitset;

        return ret;
    }

    // |=(flag_set_t)
    flag_set_t& operator|=(const flag_set_t& o)
    {
        _bitset |= o._bitset;
        return *this;
    }

    // ~
    flag_set_t operator~()
    {
        flag_set_t cp(*this);
        cp._bitset.flip();

        return cp;
    }

    // bool()
    explicit operator bool() const { return _bitset.any(); }

    // ==(flag_set_t)
    bool operator==(const flag_set_t& o) const { return _bitset == o._bitset; }

    bool operator[](const T& val) { return _bitset[static_cast<utype>(val)]; }

    std::size_t size() const { return _bitset.size(); }

    std::size_t count() const { return _bitset.count(); }

    flag_set_t& set()
    {
        _bitset.set();
        return *this;
    }

    flag_set_t& reset()
    {
        _bitset.reset();
        return *this;
    }

    flag_set_t& flip()
    {
        _bitset.flip();
        return *this;
    }

    flag_set_t& set(const T& val, bool value = true)
    {
        _bitset.set(static_cast<utype>(val), value);
        return *this;
    }

    flag_set_t& reset(const T& val)
    {
        _bitset.reset(static_cast<utype>(val));
        return *this;
    }

    flag_set_t& flip(const T& val)
    {
        _bitset.flip(static_cast<utype>(val));
        return *this;
    }

    /**
     * Overload for std::ostream
     */
    friend std::ostream& operator<<(std::ostream& stream, const flag_set_t& me) { return stream << me._bitset; }

private:
    using utype = typename std::underlying_type<T>::type;
    std::bitset<static_cast<utype>(T::LAST_VALUE)> _bitset;
    // using below line doesn't need the LAST_VALUE declaration but may bring compile error
    // std::bitset<std::numeric_limits<utype>::max()> _bitset;

    void set_true(const T& val) { _bitset[static_cast<utype>(val)] = 1; }
};

/**
 * Provide a free operator allowing to combine two enumeration
 * member into a flag_set_t.
 */
template <typename T>
typename std::enable_if<std::is_enum<T>::value, flag_set_t<T>>::type operator|(const T& lhs, const T& rhs)
{
    flag_set_t<T> bs;
    bs |= lhs;
    bs |= rhs;

    return bs;
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

void test_AND()
{
    flag_set_t<Options> red(Options::RED_FOREGROUND | Options::RED_BACKGROUND);

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
    flag_set_t<Options> red;
    red |= Options::RED_FOREGROUND | Options::RED_BACKGROUND;
    assert(red.count() == 2);

    flag_set_t<Options> opt;
    opt |= (Options::FULLSCREEN | Options::BLA);

    // FULLSCREEN and BLA match, so this evaluates to true.
    assert(opt & (Options::FULLSCREEN | Options::BLA | Options::RED_FOREGROUND | Options::RED_BACKGROUND));

    // Ensure that a group of flag is set
    flag_set_t<Options> expected;
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
    flag_set_t<Options> opt;

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
    flag_set_t<Options> bs;

    // bs & 42;
    // bs &= 42;
    // bs |= 42;
    // bs | 42;
}

