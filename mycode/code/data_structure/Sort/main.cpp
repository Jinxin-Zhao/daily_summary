#include "Sorting.h"

int main() {
    std::cout << "Hello, Let's sorting ~" << std::endl;
    Sorting s;
    int array [] = {3,2,1,5,5,5,8,9,19,10,8,13};
    int len = sizeof(array)/sizeof(int);
    s.print(array,len);
    //s.InsertSort(array,len);
    //s.ShellSort(array,len);
    s.SelectSort(array,len);
    s.print(array,len);

    int narray[]={0,45,78,55,47,4,1,2,7,8,96,36,45};
    int num_array = sizeof(narray)/sizeof(narray[0]);
    // 寻找第k最小数
    int k = 4;
    int i = s.q_select(narray, 0, num_array - 1, k);
    std::cout <<"the "<<k<<"th element is "<< i << std::endl;
    return 0;
}