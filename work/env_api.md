###simple_5
+ front1: 172.16.68.51   10041
+ front2: 172.16.68.51   10042
+ mdkernel: 172.16.68.51 10021

###simple_7
+ front1: 172.16.68.71   10041
+ front2: 172.16.68.71   10042
+ mdkernel: 172.16.68.71 10021

###substite JNI_so file(example: userapiJNI)
- make the so files
    + ]# ./mapi
    + ]# cd matcher/kernal/api/
    + ]# cd kernal/source/apiuserapi
    + ]# cp QTFtdcUserApi.h QTFtdcUserApiDataType.h QTFtdcUserApiStruct.h ../../api/userapiJNI/

    + ]# cp QTFtdcUserApiDataType.h QTFtdcUserApiStruct.h ../../api/mduserapiJNI/
    + ]# cd ../apimduserapi/ && cp QTFtdcMduserApi.h ../../api/mduserapiJNI
    + ]# cd ../../api/userapi
    + ]# cp *.so ../userapiJNI/
    + ]# cd ../userapiJNI/ && amake *.prj
    + ]# make
    + ]# cd ../mduserapi && cp *.so ../mduserapiJNI/
    + ]# cd ../mduserapiJNI && amake *.prj
    + ]# cd linux && cp *.h ../
    + ]# make

- substitute(env_host: simple_5)
    + ==[webmarket:172.16.68.54]==
    + ]# scp kernal/api/mduserapi/*.so   amazon@172.16.68.54:/opt/jetty/jetty-9.2.14/lib/linux/liunx_x64
    + ]# scp kernal/api/userapi/*.so   amazon@172.16.68.54:/opt/jetty/jetty-9.2.14/lib/linux/liunx_x64
    + ]# scp kernal/api/mduserapiJNI/bin/*.so amazon@172.16.68.54:/opt/jetty/jetty-9.2.14/lib/linux/liunx_x64
    + ]# scp kernal/api/userapiJNI/bin/*.so  amazon@172.16.68.54:/opt/jetty/jetty-9.2.14/lib/linux/liunx_x64
    ---
    + ]# scp kernal/api/mduserapi/*.so   amazon@172.16.68.54:/opt/jetty/jetty-9.2.14/lib/liunx_x64
    + ]# scp kernal/api/userapi/*.so   amazon@172.16.68.54:/opt/jetty/jetty-9.2.14/lib/liunx_x64
    + ]# scp kernal/api/mduserapiJNI/bin/*.so amazon@172.16.68.54:/opt/jetty/jetty-9.2.14/lib/liunx_x64
    + ]# scp kernal/api/userapiJNI/bin/*.so  amazon@172.16.68.54:/opt/jetty/jetty-9.2.14/lib/liunx_x64

    + ==[webfront:172.16.68.55]==
    + the same operation with webmarket

    + ==[webback:172.16.68.56]==
    + the same operation with webmarket
- look up logs
    + ==[JNI_logs]==
    + ]# cd /opt/jetty/jetty-9.2.14/log.txt
    + ==[java logs]==
    + ]# cd /opt/jetty/jetty-9.2.14/logs


