#include <iostream>
#include <map>
#include "concrete_subject.h"
#include "concrete_observer.h"
using namespace std;


int main()
{
    ConcreteSubject * pSub = new ConcreteSubject();
    IObserver * o1 = new ConcreteObserver("zhaojx");
    IObserver * o2 = new ConcreteObserver("linjunjie");

    pSub->Attach(o1);
    pSub->Attach(o2);

    pSub->SetPrice(12.5);
    pSub->Notify();

    pSub->Detach(o2);
    pSub->SetPrice(66666.8);
    pSub->Notify();






/*
map<int, double> m;
//m[1] = 8.9;
//m[2] = 9.9;
int size = m.size();
cout<<"origin size is:"<<size<<endl;

m.clear();
size = m.size();
cout<<"clear size is:"<<size<<endl;


double a = 98.9;
double b = 88.8;
double res = (a+b)/10;
cout<<res<<endl;
*/
// int a = 0;
// int n = 6;
// while(n--)
// {
//   a = (1>0) ? ++a : a;
// }
// int m = 3;
// while(m--){
//  a = (1>0) ? a : a--;
// }
// cout<<"a is :"<<a<<endl;






return 0;

}
