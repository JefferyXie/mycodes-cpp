#ifndef UTILITY_H
#define UTILITY_H

#include "header.h"
#include "node.h"
#include <fcntl.h>

#define ARRAY_SIZE(A) (sizeof(A) / sizeof(A[0]))

template <class T>
std::string dump_array(const T* arr, int len, const T* ignore = nullptr)
{
    std::ostringstream oss;
    oss << "[";
    int i = 0;
    while (i++ < len) {
        auto& v = *(arr + i - 1);
        if (ignore && *ignore == v)
            continue;

        oss << v;
        if (i != len) {
            oss << ",";
        }
    }
    oss << "]";
    return oss.str();
}
template <typename T, std::enable_if_t<std::is_array_v<T>>* = nullptr>
std::string dump_array(const T& arr)
{
    std::ostringstream oss;
    oss << "[";

    int len = sizeof(arr) / sizeof(arr[0]);
    int i   = 0;
    while (i++ < len) {
        auto& v = *(arr + i - 1);
        oss << v;
        if (i != len) {
            oss << ",";
        }
    }
    oss << "]";
    return oss.str();
}
template <class T>
std::string dump_array(const std::vector<T>& arr, const T& ignore = T{})
{
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < arr.size(); ++i) {
        if (ignore == arr[i])
            continue;

        oss << arr[i];
        if (i != arr.size() - 1) {
            oss << ",";
        }
    }
    oss << "]";
    return oss.str();
}

template <class T>
std::string dump_matrix(const std::vector<std::vector<T>>& vs)
{
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < vs.size(); ++i) {
        oss << "[";
        for (size_t j = 0; j < vs[i].size(); ++j) {
            oss << vs[i][j];
            if (j != vs[i].size() - 1) {
                oss << ",";
            }
        }
        oss << "]";
        if (i != vs.size() - 1) {
            oss << ",";
        }
    }
    oss << "]";
    return oss.str();
}

template <typename T>
std::string dump_list(list_node_t<T>* node)
{
    std::ostringstream oss;
    oss << "[";
    while (node) {
        oss << node->data;
        node = node->next;
        if (node) {
            oss << "->";
        }
    }
    oss << "]";
    return oss.str();
};

// TODO: define some concept for container to satisfy xxx.size() and std::find_if
bool equal_container_unordered(auto& a, auto& b)
{
    if (a.size() != b.size()) {
        return false;
    }
    for (auto& v : a) {
        if (auto iter = std::find_if(
                b.begin(), b.end(),
                [&v](auto& u) {
                    return u == v;
                });
            iter == b.end()) {
            return false;
        }
    }
    return true;
};

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
