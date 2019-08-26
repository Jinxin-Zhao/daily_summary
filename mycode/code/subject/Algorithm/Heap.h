#ifndef _H_HEAP_
#define _H_HEAP_

#include <vector>

//我们看这棵二叉树，它必须保证每一层填满之后才能去填充下一层。而且，如果从根结点开始计数，往下第i层的元素如果不是最后一层的话，
//这一层的元素数量为2**i(2的i次方)。这样，对于一棵高为h的二叉树，它的所有结点数目就等于前面完全填满的层元素加上最下面一层的元素。
//为什么要把他们分开来计数呢？是因为最下面一层的元素有一个变动的范围，作为一棵高度为h的树，最下面一层的元素最少可以是1，
//最大可以是把整层填充满，也就是2**(h+1)。这样，他们求和的结果就是最少为2**h，最大为2**(h+1)。
//所以假设堆的元素数量为n的话，我们就可以推出：
//2^h <= n < 2^(h+1) - 1 < 2^(h+1)
//结合这一步分析，我们可以得到: h <= lgn < h + 1.

//conclusion 1:
//我们可以发现一个n个元素的树，它的高度相当于logn(向下取整).

//我们再来看我们分析的第二个结论。对应树每一个高度的一层，该有多少个元素呢？
//假设高度为1的那一层他们的元素个数为k，那么他们的访问时间复杂度为O(k).
//根据前面的分析，我们还发现一个情况，就是如果从根结点开始计数，往下第i层的元素如果不是最后一层的话，这一层的元素数量为2**i(2的i次方).
//正好这个第i层就相当于树的总高度减去当前层的结点的高度。假设第i层的高度为h，那么也就是i = lgn - h.
//这一层的元素数量为: 2^(height - hi - 1) = 2^(lgn - hi - 1) = n/2^(hi + 1).

//

class MaxHeap{
public:
    MaxHeap(int * ar,int len);
    ~MaxHeap();

    int parent(int index);
    int lchild(int index);
    int rchild(int index);

    //from the first node which is not leaf
    void maxheapify(int * a,int index,int len);
    void buildmaxheap(int * a,int len); //O(n)
    void printheap(int *a,int len);
    void swap(int * a,int * b);

    void dynamicbuildmaxheap(int key); //O(nlgn)
    void heapincresekey(int index,int key);
    void printdyheap();

private:
    int *m_array;
    int m_len;
    std::vector<int>  m_vec;
    int m_heapsize;
};

#endif
