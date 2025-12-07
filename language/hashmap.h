#pragma once

#include "../core/header.h"
#include "classtest.h"

// http://marknelson.us/2011/09/03/hash-functions-for-c-unordered-containers/
// http://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key

// method 1), hash function and compare function are in separate class/struct
struct MyKey1 {
    std::string first;
    std::string ToString() const { return "first:" + first; }
};
// hash object is only created once per unordered_map
struct MyKey1_Hash {
    size_t operator()(const MyKey1& key) const { return std::hash<std::string>()(key.first); }
};
// compare object is only created once per unordered_map
struct MyKey1_Compare {
    bool operator()(const MyKey1& k1, const MyKey1& k2) const { return k1.first == k2.first; }
};

// method 2: hash and compare are in separate functions outside of MyKey class
struct MyKey2 {
    std::string first;
    std::string second;
    std::string ToString() const { return "first:" + first + ",second:" + second; }
};
size_t MyKey2_Hash(const MyKey2& k)
{
    return std::hash<std::string>()(k.first) ^ (std::hash<std::string>()(k.second) << 1);
}
bool MyKey2_Compare(const MyKey2& k1, const MyKey2& k2)
{
    return k1.first == k2.first && k1.second == k2.second;
}

// method 3: compare is inside MyKey class, hash is specilized within std::hash
struct MyKey3 {
    std::string first;
    std::string second;
    std::string third;
    std::string ToString() const { return "first:" + first + ",second:" + second + ",third:" + third; }
    bool        operator==(const MyKey3& k) const
    {
        return this->first == k.first && this->second == k.second && this->third == k.third;
    }
};
namespace std {
template <>
struct hash<MyKey3> {
    size_t operator()(const MyKey3& k) const
    {
        return (hash<string>()(k.first) ^ (hash<string>()(k.second) << 1) >> 1) ^ (hash<string>()(k.third) << 1);
    }
};
}    // namespace std

// method 4: hash map is class member, use customize hasher & comparar
struct MyHashMap {
    MyHashMap() = delete;

    struct hasher {
        hasher(int len) : len_(len) {}
        size_t operator()(const std::string& key) const { return std::hash<std::string>()(key.substr(0, len_)); }
        int    len_ = 0;
    };
    struct comparar {
        comparar(int len) : len_(len) {}
        bool operator()(const std::string& k1, const std::string& k2) const
        {
            return k1.substr(0, len_) == k2.substr(0, len_);
        }
        int len_ = 0;
    };

    using hasher_t   = std::function<size_t(const std::string&)>;
    using comparar_t = std::function<bool(const std::string&, const std::string&)>;

    MyHashMap(int len) : um1_(2, hasher{len}, comparar{len})
    {
        um2_ = new std::unordered_map<std::string, int, hasher, comparar>{2, hasher{len + 1}, comparar(len + 1)};

        // highly flexible: hasher & comparar are configurable
        um3_ = new std::unordered_map<std::string, int, hasher_t, comparar_t>{
            2,
            [len_ = len + 2](const std::string& key) {
                return std::hash<std::string>()(key.substr(0, len_));
            },
            [len_ = len + 2](const std::string& k1, const std::string& k2) {
                return k1.substr(0, len_) == k2.substr(0, len_);
            }};
    }

    void add(std::string k, int v)
    {
        um1_.emplace(k, v);
        um2_->emplace(k, v);
        um3_->emplace(k, v);
    }
    void print()
    {
        std::cout << "-----um1_-----" << std::endl;
        for (auto& entry : um1_) {
            std::cout << entry.first << ", " << entry.second << std::endl;
        }
        std::cout << "-----um2_-----" << std::endl;
        for (auto& entry : *um2_) {
            std::cout << entry.first << ", " << entry.second << std::endl;
        }
        std::cout << "-----um3_-----" << std::endl;
        for (auto& entry : *um3_) {
            std::cout << entry.first << ", " << entry.second << std::endl;
        }
    }

    std::unordered_map<std::string, int, hasher, comparar> um1_;

    std::unordered_map<std::string, int, hasher, comparar>* um2_;

    std::unordered_map<std::string, int, hasher_t, comparar_t>* um3_;
};

void run_hashmap()
{
    // MyKey1
    std::cout << "----------MyKey1----------" << std::endl;
    std::unordered_map<MyKey1, int, MyKey1_Hash, MyKey1_Compare> um1 = {
        {{"1st"}, 10}, {{"2nd"}, 20}, {{"3st"}, 30}, {{"5th"}, 50}, {{"6th"}, 60}, {{"7th"}, 70}, {{"4th"}, 40},
    };
    um1.insert({{"8th"}, 80});
    um1.emplace(MyKey1{"7th"}, 70);

    MyKey1 k1 = {"2nd"};
    MyKey1 k2({"10th"});
    auto   it1 = um1.find(k1);
    if (it1 != um1.end()) {
        std::cout << it1->first.ToString() << "|" << it1->second << std::endl;
    }
    auto it2 = um1.find(k2);
    if (it2 != um1.end()) {
        std::cout << it2->first.ToString() << "|" << it2->second << std::endl;
    }

    auto hasher = [](const MyKey1& key) {
        return std::hash<std::string>()(key.first);
    };
    auto equaler = [](const MyKey1& k1, const MyKey1& k2) {
        return k1.first == k2.first;
    };

    std::unordered_map<MyKey1, int, decltype(hasher), decltype(equaler)> um1_1 = {
        {
            {{"1st"}, 10},
            {{"2nd"}, 20},
            {{"3st"}, 30},
            {{"5th"}, 50},
            {{"6th"}, 60},
            {{"7th"}, 70},
            {{"4th"}, 40},

            {{"6th1"}, 160},
            {{"7th1"}, 170},

            {{"6th2"}, 260},
            {{"7th2"}, 270},

            {{"6th3"}, 360},
            {{"7th3"}, 370},

            {{"6th4"}, 460},
            {{"7th4"}, 470},

            {{"6th5"}, 560},
            {{"7th5"}, 570},
        },
        2,
        hasher,
        equaler};

    um1_1.insert({{"8th"}, 80});
    um1_1.emplace(MyKey1{"7th"}, 70);

    // MyKey2
    std::cout << "----------MyKey2----------" << std::endl;
    std::unordered_map<MyKey2, int, decltype(&MyKey2_Hash), decltype(&MyKey2_Compare)> um2(
        10, MyKey2_Hash, MyKey2_Compare);
    um2 = {
        {{"1st", "_1_"}, 10}, {{"2nd", "_2_"}, 20}, {{"3st", "_3_"}, 30}, {{"5th", "_5_"}, 50},
        {{"6th", "_6_"}, 60}, {{"7th", "_7_"}, 70}, {{"4th", "_4_"}, 40},
    };
    um2.insert({{"8th", "_8_"}, 80});
    MyKey2 kk  = {"8th", "_8_"};
    auto   itk = um2.find(kk);
    if (itk != um2.end()) {
        std::cout << itk->first.ToString() << "|" << itk->second << std::endl;
    }

    // MyKey3
    std::cout << "----------MyKey3----------" << std::endl;
    std::unordered_map<MyKey3, int> um3 = {
        {{"1st", "_1_", "one"}, 10},  {{"2nd", "_2_", "two"}, 20}, {{"3st", "_3_", "three"}, 30},
        {{"5th", "_5_", "five"}, 50}, {{"6th", "_6_", "six"}, 60}, {{"7th", "_7_", "seven"}, 70},
        {{"4th", "_4_", "four"}, 40},
    };
    um3.emplace(MyKey3{"8th", "_8_", "eight"}, 80);
    MyKey3 kkk = {"5th", "_5_", "five"};
    auto   it  = um3.find(kkk);
    if (it != um3.end()) {
        std::cout << it->first.ToString() << "|" << it->second << std::endl;
    }

    // MyHashMap
    std::cout << "----------MyHashMap----------" << std::endl;
    MyHashMap tm{1};
    tm.add("1", 10);
    tm.add("1a", 10);
    tm.add("1ab", 10);
    tm.add("1abc", 10);
    tm.add("2", 10);

    tm.print();
}

