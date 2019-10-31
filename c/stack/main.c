#include "stack.h"
int main()
{

stack_t* stack1 = StackCreate(3, 4);
int a = 10;
int b = 20;
int *ptr;
StackPush(stack1, &a);

StackPush(stack1, &b);

StackPop(stack1);
ptr = (int*)StackPeek(stack1);
printf("%d",*ptr);


printf("\nis it empty? %d",StackIsEmpty(stack1));

printf("\nstacksize %lu",StackSize(stack1));
StackPop(stack1);
StackDestroy(stack1);

return 0;
}


