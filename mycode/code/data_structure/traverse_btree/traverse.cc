#include <iostream>
#include <vector>
#include <stack>
using namespace std;

struct TreeNode
{
    int val;
    struct TreeNode * left;
    struct TreeNode * right;
};

void create(TreeNode ** root,int n)
{
    *root = new TreeNode();
    (*root)->val = 1;

}

vector<int> preorder(TreeNode * root)
{
    stack<TreeNode *> tStack;
    vector<int> result;
    while(tStack.size() > 0 || root != NULL);
    {
        if(root != NULL)
        {

        }
        else
        {
            root = tStack.top();
            tStack.pop();
        }
    }
}


int main()
{
    cout << "start to traverse tree" << endl;
    return 0;
}
