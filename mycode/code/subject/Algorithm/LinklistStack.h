#ifndef _H_LINKLISTSTACK_
#define _H_LINKLISTSTACK_

class StackByLinklist{
public:
    StackByLinklist(){m_head.next = &m_head;}
    ~StackByLinklist();

    //push elements from head
    void push(int elem);
    void pop();
    int top() const;
    bool empty() const;

    //
    //void destroystack(StackByLinklist & s);
    //void clearstack(StackByLinklist & s);
    //bool stackempty(StackByLinklist &s);
    int  length();

    //void traverse(StackByLinklist &s);

private:
    struct Node{
        int key;
        Node * next;
    };
    Node m_head;
};

#endif
