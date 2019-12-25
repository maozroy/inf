#include <stdlib.h> /*malloc and free*/
#include <assert.h> /*asserting*/
#include "../../ds/stack/stack.h"

void CopyVoidToVoid(void *src, void *dst, size_t n);

struct stack
{
    size_t elements_size;
    char *head;
    void *tail;
    void *current;
};

stack_t *StackCreate(size_t num_of_elements, size_t elements_size)
{
	stack_t * stack = malloc(sizeof(stack_t));
	if (NULL == stack)
	{
		return 0;
	}

	assert(num_of_elements || elements_size);
	
	stack->head = malloc(num_of_elements*elements_size);
	if (NULL == stack->head)
	{
		return 0;
	}

	stack->elements_size = elements_size;
	stack->tail = (char*)(stack->head) + (num_of_elements * elements_size);
	stack->current = stack->head;
	
	return stack;
}

void StackDestroy(stack_t *stack)
{
	assert(stack);
		
	free(stack->head);
	free(stack);
	
}

int StackPush(stack_t *stack, const void *n)
{
	
	assert(stack);
	assert(n);
	
	if (stack->current == stack->tail)
	{
		return 1;
	}

	CopyVoidToVoid( (char*)n ,stack->current, stack->elements_size);
	stack->current = (char*)stack->current + stack->elements_size;

	return 0;
}

void CopyVoidToVoid(void *src, void *dst, size_t n)
{
	size_t i = 0;
	
	assert(src);
	assert(dst);
	
	for (i = 0 ; n > i ; i++)
	{
		*(char*) dst = *(char*) src;
		dst = ((char*)dst) + 1;
		src = ((char*)src) + 1;
	}
}

void *StackPeek(const stack_t *stack)
{
	assert(stack);
	
	
	return (((char*) stack->current) - stack->elements_size);
}

void StackPop(stack_t *stack)
{
	assert(stack);
	
	stack->current = (((char*) stack->current) - stack->elements_size);
}

int StackIsEmpty(const stack_t *stack)
{
	assert(stack);

	if (stack->current ==  stack->head)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

size_t StackSize(const stack_t *stack)
{
	assert(stack);
	
	return ((size_t)(((char*)stack->current) - ((char*)stack->head))) / stack->elements_size;
}
