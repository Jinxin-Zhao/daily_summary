#ifndef _H_QUICKSELECT_
#define _H_QUICKSELECT_

inline int array[]={0,45,78,55,47,4,1,2,7,8,96,36,45};
inline const int num_array = sizeof(array) / sizeof(int);
inline const int num_med_array = num_array / 5 + 1;
inline int midian_array[num_med_array];

class Quickselect{
public:
    Quickselect(){};
    ~Quickselect(){};

    int q_select(int array[], int left, int right, int k);
private:
    void swap(int &a,int &b);
    void insert_sort(int array[], int left, int loop_times);
    int find_median(int array[], int left, int right);
    int find_index(int array[], int left, int right, int median);
};

#endif
