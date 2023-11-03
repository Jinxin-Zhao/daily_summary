#ifndef _BINARY_TREE_H
#define _BINARY_TREE_H

#include <memory>

struct CBinaryNode{
    int m_value;
    CBinaryNode * left;
    CBinaryNode * right;

    bool IsNull(){
        return left == nullptr && right == nullptr && m_value == 0;
    }
    CBinaryNode() : m_value(0),left(nullptr),right(nullptr){}
    CBinaryNode(int data) : m_value(data){}

    bool operator!=(const CBinaryNode & rNode){
        return !(*this == rNode);
    }

    bool operator==(const CBinaryNode & rNode){
        return (this->m_value == rNode.m_value) && (this->left == rNode.left) && (this->right == rNode.right);
    }

    CBinaryNode & operator=(const CBinaryNode & rNode){
        if(this != &rNode){
            this->m_value = rNode.m_value;
            this->left = rNode.left;
            this->right = rNode.right;
        }
        return *this;
    }

};

class CBinaryTree{
public:
    void CreatTree(int num);

    void PrintEdge(CBinaryNode * head);

private:
    int getHeight(CBinaryNode * head, int l);

    int setEdgeMap(CBinaryNode * head,int l, std::unique_ptr<CBinaryNode[][2]> & edgeMap);

    void printLeafNotInMap(CBinaryNode * head, int l, std::unique_ptr<CBinaryNode [][2]> & pEdgeMap);

private:
    std::vector<int> m_vec;
};



#endif