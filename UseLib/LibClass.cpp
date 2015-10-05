#include "LibClass.h"

SharedClass::SharedClass() {
    cout << "SharedClass()" << endl;
    _n = 100;
    _name = "SharedClass object";
}

string SharedClass::Process(const string& strInput1, const string& strInput2) {
    cout << "SharedClass::Process(const string&, const string&)" << endl;
    return strInput1 + strInput2; 
}


