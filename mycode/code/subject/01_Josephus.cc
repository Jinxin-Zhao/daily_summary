//problem 1
#include <iostream>
#include <stdio.h>
using namespace std;


struct Node
{
	int value;
	struct Node * next;
};


void Create_Ring_list(Node * pHead,int num)
{
	Node * p_Cur = NULL;
	Node * p_Prev = NULL;

	int i = 1;
	p_Prev = pHead;
	while(num--)
	{
		p_Cur = new Node();
		++i;
		p_Cur->value = i;
		p_Prev->next = p_Cur;
		p_Prev = p_Cur;
	}
	p_Cur->next = pHead;
}

void Print_lastone(Node * pHead,int m)
{
	Node * p_Cur = NULL;
	Node * p_Prev = NULL;
	p_Cur = p_Prev = pHead;
	int count = 1;
	while(p_Cur != NULL)
	{
		if(count == m)
		{
			p_Prev->next = p_Cur->next;
			delete p_Cur;
			p_Cur = p_Prev->next;
			count = 1;
		}
		p_Prev = p_Cur;
		p_Cur = p_Cur->next;
		if(p_Prev == p_Cur)
		{
			printf("last person number is:%d",p_Cur->value);
			delete p_Cur;
			break;
		}
		++count;
	}
}

int main()
{
	cout<<"[please input 2 number]:n & m:"<<endl;
	int n,m;
	cin>>n>>m;
	Node * pHead = new Node();
	pHead->value = 1;
	pHead->next = NULL;
	Create_Ring_list(pHead,n);
	Print_lastone(pHead,m);
	return 0;
}
