#pragma once

#include "../main/header.h"
#include <cxxabi.h>
#include <execinfo.h>
#include <typeindex>

class class_util
{
public:
    template <typename T>
    static std::string demangle(const T& obj, bool fullNamespace = false)
    {
        return demangleTypeInfo(typeid(obj), fullNamespace);
    }

    template <typename T>
    static std::string demangleType(bool fullNamespace = false)
    {
        return demangleTypeInfo(typeid(T), fullNamespace);
    }

    static std::string demangleTypeInfo(const std::type_info& ti, bool fullNamespace = false)
    {
        using key_t = std::pair<std::type_index, bool>;

        thread_local std::map<key_t, std::string> demangle_cache;
        auto&                                     res = demangle_cache[{std::type_index(ti), fullNamespace}];
        if (res.empty()) {
            res = demangleName(ti.name(), fullNamespace);
        }
        return res;
    }

    static std::string demangleName(const char* name, bool fullNamespace = false)
    {
        int         status    = 0;
        char*       demangled = abi::__cxa_demangle(name, 0, 0, &status);
        std::string ret;
        if (demangled) {
            ret = demangled;
            free(demangled);
        } else {
            ret = name;
        }

        // Remove namespace separators if required. Things become tricky for template.
        if (!fullNamespace) {
            size_t lastColonPos = std::string::npos;
            size_t level        = 0ul;
            for (size_t i = 0; i < ret.size(); ++i) {
                if (ret[i] == '<') {
                    ++level;
                } else if (ret[i] == '>') {
                    --level;
                } else if (ret[i] == ':' && level == 0ul) {
                    lastColonPos = i;
                }
            }

            if (lastColonPos != std::string::npos) {
                ret = ret.substr(lastColonPos + 1);
            }
        }
        return ret;
    }

    static std::string fullBacktrace()
    {
        std::ostringstream oss;
        oss << std::endl;

        void*  arr[100];
        size_t size    = ::backtrace(arr, sizeof(arr) / sizeof(arr[0]));
        char** strings = ::backtrace_symbols(arr, size);

        // Start at 1 so we skip printBacktrace() itself
        for (size_t i = 1; i < size; ++i) {
            const auto demangled = demangleStack(strings[i]);
            oss << " " << (demangled.length() ? demangled : strings[i]) << std::endl;

            // Like gdb, stop stack trace until main to avoid confusion
            if (std::string(strings[i]).find("(main+") != std::string::npos) {
                break;
            }
        }
        free(strings);
        return oss.str();
    }

    template <typename T, typename S>
    static const T* cachedDynamicCast(const S* src)
    {
        // This is hacky but is a significant optimization.
        using cache_t = std::map<const std::type_info*, std::tuple<bool, std::ptrdiff_t>>;

        thread_local cache_t cache;

        const std::type_info* srcTypeIdx = &typeid(*src);
        auto [iter, inserted]            = cache.try_emplace(srcTypeIdx);
        auto& [success, offset]          = iter->second;
        if (inserted) {
            if (auto res = dynamic_cast<const T*>(src); res) {
                success = true;
                offset  = reinterpret_cast<const char*>(res) - reinterpret_cast<const char*>(src);
                return res;
            }
        } else if (success) {
            return reinterpret_cast<const T*>(reinterpret_cast<const char*>(src) + offset);
        }
        return nullptr;
    }

    // Non-const version is to ensure we only have one local cache map
    template <typename T, typename S>
    static T* cachedDynamicCast(S* src)
    {
        return const_cast<T*>(cachedDynamicCast<T>(const_cast<const S*>(src)));
    }

private:
    // Take input of the form 'debug_dir/text_exe(_ZN7StatArb14printBacktraceEv+0x22) [0x811ee5d]' returneed by
    // backtrace_symbols, and stips out the mangleed function name
    static std::string demangleStack(const std::string& input)
    {
        std::string  result;
        const size_t firstParent = input.find_first_of('(');
        const size_t firstPlus   = input.find_first_of('+', firstParent);

        if (firstParent != std::string::npos && firstPlus != std::string::npos) {
            const auto functionName = input.substr(firstParent + 1, firstPlus - firstParent - 1);
            int        status       = -1;
            char*      demangled    = abi::__cxa_demangle(functionName.c_str(), 0, 0, &status);
            if (status == 0) {
                result = std::string(demangled);
                free(demangled);
            }
        }
        return result;
    }
};

