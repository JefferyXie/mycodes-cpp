// 
// In 100% standard C++, you cannot include standard headers in an extern "C" block
// http://stackoverflow.com/questions/22115996/complex-type-with-c-linkage-in-c11
#include "../header.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "../classtest.h"

class SharedClass : public B {
    
public:
    SharedClass();
    virtual string Process(const string& strInput1, const string& strInput2);
};

#ifdef __cplusplus
}
#endif

