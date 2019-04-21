#include "base_rule.h"


int main()
{
    A<B> * pa = new A<B>();
    pa->execute();

    test o_t;
    o_t.zhaojx();
    int s = sizeof(test);
    printf("size is %d\n",s);
    return 0;
}