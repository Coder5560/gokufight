
#ifndef __GameStore__CppJavaMethodManager__
#define __GameStore__CppJavaMethodManager__

#include "cocos2d.h"

class JavaCppBridge
{
public:
    static long long put(const std::function<void(int)> &callback);
};

#endif /* defined(__GameStore__CppJavaMethodManager__) */
