#include  <iostream>
#include  <algorithm>
#include <vector>

using namespace std;

void print(vector<int> & ivec){
	for(int i = 0;i < ivec.size();++i){
		cout<<ivec[i]<<' ';
	}
	cout<<endl;
}


int main(){
	int ia[9] = {0,1,2,3,4,8,9,3,5};
	vector<int> ivec(ia,ia+9);

	cout<<"after making heap...\n";
	make_heap(ivec.begin(),ivec.end());
	print(ivec);

	cout<<"after inserting 1 elem into heap...\n";
	ivec.push_back(7);
	push_heap(ivec.begin(),ivec.end());
	print(ivec);

	pop_heap(ivec.begin(),ivec.end());
	cout<<ivec.back()<<endl;
	ivec.pop_back();

	print(ivec);

	cout<<"after sorting heap...\n";
	sort_heap(ivec.begin(),ivec.end());
	print(ivec);


	return 0;
}