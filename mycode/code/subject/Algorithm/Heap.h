#ifndef _H_HEAP_
#define _H_HEAP_

class MaxHeap{
public:
    MaxHeap(int * ar,int len);
    ~MaxHeap();

    int parent(int index);
    int lchild(int index);
    int rchild(int index);

    //from the first node which is not leaf
    void maxheapify(int * a,int index,int len);
    void buildmaxheap(int * a,int len);

    void printheap(int *a,int len);
    void swap(int * a,int * b);

private:
    int *m_array;
    int m_len;
};

#endif
