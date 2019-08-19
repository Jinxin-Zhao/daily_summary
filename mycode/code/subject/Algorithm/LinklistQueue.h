#ifndef _H_LINKLISTQUEUE_
#define _H_LINKLISTQUEUE_

class QueueByLinklist{
public:
    QueueByLinklist();
    ~QueueByLinklist();

    void EnQueueBack(int elem);
    int DeQueueFront();
    void EnQueueFront(int elem);
    int DeQueueBack();
    int Front() const;
    int Back() const;
    int queuelength();

private:
    struct Node{
        int key;
        Node * pre;
        Node * next;
    };
    Node   m_head;
    Node * m_front;
    Node * m_rear;
};

#endif
