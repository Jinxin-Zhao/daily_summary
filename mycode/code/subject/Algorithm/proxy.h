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

template <class T>
class RCPtr{
public:
    RCPtr(T * realPtr = 0);
    RCPtr(const RCPtr & rhs);
    ~RCPtr();

    RCPtr &operator= (const RCPtr & rhs);

    T *operator->() const;
    T &operator*() const;

private:
    T * pointee;
    void init();
};



class RCObject{
public:
    void addReference();
    void removeReference();
    void markUnshareable();
    bool isShareable() const;
    bool isShared() const;

protected:
    RCObject();
    RCObject(const RCObject & rhs);
    RCObject &operator=(const RCObject &rhs);
    virtual ~RCObject() = 0; //pure destructor must be implemented

private:
    int refCount;
    bool shareable;
};

class String{
public:
    String(const char * iniValue="");

    //original version
//    const char &operator[](int index) const;
//    char &operator[](int index);

    //functions below will not be defined for compiler will generate default func
    //default copy constructor will call RCPtr member's copy constructor

    //String(const String & rhs);
    //String &operator=(const String &rhs);
    //~String();
public:
    class CharProxy{
    public:
        CharProxy(String & str,int index);
        CharProxy &operator=(const CharProxy & rhs); //lvalue operator=
        CharProxy &operator=(char c); //lvalue operator=
        operator char() const;
        char * operator&();   //for &s[1],such as " char * p = &s[1] //error ",&s[1] is an address of CharProxy Object,
        const char * operator&() const;
    private:
        String & theString;
        int charIndex;
    };

    const CharProxy operator[](int index) const;
    CharProxy operator[](int index);
    friend class CharProxy;

private:
    struct StringValue : public RCObject{
        char * data;
        StringValue(const char *initValue);
        StringValue(const StringValue & rhs);
        void init(const char * initValue);
        ~StringValue();
    };

private:
    RCPtr<StringValue> value;
};



#endif
