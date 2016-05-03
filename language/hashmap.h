#ifndef HASHMAP_H
#define HASHMAP_H

#include "../main/header.h"
#include "classtest.h"

// http://marknelson.us/2011/09/03/hash-functions-for-c-unordered-containers/
// http://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key

// method 1), hash function and compare function are in separate class/struct 
struct MyKey1 {
    string first;
    string ToString() const {
        return "first:"+first;
    }
};
// hash object is only created once per unordered_map
struct MyKey1_Hash {
    size_t operator()(const MyKey1& key) const {
        return hash<string>()(key.first);
    }
};
// compare object is only created once per unordered_map
struct MyKey1_Compare {
    bool operator()(const MyKey1& k1, const MyKey1& k2) const {
        return k1.first == k2.first;
    }
};

// method 2: hash and compare are in separate functions outside of MyKey class
struct MyKey2 {
    string first;
    string second;
    string ToString() const {
        return "first:"+first+",second:"+second;
    }
};
size_t MyKey2_Hash(const MyKey2& k) {
    return 
        hash<string>()(k.first) ^
        (hash<string>()(k.second) << 1);
}
bool MyKey2_Compare(const MyKey2& k1, const MyKey2& k2) {
    return k1.first == k2.first && k1.second == k2.second;
}

// method 3: compare is inside MyKey class, hash is specilized within std::hash
struct MyKey3 {
    string first;
    string second;
    string third;
    string ToString() const {
        return "first:"+first+",second:"+second+",third:"+third;
    }
    bool operator==(const MyKey3& k) const {
        return this->first == k.first && this->second == k.second && this->third == k.third;
    }
};
namespace std {
    template<> struct hash<MyKey3> {
        size_t operator()(const MyKey3& k) const {
            return 
                (hash<string>()(k.first) ^
                (hash<string>()(k.second) << 1) >> 1) ^
                (hash<string>()(k.third) << 1);
        }
    };
}

void RunHashmap() {
    // MyKey1
    unordered_map<MyKey1, int, MyKey1_Hash, MyKey1_Compare> um1 = {
        {{"1st"}, 10},
        {{"2nd"}, 20},
        {{"3st"}, 30},
        {{"5th"}, 50},
        {{"6th"}, 60},
        {{"7th"}, 70},
        {{"4th"}, 40},
    };
    um1.insert({{"8th"},80});
    um1.emplace(MyKey1{"7th"}, 70);

    MyKey1 k1 = {"2nd"};
    MyKey1 k2({"10th"});
    auto it1 = um1.find(k1);
    if (it1 != um1.end()) {
        cout << it1->first.ToString() << "|" << it1->second << endl;
    }
    auto it2 = um1.find(k2);
    if (it2 != um1.end()) {
        cout << it2->first.ToString() << "|" << it2->second << endl;
    }
    // MyKey2
    unordered_map<MyKey2, int, decltype(&MyKey2_Hash), decltype(&MyKey2_Compare)> um2(10, MyKey2_Hash, MyKey2_Compare); 
    um2 = {
        {{"1st","_1_"}, 10},
        {{"2nd","_2_"}, 20},
        {{"3st","_3_"}, 30},
        {{"5th","_5_"}, 50},
        {{"6th","_6_"}, 60},
        {{"7th","_7_"}, 70},
        {{"4th","_4_"}, 40},
    };
    um2.insert({{"8th","_8_"}, 80});
    MyKey2 kk = {"8th","_8_"};
    auto itk = um2.find(kk);
    if (itk != um2.end()) {
        cout << itk->first.ToString() << "|" << itk->second << endl;
    }
    // MyKey3
    unordered_map<MyKey3, int> um3 = {
        {{"1st","_1_","one"}, 10},
        {{"2nd","_2_","two"}, 20},
        {{"3st","_3_","three"}, 30},
        {{"5th","_5_","five"}, 50},
        {{"6th","_6_","six"}, 60},
        {{"7th","_7_","seven"}, 70},
        {{"4th","_4_","four"}, 40},
    };
    um3.emplace(MyKey3{"8th","_8_","eight"}, 80);
    MyKey3 kkk = {"5th","_5_","five"};
    auto it = um3.find(kkk);
    if (it!= um3.end()) {
        cout << it->first.ToString() << "|" << it->second << endl;
    }
}

#endif


