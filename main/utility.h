#ifndef UTILITY_H
#define UTILITY_H

#include "header.h"
#include <fcntl.h>

#define ARRAY_SIZE(A) (sizeof(A) / sizeof(A[0]))

template <class T>
void print_array(const T* arr, int len, const T* ignore = nullptr)
{
    int i = 0;
    std::cout << "[";
    while (i++ < len) {
        auto& v = *(arr + i - 1);
        if (ignore && *ignore == v)
            continue;

        std::cout << v;
        if (i != len) {
            std::cout << ",";
        }
    }
    std::cout << "]";
}
template <typename T, std::enable_if_t<std::is_array_v<T>>* = nullptr>
void print_array(const T& arr)
{
    int len = sizeof(arr) / sizeof(arr[0]);
    int i   = 0;
    std::cout << "[";
    while (i++ < len) {
        auto& v = *(arr + i - 1);
        std::cout << v;
        if (i != len) {
            std::cout << ",";
        }
    }
    std::cout << "]";
}
template <class T>
void print_array(const std::vector<T>& arr, const T& ignore = T{})
{
    std::cout << "[";
    for (size_t i = 0; i < arr.size(); ++i) {
        if (ignore == arr[i])
            continue;

        std::cout << arr[i];
        if (i != arr.size() - 1) {
            std::cout << ",";
        }
    }
    std::cout << "]";
}

std::vector<std::string> split(char delimiter, std::string_view str)
{
    size_t                   last = 0;
    size_t                   next = 0;
    std::vector<std::string> tokens;
    while ((next = str.find(delimiter, last)) != std::string::npos) {
        tokens.emplace_back(str.substr(last, next - last));
        last = next + 1;
    }
    tokens.emplace_back(str.substr(last));
    return tokens;
}

// swap values without introducing temp variable
template <class T>
void swap_plus(T& a, T& b)
{
    a = a + b;
    b = a - b;
    a = a - b;
}

struct shm_utils {
    // dev/shm by default
    static std::filesystem::path shm_dir() { return "/dev/shm"; }

    static int shm_open(const std::string& name, int oflag, mode_t mode)
    {
        auto path = shm_dir().native() + "/" + name;
        return ::open(path.c_str(), oflag, mode);
    }
};

#endif
