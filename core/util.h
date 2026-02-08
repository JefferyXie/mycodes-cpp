#pragma once

#include "node.h"
#include <fcntl.h>

namespace util {

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

template <typename ContainerT>
std::string dump_matrix(const ContainerT& vs, bool one_row_per_line = false)
{
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < vs.size(); ++i) {
        if (one_row_per_line) {
            oss << "\n ";
        }
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
    if (one_row_per_line) {
        oss << "\n";
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

template <typename T>
std::string dump_graph(graph_node_t<T>* node)
{
    std::unordered_set<graph_node_t<T>*> visited;
    std::ostringstream                   oss;
    oss << "[";
    while (node) {
        visited.emplace(node);

        auto tmp = node;
        node     = nullptr;

        oss << "[";
        for (auto neighbor : tmp->neighbors) {
            oss << neighbor->data;
            if (neighbor != tmp->neighbors.back()) {
                oss << ",";
            }

            if (!node && !visited.contains(neighbor)) {
                node = neighbor;
            }
        }
        oss << "]";
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

auto trim(std::string_view sv)
{
    std::string s(sv);
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](auto v) {
                return !std::isspace(v);
            }));
    s.erase(
        std::find_if(
            s.rbegin(), s.rend(),
            [](auto v) {
                return !std::isspace(v);
            })
            .base(),
        s.end());
    return s;
}

std::vector<std::string> split(char delimiter, std::string_view str)
{
    size_t                   last = 0;
    size_t                   next = 0;
    std::vector<std::string> tokens;
    while ((next = str.find(delimiter, last)) != std::string::npos) {
        // TODO: trim and ignore empty??
        tokens.emplace_back(str.substr(last, next - last));
        last = next + 1;
    }
    tokens.emplace_back(str.substr(last));
    return tokens;
}

// TODO: split spec by any delimiter from delimiters, need test and improvment...
std::vector<std::string> split_any(std::string_view delimiters, std::string_view spec)
{
    std::vector<std::string> tokens;

    auto addToken = [&tokens](std::string token) {
        // Trim leading whitespace
        if (!token.empty()) {
            size_t start = token.find_first_not_of(" ");
            token        = start != std::string::npos ? token.substr(start) : "";
        }

        // Do not add empty tokens
        if (token.empty()) {
            return;
        }
        tokens.push_back(token);
    };

    size_t start = 0;
    for (size_t end = 0; end < spec.size(); ++end) {
        // If current char is a separator, add the current token AND separator to the tokens list
        if (delimiters.find(spec[end]) != std::string::npos) {
            addToken(std::string{spec.substr(start, end - start)});
            addToken(std::string{spec.substr(end, 1)});
            start = end + 1;
        }
    }
    // Add the last token to tokens list
    addToken(std::string{spec.substr(start)});
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

}    // namespace util

