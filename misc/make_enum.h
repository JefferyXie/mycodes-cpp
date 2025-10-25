#ifndef MAKE_ENUM_H
#define MAKE_ENUM_H

#include "../main/formatter.h"

//
// https://stackoverflow.com/questions/207976/how-to-easily-map-c-enums-to-strings
// https://stackoverflow.com/questions/28828957/enum-to-string-in-modern-c11-c14-c17-and-future-c20
//

#define MAKE_ENUM(name, ...)                                                          \
    enum class name { __VA_ARGS__ };                                                  \
    inline const char* get_enum_name(name value)                                      \
    {                                                                                 \
        static std::unordered_map<int, std::string> s_##name;                         \
        if (s_##name.empty()) {                                                       \
            const std::string& str      = #__VA_ARGS__;                               \
            auto               add_name = [](int e, const std::string& m) {           \
                /* make your way to get name from enum */               \
                s_##name.emplace(e, m);                                 \
            };                                                                        \
            int         e_value = 0;                                                  \
            std::string e_name;                                                       \
            bool        assigned = false;                                             \
            std::string assignee;                                                     \
            for (unsigned int i = 0; i < str.size(); ++i) {                           \
                if (std::isspace(str[i]))                                             \
                    continue;                                                         \
                switch (str[i]) {                                                     \
                case '=':                                                             \
                    e_value  = 0;                                                     \
                    assigned = true;                                                  \
                    break;                                                            \
                case ',':                                                             \
                    if (assigned) {                                                   \
                        char* p = nullptr;                                            \
                        e_value = strtol(assignee.c_str(), &p, 10);                   \
                        /* don't support non-number assignment */                     \
                        if (p == &*assignee.end())                                    \
                            add_name(e_value, e_name);                                \
                    } else {                                                          \
                        add_name(e_value, e_name);                                    \
                    }                                                                 \
                    ++e_value;                                                        \
                    e_name.clear();                                                   \
                    assigned = false;                                                 \
                    assignee.clear();                                                 \
                    break;                                                            \
                default:                                                              \
                    if (assigned)                                                     \
                        assignee += str[i];                                           \
                    else                                                              \
                        e_name += str[i];                                             \
                    break;                                                            \
                }                                                                     \
            }                                                                         \
            add_name(e_value, e_name);                                                \
            std::cout << str << std::endl;                                            \
            for (auto pr : s_##name) {                                                \
                std::cout << "{" << pr.first << "," << pr.second << "}" << std::endl; \
            }                                                                         \
        }                                                                             \
        auto it = s_##name.find((int)value);                                          \
        return it != s_##name.end() ? it->second.c_str() : nullptr;                   \
    }

MAKE_ENUM(Empty);

MAKE_ENUM(Animal, DOG, CAT, HORSE);

MAKE_ENUM(
    STRATEGY_TYPE,
    // Simple Instrument, i.e., SingleLeg Security (Option)
    ST_SIMPLE = 0,
    // Future Spreads: 1 - 63
    ST_F_BF = 1, /* Butterfly */
    ST_F_BS,     // Bundle Spread
    ST_F_C1,     // Crack 1:1
    ST_F_CF,     // Condor
    ST_F_DF,     // Double Butterfly
    ST_F_DI,     // Interest Rate Inter-commodity Spread
    ST_F_EC,     // TAS Calendar Spread
    ST_F_EQ,     // Equity Calendar Spread
    ST_F_FB,     // Bundle
    ST_F_FS,     // Strip
    ST_F_FX,     // FX Calendar Spread
    ST_F_IN,     // Invoice Swap
    ST_F_IS,     // Inter-Commodity Spread
    ST_F_IV,     // Implied Treasury Inter-Commodity Spread
    ST_F_MP,     // Month Pack
    ST_F_MS,     // BMD Future Strip
    ST_F_PB,     // Pack Butterfly
    ST_F_PK,     // Pack
    ST_F_PS,     // Pack Spread
    ST_F_RT,     // Reduce Tick Calendar Spread
    ST_F_SA,     // Energy Strip
    ST_F_SB,     // Balanced Strip
    ST_F_SD,     // Bond Index Spread
    ST_F_SI,     // Commodities Inter-Commodity Spread
    ST_F_SP,     // Calendar Spread
    ST_F_TL,     // Treasury Tail Spread
    ST_F_WS,     // Unbalanced Strip
    ST_F_XS,     // Energy Inter-commodity Strip
    ST_F_MIN = ST_F_BF, ST_F_MAX = ST_F_XS,

    // Option Spreads: 64 - 254
    ST_O_12 = 64,    // Ratio 1X2
    ST_O_13,         // Ratio 1X3
    ST_O_23,         // Ratio 2X3
    ST_O_3C,         // 3 way straddle vs call
    ST_O_3P,         // 3 way straddle vs put
    ST_O_3W,         // 3 way
    ST_O_BO,         // Butterfly
    ST_O_BX,         // Box
    ST_O_CC,         // Conditional Curve
    ST_O_CO,         // Condor
    ST_O_DB,         // Double
    ST_O_DG,         // Calendar Diagonal
    ST_O_FO,         // Covered option outright
    ST_O_GD,         // Option Energy Strip Spread
    ST_O_GN,         // Generic strategy / not-exchange-recognizable-strategy
    ST_O_GT,         // Guts
    ST_O_HO,         // Horizontal
    ST_O_HS,         // Horizontal Straddle
    ST_O_IB,         // Iron Butterfly
    ST_O_IC,         // Iron Condor
    ST_O_JR,         // Jelly Roll
    ST_O_RR,         // Risk Reversal
    ST_O_SA,         // Option Energy Strip
    ST_O_SG,         // Strangle
    ST_O_SR,         // Strip
    ST_O_SS,         // Straddle Strip
    ST_O_ST,         // Straddle
    ST_O_VT,         // Vertical
    ST_O_XT,         // xmas tree
    ST_O_MIN = ST_O_12, ST_O_MAX = ST_O_XT,
    ST_UNKNOWN = 255    // unknown strategy
);

void run_make_enum()
{
    FUNC_BEGIN(run_make_enum);

    auto dog = Animal::DOG;

    std::cout << "to_string(dog):" << get_enum_name(dog) << std::endl;
    //    cout << "to_string(11):" << to_string((Animal)11) << endl;
    //    cout << "to_string(__COUNT):" << to_string(Animal::__COUNT) << endl;

    std::cout << "to_string(ST_F_XS):" << get_enum_name(STRATEGY_TYPE::ST_F_XS) << std::endl;
    std::cout << "to_string(ST_F_MIN):" << get_enum_name(STRATEGY_TYPE::ST_F_MIN) << std::endl;
    std::cout << "to_string(ST_O_12):" << get_enum_name(STRATEGY_TYPE::ST_O_12) << std::endl;
    std::cout << "to_string(ST_O_13):" << get_enum_name(STRATEGY_TYPE::ST_O_13) << std::endl;

    auto nm = get_enum_name((STRATEGY_TYPE)100);
    if (!nm)
        std::cout << "empty" << std::endl;
}

#endif

