#include "stack.h"
#include<stdlib.h>
#include<string.h>

struct stack_t
{
    size_t elements_size;
    char *head;
    void *tail;
    void *current;
};

stack_t *StackCreate(size_t num_of_elements, size_t elements_size)
{
	void *ptr = malloc(( num_of_elements*elements_size ));
	stack_t* stack = malloc(sizeof(stack_t));
	
	stack -> elements_size = elements_size;
	stack -> head = ptr;
	stack -> tail = (char*)(ptr)+(num_of_elements*elements_size);
	stack -> current = ptr;
	
return stack;
}

void StackDestroy(stack_t *stack)
{
	void* stack_to_kill = (*stack).head;
	
	free(stack_to_kill);
	free(stack);
}

int StackPush(stack_t *stack, const void *n)
{
	char* ch_ptr_src = (char*) n;
	char* ch_ptr_dst = (char*) stack->current;
	size_t i = 0;
	
	
	for(i = 0; (stack->elements_size) > i ; i++)
	{
		*ch_ptr_dst = *ch_ptr_src;
		++ch_ptr_dst;
		++ch_ptr_src;
	}				
																																																								
	stack->current = ch_ptr_dst;
	
	return 0;

}

void *StackPeek(const stack_t *stack)
{

return (((char*) stack->current) - stack->elements_size);

}


void StackPop(stack_t *stack)
{
	stack->current = (((char*) stack->current) - stack->elements_size);
}

int StackIsEmpty(const stack_t *stack)
{
	if(stack->current ==  stack->head)
	{
		return 1;
	}else
	{
		return 0;
	}
}

size_t StackSize(const stack_t *stack)
{
	return ((size_t)(((char*)stack->current) - ((char*)stack->head)))/stack->elements_size;
}












