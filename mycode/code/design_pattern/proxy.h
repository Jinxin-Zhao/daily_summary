#ifndef  _H_PROXY_
#define  _H_PROXY_

//template <typename T>
//class Array2D{
//public:
//    struct Proxy{
//        Proxy(Array<T> & array,int index);
//        Proxy &operator=(const T&rhs);
//        //T &operator[](int index);
//        //const T &operator[](int index) const ;
//        operator T() const;
//    };
//
//    Proxy operator[](int index);
//    const Proxy operator[](int index) const;
//};

class String{
public:
    String(const char * iniValue);
    String(const String & rhs);
    String &operator=(const String &rhs);
    ~String();

    //original version
    const char &operator[](int index) const;
    char &operator[](int index);
public:
    class CharProxy{
    public:
        CharProxy(String & str,int index);
        CharProxy &operator=(const CharProxy & rhs); //lvalue operator=
        CharProxy &operator=(char c); //lvalue operator=
        operator char() const;
        char * operator&();   //for &s[1]
        const char * operator&() const;
    private:
        String & theString;
        int charIndex;
    };

    //const CharProxy operator[](int index) const;
    //CharProxy operator[](int index);
    friend class CharProxy;

private:
    struct StringValue{
        int refCount;
        char * data;
        bool shareable;
        StringValue(const char *initValue);
        ~StringValue();
    };

private:
    //RCPtr<StringValue> value;
    StringValue value;
};



#endif
