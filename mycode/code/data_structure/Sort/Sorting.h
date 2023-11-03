#ifndef VARIETY_SORT_H
#define VARIETY_SORT_H
#include <iostream>

class Sorting{
public:
    int InsertSort(int * array,int len);

    void ShellSort(int * array,int len);
    void insertsort(int * array,int gap,int index);

    int SelectSort(int * array,int len);

    int QucikSort(int * array,int len);

    void MergeSort(int * array,int L,int R);

    void merge(int * array,int L,int mid,int R,int * temp);

    int RadixSort(int * array,int len);

    int q_select(int array[], int left, int right, int k);

private:
    int find_index(int array[], int left, int right, int median);
    int find_median(int array[], int left, int right);
    void insert_sort(int array[], int left, int loop_times);
public:
    void print(int * array,int len);

private:
    int * m_array;

    int num_med_array;
public:
    static const int DIVIDER;
};


#endif