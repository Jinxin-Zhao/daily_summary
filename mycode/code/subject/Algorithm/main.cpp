#include <iostream>
#include "Fibonacci.h"

int main() {
    //test Fibonacci
    Fibonacci fibonacci;
    int res_n = fibonacci.getFibnacci_iteraton(19);
    std::cout<<"f(n): "<<res_n<<std::endl;

    //test matrix method
    int res = fibonacci.getFibnacci_matrix(6);
    std::cout<<"f(n): "<<res<<std::endl;
    return 0;
}