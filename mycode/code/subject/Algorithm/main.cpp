#include <iostream>
#include "Fibonacci.h"
#include "Quickselect.h"

int main() {
    //test Fibonacci
    Fibonacci fibonacci;
    int res_n = fibonacci.getFibnacci_iteraton(19);
    std::cout<<"f(n): "<<res_n<<std::endl;

    //test matrix method
    int matrix_res = fibonacci.getFibnacci_matrix(14);
    std::cout<<"matrix result: "<<matrix_res<<std::endl;
    //
    //srand(unsigned(time(NULL)));
    //for (int j = 0; j < num_array; j++)
    //array[j] = rand();
    // 寻找第k最小数
    Quickselect q;
    int k = 13;
    int i = q.q_select(array, 0, num_array - 1, k);
    std::cout <<"kth num is : "<< i << std::endl;
    for(int j = 0; j < num_array; j++){
        std::cout<<array[j]<<" ";
    }
    return 0;
}
