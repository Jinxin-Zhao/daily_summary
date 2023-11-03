#include <iostream>
#include <stdio.h>
using namespace std;

// class GameCharacter
// {
// public:
// 	int healthValue()
// 	{
// 		int ret = doHealthValue();
// 		return ret;
// 	}
// private:
// 	virtual int doHealthValue() const
// 	{
// 		cout<<"this is base class"<<endl;
// 		return 0;
// 	}
// };
// class B : public GameCharacter
// {
//     virtual int doHealthValue()const{
//         cout<<"this is derived class"<<endl;
//         return 0;
//     }
// };

template<typename T>
class base{
public:
	void f1(){static_cast<T&>(*this).f1();}
};

class derived1:public base<derived1>{
public:
	void f1(){std::cout<<"this is derived class\n";}
};

//////////////////////////////////////////////
//global function
void f() {std::cout<<"global f\n";}

template<typename T>
class aTMP{
public:
	typedef const T reType;
};

template<class T>
class Base{
public:
	template<int N = 99>
	void f() {std::cout<<"base f()"<<N<<std::endl;}
};

/*note: member template function can't be virtual,the function below is wrong
template<int N = 99>
virtual void f() {std::cout<<"base f()"<<N<<std::endl;}
*/
//reason: the template functions are instantiated when being used,it causes the
//uncertainty of the size of vtables
template<typename T>
class Sub : public Base<T>{
public:
	typename T::reType m;
	Sub(typename T::reType a):m(a)
	{
	}
	void sf1() {this->f();}
	void sf2() {this->template f<100>();}
	void sf3() {::f();}
};

class test{
public:
	//test(int & tar):t(tar){cout << "Ref constructor" << endl;}
	//int & t;
	int b_r;
//private:
	test & operator=(const test & a){
		if(this == &a) {return *this;}
		b_r = a.b_r;
		return *this;
	}
};

class sub_test : public test{
public:
	// sub_test & operator=(const sub_test & b){
	// 	if(this == &b) {return *this;}
	// 	r = b.r;
	// 	return *this;
	// }

	int r;
};

int main()
{
    // char a = 255;
    // printf("%d\n",a);
    // printf("%d\n",sizeof(++a));
    // printf("%d\n",sizeof(char));

    // B b;
    // b.healthValue();
    base<derived1> y;
    y.f1();

///
    Sub<aTMP<int>> a(10);
    a.sf1();
    a.sf2();
    a.sf3();

// int temp = 3;
// test t(temp);

sub_test aa;
aa.r = 88;
sub_test s_t;
s_t = aa;
cout<<"sub_test:r "<<s_t.r<<endl;
// derived1 a;
// a.f1();
	return 0;
}
