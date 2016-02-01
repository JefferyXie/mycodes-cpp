#include "../header.h"
#include "classtest.h"

//
// http://www.zhihu.com/question/29511959
// http://www.tuicool.com/articles/F7Fjuu3
// http://en.cppreference.com/w/cpp/language/copy_elision
//
Plain getPlain() {
    return Plain();
}

Plain getPlainWithName() {
    Plain temp;
    return temp;
}

vector<Plain> getVector() {
    // copy elision from tempory object to 'temp', omit copy and move constructor
    vector<Plain> temp = vector<Plain>(1); 

    cout << "message after vector is created." << endl;

    return temp; // NRVO temp to returned 
}


