#include <iostream>
#include <string>
#include <vector>
//classic dynamic pattern
// Q1: return longest common substring
// two solution: 1 time O(M*N),space O(M*N);   2 time O(M*N),space O(1)
// Example: str1: 1AB2345CD, str2: 12345EF
// substring: 2345


// Q2: return longest common subvector
// Example: str1: 1A2C3D4B56, str2: B1D23CA45B6A
// subvector: 123456 or 12C4B6

typedef std::vector<int>  IntVector;
typedef std::vector<IntVector>    IntVector2D;

const int height = 500;
const int width  = 500;

/******************* solution 1 ***************/
//space O(n) = O(M*N)
IntVector2D * getdp(std::string & str1,std::string & str2){
    IntVector2D * pArray = new IntVector2D;

    pArray->resize(height);
    for(int i=0; i<height; ++i)
    {
        (*pArray)[i].resize(width);
    }
    //initialize the first column
    for(int i = 0; i < str1.size(); ++i){
        if(str1[i] == str2[0]){
            (*pArray)[i][0] = 1;
        }else{
            (*pArray)[i][0] = 0;
        }
    }

    //initialize the first row
    for(int i = 0; i < str2.size(); ++i){
        if(str2[i] == str1[0]){
            (*pArray)[0][i] = 1;
        }else{
            (*pArray)[0][i] = 0;
        }
    }

    for(int i = 1; i < str1.size(); ++i){
        for(int j = 1; j < str2.size(); ++j){
            if(str1[i] == str2[j]){
                (*pArray)[i][j] = (*pArray)[i-1][j-1] + 1;
            }
        }
    }
    return pArray;
}

std::string lcstl(std::string & str1,std::string & str2){
    IntVector2D * pArray = getdp(str1,str2);
    int end = 0,max = 0;

    for(int i = 1; i < str1.size(); ++i) {
        for (int j = 1; j < str2.size(); ++j) {
            if ((*pArray)[i][j] > max) {
                end = i;
                max = (*pArray)[i][j];
            }
        }
    }
    std::cout<<"max: "<<max<<std::endl;
        return str1.substr(end - max + 1,max);
}

/******************* solution 2 ***************/
//space O(n) = O(1)
std::string lcst2(std::string & str1,std::string & str2){
    //traverse diagonal from north-east to south-west
    int row = 0;  //beginning diagonal row
    int col = str2.size() -1;
    int max = 0,end = 0;
    while(row < str1.size()){
        int i = row;
        int j = col;
        int len = 0;
        while (i < str1.size() && j < str2.size()){
            if(str1[i] != str2[j]){
                len = 0;
            }else{
                len++;
            }

            if(len > max){
                end = i;
                max = len;
            }
            i++;
            j++;
        }
        if(col > 0){
            //column decrease from str2.size()-1 to 0
            col--;
        }else{
            row++;
        }
    }
    return str1.substr(end - max + 1,max);
}

int main() {
    std::cout << "initilize two string " << std::endl;

    std::string str1 = "1AB2345CD";
    std::string str2 = "12345EF";

    //std::string result = lcstl(str1,str2);

    std::string result = lcst2(str1,str2);
    std::cout<<"substring : "<<result.c_str()<<std::endl;
    return 0;
}



/*
 //three different ways to create a planar array
 ////////
void TestFunc_(unsigned int n)
{
    unsigned int i, j;

    // 元素类型为‘int[5]’的数组，可赋值给‘int[5]’类型的指针.
    int (* array2D)[5] = new int[n][5];
    for(i=0; i<n; ++i)
    {
        for(j=0; j<5; ++j)
        {
            array2D[i][j] = i * 5 + j;
        }
    }

    // 回收方法和普通动态数组相同，使用'delete[]'即可
    delete[] array2D;
}


 //////////
 void TestFunc_pointer(unsigned int height, unsigned int width)
{
    unsigned int i, j;

    //  数组的元素类型为‘int *’，可赋值给指向‘int *’的指针.
    int **array2D = new int *[height];
    for(i=0; i<height; ++i)
    {
        array2D[i] = new int[width];
    }

    // 访问.
    for(i=0; i<height; ++i)
    {
        for(j=0; j<width; ++j)
        {
            // 内存非连续，注意防止越界.
            array2D[i][j] = i * width + j;
        }
    }

    // 首先回收低一级的动态数组.
    for(i=0; i<height; ++i)
    {
        delete[] array2D[i];
    }
    // 然后回收高一级的动态数组.
    delete[] array2D;
}



 ////////
 void TestFunc_vector(unsigned int height, unsigned int width)
{
    typedef std::vector<int>  IntVector;
    typedef std::vector<IntVector>    IntVector2D;
    unsigned int i, j;

    IntVector2D *pArray2D = new IntVector2D;

    // 动态设置大小.
    pArray2D->resize(height);
    for(i=0; i<height; ++i)
    {
        (*pArray2D)[i].resize(width);
    }

    for(i=0; i<height; ++i)
    {
        for(j=0; j<width; ++j)
        {
            (*pArray2D)[i][j] = i * width + j;
        }
    }

    delete pArray2D;
}


 */