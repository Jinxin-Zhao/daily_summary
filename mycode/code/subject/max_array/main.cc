#include <iostream>
using namespace std;

#define max(a,b) (a>b) ? a : b

int ar[] = {-2,1,-3,4,-1,2,1,-5,4};

int maxSubArray(int a[],int n)
{
    int sum = 0;
    int max = -9999;
    for(int i = 0;i < n;++i)
    {
        sum += a[i];
        if(sum > max)
        {
            max = sum;
        }
        if(sum < 0)
        {
            sum = 0;
        }
    }
    return max;
}

int d_max(int a[],int left,int right,int &maxV)
{
    if(left > right)
    {
        return -999;
    }
    int mid = (left + right)/2;
    int lmaxV = d_max(a,left,mid - 1,maxV);
    int rmaxV = d_max(a,mid+1,right,maxV);
    maxV = max(maxV,lmaxV);
    maxV = max(maxV,rmaxV);
    int sum = 0,mlmax = 0;
    for(int i = mid -1;i >= left;--i)
    {
        sum += a[i];
        if(sum > mlmax)
            mlmax = sum;
    }
    sum = 0;int mrmax = 0;
    for(int i = mid+1;i <= right;++i)
    {
        sum += a[i];
        if(sum > mrmax)
        {
            mrmax = sum;
        }
    }
    maxV = max(maxV,mlmax + mrmax + a[mid]);
    return maxV;
}


int divided_array(int a[],int n)
{
    int max = -999;
    return d_max(a,0,n-1,max);
}

int main()
{
    //int res = maxSubArray(ar,sizeof(ar)/sizeof(ar[0]));
    int res = divided_array(ar,sizeof(ar)/sizeof(ar[0]));
    cout<<"the max_subarray_is:"<<res<<endl;
    return 0;
}