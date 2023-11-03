#include <iostream>
using namespace std;

//class A{};
//class B : public A{};
//class C : public A{};
//void display(const auto_ptr<A> &pmp)
//auto_ptr<B> b;
//auto_ptr<C> c;
//display(b)   //error cannot convert auto_ptr<B> to auto_ptr<A>
//display(c)   //error cannot convert auto_ptr<C> to auto_ptr<A>

template <typename T>
class auto_ptr{
public:
    auto_ptr(auto_ptr<T> & rhs);
    auto_ptr<T> & operator=(auto_ptr<T> &rhs);
    ~auto_ptr();
    T & operator *() const;
    T * operator ->() const;

    //explicit conversion to confirm if auto_ptr is null 
    //it's perilous if you call like this:
    //auto_ptr<Apple> pa;
    //autp_ptr<orange> pb;
    //if(pa == pb)  //compile success!
    operator void*();   //not suggested

    //while object is nullptr
    //auto_ptr<TreeNode> ptn;
    //if(!pn){}else{}    //correct
    //if(pn)  or if(ptn == 0) //error
    bool operator !() const;   //not suggested

    template <typename newtype>
    operator auto_ptr<newtype>(){
        return auto_ptr<newtype>(pointee);
    }

private:
    T * pointee;
};

template <typename T>
auto_ptr<T>::auto_ptr(auto_ptr<T>& rhs){
    pointee = rhs.pointee;
    rhs.pointee = nullptr;
}

template <typename T>
auto_ptr<T>::~auto_ptr(){
    if(this.pointee != nullptr){
        delete pointee;
    }
    pointee = nullptr;
}

T& auto_ptr<T>::operator *() const{
    return *pointee;
}

T * auto_ptr<T>::operator->() const{
    return pointee;
}

template <typename T>
auto_ptr <T> & auto_ptr<T>::operator=(auto_ptr<T>& rhs)
{
    if(this == &rhs){
        return this;
    }
    delete pointee;
    pointee = rhs.pointee;
    rhs.pointee = nullptr;
    return *this;
}
