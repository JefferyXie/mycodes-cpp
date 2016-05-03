#ifndef BIKERACER_H
#define BIKERACER_H

#include "../main/header.h"

// https://www.hackerrank.com/challenges/bike-racers
// https://en.wikipedia.org/wiki/Hungarian_algorithm
// http://www.cnblogs.com/across-fun/p/3574575.html
// https://github.com/zjsxzy/HackerRank/blob/master/Contest/20%5C20%20Hack%20February/Bike%20Racers/main.cpp
//
// list permutation with order consideration
template<class T> int PossiblePermutation_WithOrder(vector<T>& target, int pos, vector<vector<T> >& allTargets) {
    if ((int)target.size() < pos) return 0;
    if ((int)target.size() == pos) {
        /*cout << "<<<<<<<<<" << endl;
        cout << "\t";
        for_each(target.begin(), target.end(), [](int a) { cout << a << ","; });
        cout << endl;*/
        allTargets.push_back(target);
        return 1;
    }
    int count = 0;
    for (int i = pos; i < (int)target.size(); ++i) {
        iter_swap(target.begin()+pos, target.begin()+i);
        count += PossiblePermutation_WithOrder(target, pos+1, allTargets);
        iter_swap(target.begin()+pos, target.begin()+i);
    }
    return count;
}
// list permutation that has elements as M without order consideration
int PossiblePermutation_NoOrder(const vector<int>& source, int pos, int M, vector<int>& target) {
    if ((int)source.size() < M || (int)source.size() < pos) return -1;
    if ((int)target.size() == M) {
        for_each(target.begin(), target.end(), [](int a) { cout << a << ","; });
        cout << endl;
        return 1;
    }

    int count = 0;
    for (int i = pos; i < (int)source.size(); ++i) {
        target.push_back(source[i]);
        count += PossiblePermutation_NoOrder(source, i+1, M, target);
        target.erase(target.end()-1);
    }
    return count;
}
// list permutation that has elements as M with order consideration
int PossiblePermutation_WithOrder(const vector<int>& source, int pos, int M, vector<int>& target) {
    if ((int)source.size() < M || (int)source.size() < pos) return 0;
    if ((int)target.size() == M) {
        /*cout << "*******************" << endl;
        for_each(target.begin(), target.end(), [](int a) { cout << a << ","; });
        cout << endl;*/
        vector<vector<int> > allTargets;
        int n = PossiblePermutation_WithOrder(target, 0, allTargets);
        for_each(allTargets.begin(), allTargets.end(), [](vector<int>& v) {
            cout << "******" << endl;
            for_each(v.begin(), v.end(), [](int a) { cout << a << ","; });
            cout << endl;
        });
        return n;
    }

    int count = 0;
    for (int i = pos; i < (int)source.size(); ++i) {
        target.push_back(source[i]);
        count += PossiblePermutation_WithOrder(source, i+1, M, target);
        target.erase(target.end()-1);
    }
    return count;
}
struct Coordinate {
    int X;
    int Y;
    long long SquareDistance(const Coordinate& d) const {
        return (long long)(X-d.X)*(X-d.X)+(long long)(Y-d.Y)*(Y-d.Y);
    }
    string ToString() const {
        return "("+to_string(X)+","+to_string(Y)+")";
    }
};

// permutation with order
int PossiblePermutation(
                const vector<Coordinate>& s1, int pos, int perm_length, 
                vector<Coordinate>& target, 
                vector<vector<Coordinate> >& allTargets) {
    if ((int)s1.size() < pos) return 0;
    if ((int)target.size() == perm_length) {
        //for_each(target.begin(), target.end(), [](Coordinate& a) { cout << a.ToString() << ","; });
        //cout << endl;
        return PossiblePermutation_WithOrder(target, 0, allTargets);
    }

    int count = 0;
    for (int i = pos; i < (int)s1.size(); ++i) {
        target.push_back(s1[i]);
        count += PossiblePermutation(s1,i+1, perm_length, target, allTargets);
        target.erase(target.end()-1);
    }
    return count;
}
vector<vector<Coordinate> > ListAllPermutation(const vector<Coordinate>& s, int perm_length) {
    vector<Coordinate> target;
    vector<vector<Coordinate> > allTargets;
    PossiblePermutation(s, 0, perm_length, target, allTargets);
    return allTargets;
}
long long CalcShortestTime(const vector<vector<Coordinate> >& s1, const vector<vector<Coordinate> >& s2, int Kth) {
    cout << s1.size() << "," << s2.size() << endl;
    if (s1[0].size() != s2[0].size() || Kth > (int)s1[0].size()) return -1;

    long long shortestTime = -1;
    for (int i = 0; i < (int)s1.size(); ++i) {
        for (int j = 0; j < (int)s2.size(); ++j) {
            //cout << "~~~~~~~~~~~~~~~~~~~" << endl;
            // record distance for all persons
            multiset<long long> allDistances;
            for (int k = 0; k < (int)s1[i].size(); ++k) {
                long long time = s1[i][k].SquareDistance(s2[j][k]);
                allDistances.insert(time);
            }
            // find the Kth person by ascending order
            auto it = allDistances.begin();
            advance(it, Kth-1);
            //cout << *it << endl;
            shortestTime = (shortestTime < 0 ? *it : min(*it, shortestTime));
        }
    }
    return shortestTime;
}

long long BikeRacerShortestTime(vector<Coordinate>& bikers, vector<Coordinate>& bikes, int Kth) {
    auto bikersPerm = ListAllPermutation(bikers, Kth);
    auto bikesPerm = ListAllPermutation(bikes, Kth);
    return CalcShortestTime(bikersPerm, bikesPerm, Kth);
}

#endif


