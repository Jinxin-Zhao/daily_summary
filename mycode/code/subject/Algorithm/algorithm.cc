#include "Fibonacci.h"

Fibonacci::Fibonacci(){
    m_item_array_f.push_back(1);
    m_item_array_f.push_back(1);
    m_array.push_back(m_item_array_f);
    m_item_array_s.push_back(1);
    m_item_array_s.push_back(0);
    m_array.push_back(m_item_array_s);
    m_row = m_array.size();
    m_column = m_item_array_f.size();
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
    R res = getMatrixPower(m_array,n-2);
    return res[0][0] + res[1][0];
}

R Fibonacci::getMultiMatrix(R m1,R m2){
    R res;
    Itemtype n1;
    for(int i = 0;i < m1.size();i++){
        for(int j = 0;j < m1.size();j++){
            int e1 = 0;
            for(int k = 0;k < m2.size();k++){
               e1 += m1[i][k] * m2[k][j];
            }
            n1.push_back(e1);
        }
    }
    int s = n1.size();
    Itemtype n2(n1.begin(),n1.end() - 3);
    res.push_back(n1);
    res.push_back(n2);
    int ss = res.size();
    return res;
}
R Fibonacci::getMatrixPower(R & matrix,int stage){
    R res;
    //init E matrix
    for(int i = 0;i < m_row;i++){
        Itemtype item;
        for(int j = 0;j < m_column; j++){
            if(i == j){
                item.push_back(1);
            }
            item.push_back(0);
        }
        res.push_back(item);
    }
    R temp = matrix;
    std::cout<<temp[0][0]<<" "<<temp[0][1]<<" "<<temp[1][0]<<" "<<temp[1][1]<<std::endl;
    for(;stage != 0; stage >>= 1){
        if( (stage & 1) == 1){
            res = getMultiMatrix(res,temp);
        }
        temp = getMultiMatrix(temp,temp);
    }
    return res;
}
