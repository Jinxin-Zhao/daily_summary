#include <iostream>
#include "Fibonacci.h"

int main() {
    //test Fibonacci
    Fibonacci fibonacci;
    int res_n = fibonacci.getFibnacci_iteraton(19);
    std::cout<<"f(n): "<<res_n<<std::endl;

    //test matrix method
    fibonacci.getFibnacci_matrix(4);
    return 0;
}