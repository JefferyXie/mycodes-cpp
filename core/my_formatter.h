#pragma once

#include "../main/header.h"

namespace my_formatter {

//
// center alignment the text
// https://stackoverflow.com/a/14861289/7169399
//
/* usage:
    std::cout << '\n'
              << "|" << std::setw(15) << centered(1.12)
              << "|" << std::setw(10) << centered("Column")
              << "|" << std::setw(20) << centered(std::string("Column good"))
              << "|" << std::setw(9)  << centered("Header") << "|"
              << std::endl;
*/

template <typename charT, typename traits = std::char_traits<charT>>
class center_helper
{
public:
    center_helper(std::basic_string<charT, traits> str, char fill) : str_(str), fill_(fill) {}
    template <typename a, typename b>
    friend std::basic_ostream<a, b>& operator<<(std::basic_ostream<a, b>& s, const center_helper<a, b>& c);

private:
    std::basic_string<charT, traits> str_;
    char                             fill_;
};

/*
template<typename charT, typename traits = std::char_traits<charT> >
center_helper<charT, traits>
centered(std::basic_string<charT, traits> str) {
    return center_helper<charT, traits>(str);
}
// redeclare for std::string directly so we can support anything that implicitly converts to std::string
center_helper<std::string::value_type, std::string::traits_type>
centered(const std::string& str) {
    return center_helper<std::string::value_type, std::string::traits_type>(str);
}

*/

template <typename T, typename std::enable_if_t<!std::is_arithmetic_v<T>>* = nullptr>
center_helper<std::string::value_type> centered(T v, char fill = ' ')
{
    // keep ONE buffer around
    char        buf[] = " ";
    std::string str(buf);
    str += v;
    str += buf;
    return center_helper<std::string::value_type, std::string::traits_type>(str, fill);
}
template <typename T, typename std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
center_helper<std::string::value_type, std::string::traits_type> centered(T v, char fill = ' ')
{
    auto str = std::to_string(v);
    return centered(str, fill);
}

template <typename charT, typename traits>
std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>& s, const center_helper<charT, traits>& c)
{
    // make sure the default adjustfield is valid
    std::cout << std::right;
    char            fill_default = s.fill(c.fill_);
    std::streamsize w            = s.width();
    if (w > c.str_.length()) {
        std::streamsize left = (w + c.str_.length()) / 2;
        s.width(left);
        s << c.str_;
        s.width(w - left);
        s << "";
    } else {
        s << c.str_;
    }
    s.fill(fill_default);
    return s;
}

}    // end namespace my_formatter

// clang-format off
#define FUNC_BEGIN(name)                            \
    std::cout << "\n"                               \
              << std::setw(80)                      \
              << my_formatter::centered(#name, '>') \
              << std::endl;

#define FUNC_END(name)                              \
    std::cout << std::setw(80)                      \
              << my_formatter::centered(#name, '<') \
              << "\n"                               \
              << std::endl;

#define SECTION_BEGIN(name)                         \
    std::cout << std::setw(80)                      \
              << my_formatter::centered(#name, '-') \
              << std::endl;
// clang-format on

