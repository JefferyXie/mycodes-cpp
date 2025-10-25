#ifndef MAX_POINTS_ON_LINE_H
#define MAX_POINTS_ON_LINE_H

#include "../main/header.h"

//
// Given a set of points, find line with the most number of points
//

int gcd(int a, int b)
{
    return a ? gcd(b % a, a) : b;
}

struct point {
    int x;
    int y;
};

// coeff_y * y = coeff_x * x + interception
struct line {
    int  coefficient_x;
    int  coefficient_y;
    int  interception;
    bool create(int coeff_x, int coeff_y, int inter)
    {
        if (coeff_x == 0 && coeff_y == 0 && inter == 0) {
            return false;
        }
        int t_gcd;
        if (inter == 0 && coeff_x == 0) {
            t_gcd = coeff_y;
        } else if (inter == 0 && coeff_y == 0) {
            t_gcd = coeff_x;
        } else if (coeff_x == 0) {
            t_gcd = gcd(coeff_y, inter);
        } else if (coeff_y == 0) {
            t_gcd = gcd(coeff_x, inter);
        } else if (inter == 0) {
            t_gcd = gcd(coeff_x, coeff_y);
        } else {
            t_gcd = gcd(coeff_x, coeff_y);
            t_gcd = gcd(t_gcd, inter);
        }
        coefficient_x = coeff_x / t_gcd;
        coefficient_y = coeff_y / t_gcd;
        interception  = inter / t_gcd;
        return true;
    }
    bool operator==(const line& ref) const
    {
        return this == &ref || (coefficient_x == ref.coefficient_x && coefficient_y == ref.coefficient_y &&
                                interception == ref.interception);
    }
};

namespace std {
template <>
struct hash<line> {
    size_t operator()(const line& ln) const
    {
        return hash<decltype(ln.coefficient_x)>()(ln.coefficient_x) ^
               hash<decltype(ln.coefficient_y)>()(ln.coefficient_y << 1) ^
               hash<decltype(ln.interception)>()(ln.interception << 2);
    }
};
}    // namespace std

//
// keep the lines in hash set and the number of points
// O(n^2)
//
int max_points_on_line(std::vector<point> points)
{
    std::unordered_map<line, int> line_points;
    for (unsigned i = 0; i < points.size(); ++i) {
        const auto& pi = points[i];
        for (unsigned j = i + 1; j < points.size(); ++j) {
            const auto& pj      = points[j];
            int         coeff_x = pi.y - pj.y;
            int         coeff_y = pi.x - pj.x;
            int         inter   = coeff_y * pi.y - coeff_x * pi.x;
            line        t_line;
            if (t_line.create(coeff_x, coeff_y, inter)) {
                line_points[t_line]++;
            } else {
                throw "wrong input points";
            }
        }
    }

    int max_points = 0;
    for (auto li : line_points) {
        max_points = max_points > li.second ? max_points : li.second;
    }
    return max_points;
}

#endif

