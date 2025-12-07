#pragma once

#include "../core/my_formatter.h"
#include "../core/utility.h"

#ifndef _VA_LIST
typedef __builtin_va_list va_list;
#define _VA_LIST
#endif

#define va_start(ap, param) __builtin_va_start(ap, param)
#define va_end(ap) __builtin_va_end(ap)
#define va_arg(ap, type) __builtin_va_arg(ap, type)

// Set the error string when used in conjunction with maybe<T>
class err final
{
    char buf_[BUFSIZ];

    template <typename T>
    friend class maybe;

public:
    err() { *buf_ = '\0'; }

    [[gnu::format(printf, 2, 3)]] err(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        ::vsnprintf(buf_, BUFSIZ, fmt, args);
        va_end(args);
    }

    [[gnu::format(printf, 4, 5)]] err(int e, const char* err_fn, const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        const auto n = ::vsnprintf(buf_, BUFSIZ, fmt, args);
        va_end(args);
        if (n < BUFSIZ) {
            ::snprintf(
                buf_ + n, std::make_unsigned<decltype(BUFSIZ)>::type(BUFSIZ - n), ": %s errno %d (%s)", err_fn, e,
                ::strerror(e));
        }
    }
};

// Enforce error checking of return values
template <typename T>
class maybe final
{
    maybe()                        = delete;
    maybe(const maybe&)            = delete;
    maybe& operator=(const maybe&) = delete;

    using val_t = typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type;

    union {
        val_t       val_;
        std::string err_;
    };
    bool         success_ = false;
    mutable bool safe_    = false;    // true if no error, or has checked for error.

    void abort_if_unsafe() const
    {
        const auto safe = safe_;
        safe_           = true;
        if (!safe) {
            printf("\tnot safe:%s\n", err_.c_str());
            // exit(1);
        }
    }

    T& get()
    {
        abort_if_unsafe();
        return *reinterpret_cast<T*>(&val_);
    }

    const T& get() const
    {
        abort_if_unsafe();
        return *reinterpret_cast<const T*>(&val_);
    }

public:
    // Deliberately allow implicit construction of 'maybe' from various sources
    maybe(const T& val) : success_{true}, safe_{true} { new (&val_) T{val}; }

    maybe(T&& val) : success_{true}, safe_{true} { new (&val_) T{std::move(val)}; }

    maybe(err&& e) { new (&err_) std::string{e.buf_}; }

    maybe(maybe&& rhs) : success_{rhs.success_}, safe_{rhs.safe_}
    {
        if (success_)
            new (&val_) T{std::move(*reinterpret_cast<T*>(&rhs.val_))};
        else
            new (&err_) std::string{std::move(rhs.err_)};
        rhs.safe_    = true;
        rhs.success_ = false;
        new (&rhs.err_) std::string{};
    }

    ~maybe() noexcept(false)
    {
        abort_if_unsafe();
        using std::string;
        if (success_)
            reinterpret_cast<T*>(&val_)->~T();
        else
            err_.~string();
    }

    bool ok() const
    {
        safe_ = true;
        return success_;
    }

    // Only valid if ok() return false.
    const char* errstr() const { return err_.c_str(); }

    // WARNING: Calling the accessor methods below if an error is flagged will
    // result in undefined behaviour
    operator T() { return get(); }
    operator T() const { return get(); }

    T&       operator*() { return get(); }
    const T& operator*() const { return get(); }

    T*       operator->() { return &get(); }
    const T* operator->() const { return &get(); }
};

maybe<bool> try_maybe_bool(int flag)
{
    if (flag == 0)
        return err{"try_maybe_bool: %d", flag};
    if (flag < 0)
        return false;
    return true;
}
maybe<double> try_maybe_double(int flag)
{
    if (flag < 0)
        return err{"try_maybe_double: %d", flag};
    return 100.0f;
}
maybe<std::string> try_maybe_string(int flag)
{
    if (flag < 0)
        return err{"try_maybe_string: %d", flag};
    return std::string("awesome!");
}

void run_maybe()
{
    FUNC_BEGIN(run_maybe);

    std::cout << std::boolalpha;

    SECTION_BEGIN(try_maybe_bool);
    if (auto a = try_maybe_bool(1)) {
        std::cout << "try_maybe_bool(1): " << a << std::endl;
    }
    if (auto a = try_maybe_bool(0)) {
        std::cout << "try_maybe_bool(0): " << a << std::endl;
    }
    if (auto a = try_maybe_bool(-10)) {
        std::cout << "try_maybe_bool(-10): " << a << std::endl;
    }

    SECTION_BEGIN(try_maybe_double);
    if (auto a = try_maybe_double(1)) {
        std::cout << "try_maybe_double(1): " << a << std::endl;
    }
    if (auto a = try_maybe_double(0)) {
        std::cout << "try_maybe_doulbe(0): " << a << std::endl;
    }
    if (auto a = try_maybe_double(-10)) {
        std::cout << "try_maybe_double(-10): " << a << std::endl;
    }

    SECTION_BEGIN(try_maybe_string);
    auto a = try_maybe_string(1);
    if (a.ok()) {
        std::cout << "try_maybe_string(1): " << *a << std::endl;
    }
    auto b = try_maybe_string(0);
    if (b.ok()) {
        std::cout << "try_maybe_string(0): " << *b << std::endl;
    }
    auto c = try_maybe_string(-10);
    if (c.ok()) {
        std::cout << "try_maybe_string(-10): " << (std::string)c << std::endl;
    }
}

