#include <iostream>
#include <string>

class Node{
public:
    Node(int data){
        value = data;
    }
public:
    int value;
    Node * next;
};

class List{
public:
    Node * create(int n);

    Node * reverse(Node * phead);

    void print(Node * head);
public:
    Node * plist;
};

Node * List::create(int n) {
    Node * phead = new Node(0);
    int m = 1;
    Node * tmp = phead;
    while (n){
        Node * p = new Node(m);
        m += 2;
        tmp->next = p;
        tmp = p;
        n--;
    }
    tmp->next = nullptr;
    return phead;
}

Node * List::reverse(Node *phead) {
    Node * pre = nullptr;
    Node * next = nullptr;
    while(phead != nullptr){
        next = phead->next;
        phead->next = pre;
        pre = phead;
        phead = next;
    }
    return pre;
}

void List::print(Node *head) {
    std::cout<<head->value<<" ";
    Node * tmp = head->next;
    while(tmp != nullptr){
        std::cout<<tmp->value<<" ";
        tmp = tmp->next;
    }
    std::cout<<std::endl;
}


int main() {
    std::cout << "list operation" << std::endl;
    List l;
    Node * h = l.create(6);
    l.print(h);
    std::cout<<"after reverse...\n";
    Node * rev = l.reverse(h);
    l.print(rev);

    return 0;
}