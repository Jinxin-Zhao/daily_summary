+ C++11 error: unable to find string literal operator 'operator"
    - 一个简单的宏
```
#define LOG_INFORMATION(x, ...) LOG_ME("%s:%d, "x, __FUNCTION__,__LINE__, ##__VA_ARGS__)
```

error: unable to find string literal operator 'operator""x'

一直都可以正常编译，但是当启用C++11的时候，报告编译错误;
网上搜了一下，说是C++11要求，当字符串跟变量连接的时候，必须增加一个空格才行。因此简单修改如下即可。
```
 #define LOG_INFORMATION(x, ...) LOG_ME("%s:%d, " x, __FUNCTION__,__LINE__, ##__VA_ARGS__)
#endif
```
GCC-4.9之前的版本都存在"__cplusplus"宏定义错误的问题，需要参考“NDK下GCC定义__cplusplus不正确的问题”，把GCC升级到>=4.9 version