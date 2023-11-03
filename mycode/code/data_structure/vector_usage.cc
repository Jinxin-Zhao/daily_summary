#include <iostream>
#include <vector>

using namespace std;

int main()
{

	vector<int> v;
	for(int i = 0; i < 10; ++i){
		v.push_back(i);
	}
	vector<int>::iterator it = v.begin();
	for(; it != v.end(); ++it){
		cout<<*it<<"  ";
	}
	cout<<endl;
	v.erase(v.begin() + 2);
	//it = v.begin();
	v.push_back(9);
	v.push_back(9);
	v.push_back(9);
	v.push_back(18);
	v.push_back(9);
	v.push_back(9);
	v.push_back(9);
	v.push_back(9);
	v.push_back(9);
	v.push_back(9);

	//这里的it赋值和19行的区别就是，以上push_back操作过程中发生了resize操作，
	//如果将下面屏蔽，打开19行，就会出现打印出的两个地址的容器内容，因为19行的iterator指向
	//的是原始的内存块的内容
	it = v.begin();
	cout<<endl<<"now the  size is: "<<v.size()<<" capacity: "<<v.capacity()<<endl;
	for(; it != v.end(); ++it){
		cout<<*it<< "  ";
	}

	cout<<endl<<"now the  size is: "<<v.size()<<" capacity: "<<v.capacity()<<endl;
	it = v.begin();
	for(; it != v.end();){
		if(*it == 9){
			v.erase(it);
		}else
		{
			it++;
		}
	}
	cout<<endl;
	vector<int>::iterator i = v.begin();
	for(; i != v.end(); ++i){
		cout<<*i<<"  ";
	}
	// it = v.begin();
	// for(; it != v.end(); ++it){
	// 	auto i = v.find(9);
	// 	cout<<*it<< "  ";
	// }

	return 0;
}

