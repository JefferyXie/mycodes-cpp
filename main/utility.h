#ifndef UTILITY_H
#define UTILITY_H

#include "header.h"

template<class T> string ToString(T v)
{
    ostringstream ss;
    ss << v;
    return ss.str();
}

// swap values without introducing temp variable
template<class T> void swap_plus(T& a, T& b)
{
    a = a + b;
    b = a - b;
    a = a - b;
}

// sorting algorithm, http://blog.csdn.net/semo_1006/article/details/4706890
// method 1: insertion sort
template<class T> void sort_inserttion(const vector<T>& src, vector<T>& out)
{

}

// method 2: merge sort
// http://blog.chinaunix.net/uid-24774106-id-2240452.html
template<class T> void sort_merge(const vector<T>& src, vector<T>& out)
{

}

// method 3: quick sort
template<class T> void sort_quick(const vector<T>& src, vector<T>& out)
{

}

// major container's time complexity -
// http://en.wikipedia.org/wiki/Search_data_structure
template<class T> void quickSort(T s[], int l, int r)
{
    if (l < r)
    {
        int i = l, j = r;
        T x = s[l];
        while (i < j)
        {
            while(i < j && s[j] >= x) // 从右向左找第一个小于x的数
                j--;
            if(i < j)
                s[i++] = s[j];
            while(i < j && s[i] < x) // 从左向右找第一个大于等于x的数
                i++;
            if(i < j)
                s[j--] = s[i];
        }
        s[i] = x;
        quickSort(s, l, i - 1); // 递归调用
        quickSort(s, i + 1, r);
    }
}

// maximum continuous subarray
template<class T> T maxSubArray(const vector<T>& src)
{
    return *src.begin();
}

namespace formatter {

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


template<typename charT, typename traits = std::char_traits<charT> >
class center_helper {
public:
    center_helper(std::basic_string<charT, traits> str, char fill) : str_(str), fill_(fill) {}
    template<typename a, typename b>
    friend std::basic_ostream<a, b>& operator<<(std::basic_ostream<a, b>& s, const center_helper<a, b>& c);
private:
    std::basic_string<charT, traits> str_;
    char fill_;
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

template <typename T, typename std::enable_if_t<!std::is_arithmetic<T>::value>* = nullptr>
center_helper<std::string::value_type>
centered(T v, char fill = ' ') {
    // keep ONE buffer around
    char buf[] = " ";
    std::string str(buf);
    str += v;
    str += buf;
    return center_helper<std::string::value_type,
                         std::string::traits_type>
                        (str, fill);
}
template<typename T, typename std::enable_if_t<std::is_arithmetic<T>::value>* = nullptr>
center_helper<std::string::value_type, std::string::traits_type>
centered(T v, char fill = ' ') {
    auto str = std::to_string(v);
    return centered(str, fill);
}

template<typename charT, typename traits>
std::basic_ostream<charT, traits>&
operator<< (std::basic_ostream<charT, traits>& s,
            const center_helper<charT, traits>& c)
{
    char fill_default = s.fill(c.fill_);
    std::streamsize w = s.width();
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

} // end namespace formatter

#define FUNC_BEGIN(name)                        \
    std::cout << std::setw(80)                  \
              << formatter::centered(#name, '>')\
              << std::endl;

#define FUNC_END(name)                          \
    std::cout << std::setw(80)                  \
              << formatter::centered(#name, '<')\
              << std::endl;

#endif
