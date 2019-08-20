#include <iostream>
#include "Fibonacci.h"
#include "Quickselect.h"
#include "LinkList.h"
#include "LinklistStack.h"
#include "LinklistQueue.h"

int main() {
    //test Fibonacci
    Fibonacci fibonacci;
    int res_n = fibonacci.getFibnacci_iteraton(19);
    std::cout<<"f(n): "<<res_n<<std::endl;

    //test matrix method
    int matrix_res = fibonacci.getFibnacci_matrix(14);
    std::cout<<"matrix result: "<<matrix_res<<std::endl;
    //
    //srand(unsigned(time(NULL)));
    //for (int j = 0; j < num_array; j++)
    //array[j] = rand();
    // 寻找第k最小数
    Quickselect q;
    int k = 13;
    int i = q.q_select(array, 0, num_array - 1, k);
    std::cout <<"kth num is : "<< i << std::endl;
    for(int j = 0; j < num_array; j++){
        std::cout<<array[j]<<" ";
    }

    ///////////
    std::cout<<std::endl;
    LinkList ll;
    Node *head=ll.creat(10);
    ll.print(head);
    /*O(1)策略删除一个给定元素*/
    Node *del = ll.locateNode(head,5);
    ll.deleteNode(head,del);
    ll.print(head);

    /*O(n)策略删除结尾元素*/
    Node *del2 = ll.locateNode(head,9);
    ll.deleteNode(head,del2);

    std::cout<<std::endl;
    ll.print(head);

    ////////stack by linklist
    StackByLinklist sta;
    sta.push(1);
    sta.push(2);
    sta.push(3);

    std::cout<<"stack len: "<<sta.length()<<std::endl;
    sta.pop();
    std::cout<<"stack top: "<<sta.top()<<std::endl;
    std::cout<<"stack len: "<<sta.length()<<std::endl;
    ///deqeue by linklist
    QueueByLinklist que;
    que.EnQueueFront(1);
    que.EnQueueFront(2);
    que.EnQueueFront(3);
    que.EnQueueFront(4);
    que.EnQueueBack(9);
    que.EnQueueBack(8);
    que.EnQueueBack(4);
    que.DeQueueFront();
    //
    que.DeQueueBack();
    std::cout<<"queue front: "<<que.Front()<<std::endl;
    std::cout<<"queue rear: "<<que.Back()<<std::endl;
    std::cout<<"queue len: "<<que.length()<<std::endl;


    return 0;
}
