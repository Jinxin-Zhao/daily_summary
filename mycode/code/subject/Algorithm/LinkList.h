#ifndef _H_LINKLIST_
#define _H_LINKLIST_

struct Node
{
    int    data;
    Node*  next;
};

class LinkList{
public:
    LinkList(){}
    ~LinkList(){}

    Node * creat(int num);
    //locate kth node in LinkList
    Node * locateNode(Node * head,int k);
    //delete appointed node in O(1)
    void deleteNode(Node *head,Node *del);
    void print(Node *head);

private:
    Node *findLastPreNode(Node *head);

};

#endif
