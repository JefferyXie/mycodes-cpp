#ifndef HYBRID_RANGE_H
#define HYBRID_RANGE_H

#include "../main/header.h"
#include "../main/utility.h"

struct st_1 {
    int v = 0;
    st_1(int i) : v(i) {}
};
struct st_2 : public st_1 {
    st_2(int i) : st_1(i) {}
};
struct st_3 : public st_1 {
    st_3(int i) : st_1(i) {}
};

// 
// https://stackoverflow.com/questions/8164567/how-to-make-my-custom-type-to-work-with-range-based-for-loops
// 
// hybrid_range is designed to iterate two std containers continuously and 
// facilitate the use in for-each
// 
// 1) the two containers' value type can be different as long as they are
//    inheriting from same base. e.g. st_2 and st_3 have same base st_1.
//    
// 2) custom container is also supported as long as begin & end is overrided,
//    operator ++ should be overrided to iterate element.
//    
// 3) 1) doesn't apply to map since map's value type is std::pair. If we wanna
//    combine two maps that with different value type, like map<int, st_2> and
//    map<int, st_3>, we need to specialize the hybrid_iterator so that it can
//    convert both pair<int, st_2> and pair<int, st_3> into pair<int, st_1>,
//    from which st_1 is base of st_2 and st_3.
//    

template <typename Range_O, typename Range_CO, typename Result_t = st_1>
class hybrid_range {
    Range_O  range_o_;
    Range_CO range_co_;
public:
    template <typename Iter_O, typename Iter_CO>
    class hybrid_iterator {
        Iter_O  o_;
        Iter_O  end_o_;
        Iter_CO co_;
        Iter_CO end_co_;
        Result_t* result_;
    public:
        hybrid_iterator(Iter_O begin_o,   Iter_O end_o,
                        Iter_CO begin_co, Iter_CO end_co) : 
            o_(begin_o),   end_o_(end_o),
            co_(begin_co), end_co_(end_co), result_(nullptr)
        {
            if (o_ != end_o_) {
                result_ = &(*o_);
                ++o_;
            }
            else if (co_ != end_co_) {
                result_ = &(*co_);
                ++co_;
            }
        }
        hybrid_iterator& operator++() {
            if(o_ != end_o_) {
                result_ = &(*o_);
                ++o_;
                return *this;
            }
            if (co_ != end_co_) {
                result_ = &(*co_);
                ++co_;
                return *this;
            }
            result_ = nullptr;
            return *this;
        }
        bool operator!=(const hybrid_iterator& rhs) const {
          return result_ != rhs.result_;
        }
        Result_t& operator*() {
            return *result_;
        }
    };

    hybrid_range(Range_O&& o, Range_CO&& co) :
        range_o_(std::move(o)), range_co_(std::move(co))
    {}

    auto begin() {
        return hybrid_iterator<decltype(std::begin(range_o_)),
                               decltype(std::begin(range_co_))>{
            std::begin(range_o_), std::end(range_o_),
            std::begin(range_co_),std::end(range_co_)};
    }
    auto end() {
        return hybrid_iterator<decltype(std::begin(range_o_)),
                               decltype(std::begin(range_co_))>{
            std::end(range_o_), std::end(range_o_),
            std::end(range_co_),std::end(range_co_)};
    }
};

auto get_range_vec() {
    vector<st_2> v_2 = {st_2{1}, st_2{2}};
    vector<st_3> v_3 = {st_3{3}, st_3{4}, st_3{5}};
    return hybrid_range<decltype(v_2), decltype(v_3)>(
                        std::move(v_2), std::move(v_3));
}
auto get_range_arr() {
    st_2 arr_2[] = {st_2{1}, st_2{2}};
    st_3 arr_3[] = {st_3{3}, st_3{4}, st_3{5}};
    return hybrid_range<decltype(arr_2), decltype(arr_3)>(
                        std::move(arr_2), std::move(arr_3));
}
auto get_range_map() {
    map<int, st_2> m_2 = {{1, st_2{1}}, {2, st_2{2}}};
    map<int, st_2> m_3 = {{3, st_2{3}}, {4, st_2{4}}, {5, st_2{5}}};
    using Result_Type = map<int, st_2>::value_type;
    return hybrid_range<decltype(m_2), decltype(m_3), Result_Type>(
                        std::move(m_2), std::move(m_3));

    // TODO: if we wanna combine two maps that with different value type, like
    // map<int, st_2> and map<int, st_3>, we need to specialize the hybrid
    // iterator so that it can convert both pair<int, st_2> and pair<int, st_3>
    // into pair<int, st_1>, from which st_1 is base of st_2 and st_3.
}

void run_hybrid_range()
{
    FUNC_BEGIN(run_hybrid_range);
    SECTION_BEGIN(vector);
    for (auto& st : get_range_vec()) {
        cout << st.v << endl;
    }
    SECTION_BEGIN(array);
    for (auto& st : get_range_arr()) {
        cout << st.v << endl;
    }
    SECTION_BEGIN(map);
    for (auto& m : get_range_map()) {
        cout << m.first << "," << m.second.v << endl;
    }
    FUNC_END(run_hybrid_range);
}

#endif

