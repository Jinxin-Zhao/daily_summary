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
 

# compile error: fatal error C1001
+ include directory error, in path: C:\Program Files (x86)\Windows Kits\10\Include\10.0.20348.0
  1. open Developer Command Prompt for VS 2022;
  2. vcvarsall.bat x86 (compiler 32bit program)
  3. set INCLUDE= C:\Program Files (x86)\Windows Kits\10\Include\10.0.20348.0:%INCLUDE% // switch to different include version
  4. nmake

# recognize the dll (32bit / 64bit)
+ After compilation of dll, run the cmd below to see dll (32bit or 64bit, run in 'x86 Native Tools Command Prompt for VS 2022') :
  ```shell
  ]$ dumpbin /headers QTuserapi.dll | find "machine"

  # 14C machine (x86)
  #    32 bit word machine
  ```
+ run the cmd below to see if the dll exports the function successfully (run in 'x86 Native Tools Command Prompt for VS 2022'):
    ```shell
    ]$ dumpbin /exports QTuserapi.dll

    # output:
    Microsoft (R) COFF/PE Dumper Version 14.44.35209.0
    Copyright (C) Microsoft Corporation.  All rights reserved.


    Dump of file QTuserapi.dll

    File Type: DLL

    Section contains the following exports for QTuserapi.dll

    00000000 characteristics
    FFFFFFFF time date stamp
        0.00 version
           1 ordinal base
           7 number of functions
           7 number of names

     ordinal hint RVA      name

          1    0 00090BA0 ??0CQTFtdcUserApi@@QAE@ABV0@@Z
          2    1 00090BB0 ??0CQTFtdcUserApi@@QAE@XZ
          3    2 00090BC0 ??1CQTFtdcUserApi@@IAE@XZ
          4    3 00090BD0 ??4CQTFtdcUserApi@@QAEAAV0@ABV0@@Z
          5    4 000E32FC ??_7CQTFtdcUserApi@@6B@
          6    5 00090C10 ?CreateFtdcUserApi@CQTFtdcUserApi@@SAPAV1@PBD0@Z
          7    6 00090CE0 ?GetVersion@CQTFtdcUserApi@@SAPBDXZ

     Summary

      35E000 .data
        1000 .fptable
       26000 .rdata
       11000 .reloc
       DC000 .text
    ```