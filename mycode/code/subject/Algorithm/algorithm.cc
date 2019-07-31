#include "Fibonacci.h"

Fibonacci::Fibonacci(){
    //m_array = (int**)malloc(2*sizeof(int *));
    //(*m_array) = (int *)malloc(2*sizeof(int));
    m_array[0] = new int[2];
    m_array[1] = new int[2];
    m_array[0][0] = 1;
    m_array[0][1] = 1;
    m_array[1][0] = 1;
    m_array[1][1] = 0;
    m_row = sizeof(m_array)/sizeof(int *);
    m_column = sizeof(*m_array)/sizeof(int);
}

int Fibonacci::getFibnacci_iteraton(int n){
    std::cout<<"Fibonacci queue: ";
    if(n <= 0){
        return 0;
    }
    if(n == 1 || n == 2){
        (n == 1) ? std::cout<<"1"<<std::endl : std::cout<<"1 1"<<std::endl;
        return 1;
    }
    int pre = 1;
    int res = 1;
    int temp = 0;
    std::cout<<"1 1 ";
    for(int i = 3 ;i <= n;i++){
        temp = res;
        res = pre + res;
        pre = temp;
        std::cout<<res<<" ";
    }
    std::cout<<std::endl;
    return res;
}

int Fibonacci::getFibnacci_matrix(int n){
    std::cout<<"Fibonacci queue: ";
    if(n <= 0){
        return 0;
    }
    if(n == 1 || n == 2){
        (n == 1) ? std::cout<<"1"<<std::endl : std::cout<<"1 1"<<std::endl;
        return 1;
    }
    int ** res = getMatrixPower(m_array,n-2);
    return res[0][0] + res[1][0];
}

int ** Fibonacci::getMultiMatrix(int ** m1,int ** m2){

}
int ** Fibonacci::getMatrixPower(int ** matrix,int dimension){
    int  res[m_row][m_column];
    //init E matrix
    for(int i = 0;i < m_row;i++)
        for(int j = 0;j < m_column; j++){
            if(i == j){
                res[i][j] = 1;
            }
        }
    int *p = res[0];
    //std::cout<<"res.size: "<<res.size()<<" "<<res[0].size()<<std::endl;
        return &p;
}