#include "Fibonacci.h"
#include "Quickselect.h"
#include "LinkList.h"
#include "LinklistStack.h"
#include "LinklistQueue.h"
#include "Heap.h"

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

///private function
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
    Itemtype n2(n1.begin()+2,n1.end());
    n1.pop_back();
    n1.pop_back();
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
            }else{
                item.push_back(0);
            }
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

/******************************QuickSelect Algorithm O(n)***************************/
/*
//插入排序算法伪代码
INSERTION-SORT(A)                              cost    times
1  for j ← 2 to length[A]num_med_array                      c1      n
2       do key ← A[j]                          c2      n - 1
3          Insert A[j] into the sorted sequence A[1 ‥ j - 1].     0...n - 1
4          i ← j - 1                           c4      n - 1
5          while i > 0 and A[i] > key           c5
6             do A[i + 1] ← A[i]               c6
7             i ← i - 1                        c7
8          A[i + 1] ← key                      c8      n - 1
*/
//已修正为插入排序，如下：
void Quickselect::insert_sort(int array[], int left, int loop_times)
{
    for (int j = left; j < left+loop_times; j++)
    {
        int key = array[j];
        int i = j-1;
        while ( (i > (left - 1)) && array[i]>key )
        {
            array[i+1] = array[i];
            i--;
        }
        array[i+1] = key;
    }
}

void Quickselect::swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

int Quickselect::find_median(int array[], int left, int right)
{
    if (left == right)
        return array[left];

    if(right < 6){
        insert_sort(array, 0, right + 1);
        return array[(right+1)/2];
    }

    int index = 0;
    for (index = left; index < right - 3; index += 5)
    {
        insert_sort(array, index, 5);
        int num = index - left;
        midian_array[num / 5] = array[index + 2];
    }

    // 处理剩余元素
    int remain_num = right - index + 1;
    if (remain_num > 0)
    {
        insert_sort(array, index, remain_num);
        int num = index - left;
        midian_array[num / 5] = array[index + remain_num / 2];
    }

    //number of [5] subarray
    int elem_aux_array = (right - left + 1) / 5;
    if ((right - left + 1) % 5 != 0)
        elem_aux_array++;

    //now elem_aux_array denotes the number of all the subarray: [5] .... [5], [n]
    // 如果剩余一个元素返回，否则继续递归
    if (elem_aux_array == 0)
        return midian_array[0];
    else
        return find_median(midian_array, 0, elem_aux_array - 1);
}

// 寻找中位数的所在位置
int Quickselect::find_index(int array[], int left, int right, int median)
{
    for (int i = left; i <= right; i++)
    {
        if (array[i] == median)
            return i;
    }
    return -1;
}

int Quickselect::q_select(int array[], int left, int right, int k)
{
    // 寻找中位数的中位数
    int median = find_median(array, left, right);

    // 将中位数的中位数与最右元素交换
    int index = find_index(array, left, right, median);
    swap(array[index], array[right]);

    int pivot = array[right];

    // 申请两个移动指针并初始化
    int i = left;
    int j = right - 1;

    // 根据枢纽元素的值对数组进行一次划分
    while (true)
    {
        while(array[i] < pivot)
            i++;
        while(array[j] > pivot)
            j--;
        if (i < j)
            swap(array[i], array[j]);
        else
            break;
    }
    swap(array[i], array[right]);

    /* 对三种情况进行处理：(m = i - left + 1)
    1、如果m=k，即返回的主元即为我们要找的第k小的元素，那么直接返回主元a[i]即可;
    2、如果m>k，那么接下来要到低区间A[0....m-1]中寻找，丢掉高区间;
    3、如果m<k，那么接下来要到高区间A[m+1...n-1]中寻找，丢掉低区间。
    */
    int m = i - left + 1;
    if (m == k)
        return array[i];
    else if(m > k)
        //上条语句相当于if( (i-left+1) >k)，即if( (i-left) > k-1 )，于此就与2.2节里的代码实现一、二相对应起来了。
        return q_select(array, left, i - 1, k);
    else
        return q_select(array, i + 1, right, k - m);
}

/******************************Linklist Algorithm: delete specific node in O(1)***************************/

Node *LinkList::creat(int num)
{
    Node *head=new Node();
    head->data=0;
    head->next=nullptr;
    for(int i=num;i>0;--i)
    {
        Node *temp=new Node();
        temp->data=i;
        temp->next=nullptr;
        temp->next=head->next;
        head->next=temp;
    }
    return head;
}
Node *LinkList::findLastPreNode(Node *head) {
    while (head->next->next) {
        head = head->next;
    }
    return head;

}
Node * LinkList::locateNode(Node * head,int k){
    Node * phead = head;
    while(k--){
        phead = phead->next;
    }
    return phead;
}
void LinkList::deleteNode(Node *head,Node *del)
{
    if(del->next!=nullptr)
    {
        //normal scenario costs: O(1)
        Node *p = del->next;
        del->data = del->next->data;
        del->next = del->next->next;
        delete p;
    }
    else
    {
        //the node to be deleted is the last node,this scenario costs: O(n)
        Node *pre = findLastPreNode(head);
        delete pre->next;
        pre->next=nullptr;
    }

    //average costs: ( O(1) * (n-1) + O(n) * 1 ) / n = O(1)
}
void LinkList::print(Node *head)
{
    while(head != nullptr)
    {
        std::cout<<head->data<<" ";
        head=head->next;
    }
    std::cout<<std::endl;
}

/******************************StackByLinklist Algorithm: push/pop specific node in O(1)***************************/
StackByLinklist::~StackByLinklist(){
    Node * pCurrent = m_head.next;
    Node * pNext = nullptr;
    if(pCurrent != &m_head){
        pNext = pCurrent->next;
        delete pCurrent;
        pCurrent = pNext;
    }
}
void StackByLinklist::push(int elem){
    Node * pNew = new Node();
    pNew->key = elem;
    pNew->next = m_head.next;
    m_head.next = pNew;
}

void StackByLinklist::pop() {
    if(empty()){
        std::cout<<"empty stack"<<std::endl;
        return;
    }
    Node *del = m_head.next;
    m_head.next = del->next;
    delete del;
}

int StackByLinklist::top() const{
    if(empty()){
        std::cout<<"empty list"<<std::endl;
        return -1;
    }
    return m_head.next->key;
}

int  StackByLinklist::length(){
    if(m_head.next == &m_head){
        return 0;
    }
    int count = 0;
    Node * p = m_head.next;
    while(p != &m_head){
        count++;
        p = p->next;
    }
    return count;
}
bool StackByLinklist::empty() const{
    if(m_head.next == &m_head){
        return true;
    }
    return false;
}


/******************************QueueByLinklist Algorithm: push/pop specific node from two sides in O(1)***************************/
QueueByLinklist::QueueByLinklist(){
    m_head.next = &m_head;
    m_front = m_rear = &m_head;
}
QueueByLinklist::~QueueByLinklist(){
    Node *pCur = m_head.next;
    Node *pNext = nullptr;
    if(pCur != &m_head){
        pNext = pCur->next;
        delete pCur;
        pCur = pNext;
    }
    m_rear = &m_head;
}

void QueueByLinklist::EnQueueBack(int elem){
    Node * pNew = new Node();
    pNew->key = elem;
    m_rear->next = pNew;
    pNew->pre = m_rear;
    pNew->next = m_front;
    m_rear = pNew;
}
int QueueByLinklist::DeQueueFront(){
    if(empty()){
        std::cout<<"empty queue"<<std::endl;
        return -1;
    }
    Node * del = m_front->next;
    m_front->next = del->next;
    del->next->pre = m_front;
    //critical
    if(del == m_rear){
        m_rear = m_front;
    }
    delete del;
}
//
void QueueByLinklist::EnQueueFront(int elem){
    Node * pNew = new Node();
    pNew->key = elem;
    Node * Next = m_front->next;
    if(Next == &m_head){
        m_rear = pNew;
        m_front->next = pNew;
        m_rear->pre = m_front;
        m_rear->next = m_front;
        return;
    }
    Next->pre  = pNew;
    pNew->next = Next;
    m_front->next = pNew;
    pNew->pre = m_front;
}
int QueueByLinklist::DeQueueBack(){
    if(empty()){
        std::cout<<"empty queue"<<std::endl;
        return -1;
    }
    Node * del = m_rear;
    Node * pNewrear = m_rear->pre;
    pNewrear->next = m_front;
    if(m_front == pNewrear){
        m_rear = m_front;
    }
    delete del;
    m_rear = pNewrear;
}
int QueueByLinklist::Front() const{
    if(empty()){
        std::cout<<"empty queue"<<std::endl;
        return -1;
    }
    return m_front->next->key;
}
int QueueByLinklist::Back() const{
    if(m_front == m_rear){
        std::cout<<"empty queue"<<std::endl;
        return -1;
    }
    return m_rear->key;
}

int QueueByLinklist::length(){
    if(empty()){
        std::cout<<"empty queue"<<std::endl;
        return -1;
    }
    int count = 0;
    Node * p = m_front->next;
    while(p != &m_head){
        count++;
        p = p->next;
    }
    return count;
}

bool QueueByLinklist::empty() const{
    if(m_front == m_rear){
        return true;
    }
    return false;
}

/******************************Heap Algorithm: optimized insertion***************************/
MaxHeap::MaxHeap(int * ar,int len){
    m_array = new int[len];
    for(int i = 0; i < len; ++i){
        m_array[i] = ar[i];
    }
}
MaxHeap::~MaxHeap(){
    delete [] m_array;
}

int MaxHeap::parent(int index){
    return m_array[index/2];
}
int MaxHeap::lchild(int index){
    return m_array[index*2+1];
}
int MaxHeap::rchild(int index){
    return m_array[index*2+2];
}

//from the first node which is not leaf
void MaxHeap::maxheapify(int * a,int index,int len){
    int lindex = 2*index + 1;
    int rindex = 2*index + 2;
    int largest = index;
    while(true){
        if(lindex < len && m_array[lindex] > m_array[index]){
            largest = lindex;
        }
        if(rindex < len && m_array[rindex] > m_array[largest]){
            largest = rindex;
        }
        if(index != largest){
            swap(&m_array[index],&m_array[largest]);
        }else{
            break;
        }
        index = largest;
        lindex = 2 * index + 1;
        rindex = 2 * index + 2;
    }
}
void MaxHeap::buildmaxheap(int * a,int len){
    for(int i = len/2; i >= 0; i--){
        maxheapify(a,i,len);
    }
    std::cout<<"max heap is : ";
    printheap(m_array,len);
}

void MaxHeap::printheap(int *a,int len){
    for(int i = 0; i < len; ++i){
        std::cout<<m_array[i]<<" ";
    }
    std::cout<<std::endl;
}

void MaxHeap::swap(int * a,int * b){
    int temp = *a;
    *a = *b;
    *b = temp;
}
