#include <iostream>
#include <stack>
//reverse a stack only by recursive way
//difficulty: **

using Stack = std::stack<int> ;

int getAndRemoveLastElement(Stack & stack){
    int result = stack.top();
    stack.pop();
    if(stack.empty()){
        return result;
    }else{
        int last = getAndRemoveLastElement(stack);
        stack.push(result);
        return last;
    }
}

void reverse(Stack & stack){
    if(stack.empty()){
        return;
    }
    int i = getAndRemoveLastElement(stack);
    reverse(stack);
    stack.push(i);
}

int main() {
    std::cout << "initialize a stack..." << std::endl;
    Stack  s;
    s.push(3);
    s.push(2);
    s.push(1);

    reverse(s);

    while(!s.empty()){
        std::cout<<s.top()<<" ";
        s.pop();
    }
    return 0;
}