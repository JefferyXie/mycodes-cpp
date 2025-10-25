#include "classtest.h"

//
// http://www.zhihu.com/question/29511959
// http://www.tuicool.com/articles/F7Fjuu3
// http://en.cppreference.com/w/cpp/language/copy_elision
//
Plain getPlain()
{
    return Plain();
}

Plain getPlainWithName()
{
    Plain temp;
    return temp;
}

std::vector<Plain> getVector()
{
    // copy elision from tempory object to 'temp', omit copy and move constructor
    std::vector<Plain> temp = std::vector<Plain>(1);
    std::cout << "message after vector is created." << std::endl;
    return temp;    // NRVO temp to returned
}
