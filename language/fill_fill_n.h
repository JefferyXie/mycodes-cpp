#include <stdio.h>
#include <iostream>

using namespace std;

void fill_fill_n() {
    int bb[3];
    std::fill_n(bb, sizeof(bb)/sizeof(int), 5);
    std::fill_n(&bb[0], sizeof(bb)/sizeof(int), 5);
    std::fill_n(begin(bb), sizeof(bb)/sizeof(int), 5);

    int aa[3][2];
    for (int i = 0; i < 3; ++i) 
        for (int j = 0; j < 2; ++j) 
            cout << aa[i][j] << " ";
    cout << endl;
    std::fill(*aa, *aa+sizeof(aa)/sizeof(int), 2);
    std::fill(begin(*aa), end(*aa), 1);
    //memset((void*)aa, 2, sizeof(aa)/sizeof(int));
    for (int i = 0; i < 3; ++i) 
        for (int j = 0; j < 2; ++j) 
            cout << aa[i][j] << " ";
    cout << endl;
    std::fill_n(*aa, sizeof(aa)/sizeof(**aa), 3);
    //memset(aa, 3, sizeof(aa)/sizeof(int));
    for (int i = 0; i < 3; ++i) 
        for (int j = 0; j < 2; ++j) 
            cout << aa[i][j] << " ";
    cout << endl;
    cout << "aa:" << aa << endl;
    cout << "*aa:" << *aa << endl;
    cout << "**aa:" << **aa << endl;
    cout << "aa[0]:" << aa[0] << endl;
    cout << "&aa[0]:" << &aa[0] << endl;
    cout << "aa[0][0]:" << aa[0][0] << endl;
    cout << "&aa[0][0]:" << &aa[0][0] << endl;
    cout << "begin(*aa)-end(*aa):" << begin(*aa)-end(*aa) << endl;
}

