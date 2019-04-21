### jsoncpp installation

+ packages required:
    - scons-2.1.0.tar.gz (http://sourceforge.net/projects/scons/files/scons/2.1.0/scons-2.1.0.tar.gz/download)
    - jsoncpp-src-0.5.0.tar.gz (http://sourceforge.net/projects/jsoncpp/files/)
+ installation:
    - ]# tar -zxvf scons-2.1.0.tar.gz
    - ]# tar -zxvf jsoncpp-src-0.5.0.tar.gz
    - ]# cd scons-2.1.0
    - ]# sudo python setup.py install
    - ]# cd ../jsoncpp-src-0.5.0
    - ]# sudo scons platform=linux-gcc
    - ]# sudo cp include/json /usr/include
    - ]# sudo cp libs/*.a /usr/local/lib/
    - ]# sudo mv libjson-linux-gcc_mt.a libjson.a