# env configuration
## C++ env variables configuration in win11(suppose you've already installed microsoft Visual studio)
+ 系统属性(tab:高级)->环境变量
+ include: D:\Program Files\Microsoft Visual Studio\2022\Community\SDK\ScopeCppSDK\vc15\SDK\include\ucrt; // 包含stdio.h标准头文件
           D:\Program Files\Microsoft Visual Studio\2022\Community\SDK\ScopeCppSDK\vc15\SDK\include\um
           D:\Program Files\Microsoft Visual Studio\2022\Community\SDK\ScopeCppSDK\vc15\VC\include
+ lib:     D:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.40.33807\lib\x86
           D:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.40.33807\lib\x64
           D:\Program Files\Microsoft Visual Studio\2022\Community\SDK\ScopeCppSDK\vc15\VC\lib
+ open terminal: Developer Command Prompt for VS 2022
  
## compiler error of file encode format
+ If your project files format is UTF-8, you can configure your win11 system file format to UTF-8:
  设置->时间和语言->语言和区域->管理语言设置->更改系统区域设置->[勾选]"Beta版: 使用Unicode UTF-8提供全球语言支持"

# 

# compile error: fatal error C1001
+ include directory error, in path: C:\Program Files (x86)\Windows Kits\10\Include\10.0.20348.0
  1. open Developer Command Prompt for VS 2022;
  2. vcvarsall.bat x86 (compiler 32bit program)
  3. set INCLUDE= C:\Program Files (x86)\Windows Kits\10\Include\10.0.20348.0:%INCLUDE% // switch to different include version
  4. nmake
