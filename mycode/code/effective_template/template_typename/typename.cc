#include <iostream>
#include <string>
using namespace std;


template <class T>
struct MyInter{
    typedef T value_type;
    T *ptr;
    MyInter(T *p = 0) : ptr(p) {}

};

template <class I>
struct iterator_traits_a{
    typedef typename I::value_type value_type;
};

template <class I>
typename iterator_traits_a<I>::value_type func(I ite)
{
    return *ite;
}

//
struct MsgInfo
{
    string a;
};

class company_a
{
public:
    void sendCleartext(const string & msg) {cout<<msg.c_str()<<endl;}
    void sendEncrypt(const string & msg);
};

class company_b
{
public:
    void sendCleartext(const string & msg);
    void sendEncrypt(const string & msg);
};

template <typename company>
class MsgSender{
public:
    void SendClear(const MsgInfo & info)
    {
        company c;
        c.sendCleartext(info.a);
    }
    virtual void send_text() {cout<<"this is MsgSender sending\n";}
};

template <typename company>
class LoggingMsgSender : public MsgSender<company>
{
public:
    //第一种写法，在下面函数就可以直接写:SendClear(info)
    //using MsgSender<company>::SendClear;
    //virtual void send_text() {cout<<"this is LoggingMsgSender sending\n";}
    using MsgSender<company>::send_text ;
    void SendClearMsg(const MsgInfo & info)
    {
        //SendClear(info);      //error
        //第二种写法,编译器根本不知道父类是否有这个函数，只能像下面三种写法
        this->SendClear(info);
        send_text();
        //第三种写法
        //MsgSender<company>::SendClear(info);
    }
};

int main()
{
    //MyInter<int> ite(new int(8));
    //cout<<func(ite);
    LoggingMsgSender<company_a>  o_log;
    MsgInfo  info;
    info.a = "zhaojx_a";
    //子类直接调用父类可以调用成功, 因为已经实例化了company_a,直接可以调用父类的函数
    //o_log.SendClear(info);
    o_log.SendClearMsg(info);
    return 0;
}