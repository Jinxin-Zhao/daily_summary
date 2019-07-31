//Fibonacci

#ifndef _H_FIBONACCI_
#define _H_FIBONACCI_

#include <iostream>
#include <vector>

typedef std::vector<std::vector<int>>  SArray;

class Fibonacci{
public:
    Fibonacci();
    ~Fibonacci() {}
public:
    //solution 1: iteration method  O(N)
    int getFibnacci_iteraton(int n);


    //solution2: matrix method   O(logN)
    //
    //(f(3),f(2)) = (f(2),f(1)) * [1,1] = [1,1] * [1,1]
    //                            [1,0]           [1,0]
    // ......
    //
    //(f(n),f(n-1)) = (f(n-1),f(n-2)) * [1,1] = [1,1] * [1,1] ^ (n-2)
    //                                  [1,0]           [1,0]

    //(f(n),f(n-1)) = E * [1,1] = [1,0] * [1,1] ^ (n-2) = a[0][0] + a[1][0];
    //                            [0,1]   [1,0]
    //
    int getFibnacci_matrix(int n);

private:
    int ** getMultiMatrix(int ** m1,int ** m2);
    int ** getMatrixPower(int ** matrix,int dimension);

private:
    int *  m_array[2];
    int    m_row;
    int    m_column;
};

#endif