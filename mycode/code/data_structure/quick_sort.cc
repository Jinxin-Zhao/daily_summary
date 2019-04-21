#include <iostream>
using namespace std;

void swap(int & a,int & b)
{
	int temp = a;
	a = b;
	b = temp;
}

int partition(int s[],int low,int high)
{
	int i =  low - 1,j = low;
	int temp = s[high];
	for(;j < high;++j)
	{
		if(s[j] <= temp)
		{
			i++;
			if(i != j)
			{
				swap(s[i],s[j]);
			}
		}
	}
	swap(s[i+1],s[high]);
	return (i+1);
}

void quicksort(int s[], int low,int high)
{
	while(low < high)
	{
		int pivot = partition(s,low,high);	
		quicksort(s,low,pivot-1);
		quicksort(s,pivot+1,high);
	}
}

int main()
{
	cout<<"start to quicksort"<<endl;
	int s[] = {2,8,7,9,4,10,6};
	quicksort(s,0,6);
	for(int i = 0;i < 7;++i)
	{
		cout<<s[i]<<" ";
	}
	cout<<endl;
	return 0;
}