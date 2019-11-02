/************************************************
 *                                              *
 *              OL-79 - DS - Stack              *
 *                                              *
 *    Author: Dvir             Reviewer: Maoz   *
 *                   31/10/19                   *
 ************************************************/

#include <stdio.h>      /* For size_t  */ 
#include <stdlib.h>     /* for malloc  */
#include <string.h>     /* for memcpy  */
#include <assert.h>     /* For assert  */
#include "stack.h"

#define MALLOC_CHECK(mptr) if ( NULL == (mptr) ) \
	{ \
		printf("Not enough memory, aborting program\n"); \
		exit(0); \
	} \

struct stack_t
{
    size_t elements_size;
    char *head;
    void *tail;
    void *current;
};

stack_t *StackCreate(size_t num_of_elements, size_t elements_size)
{
	stack_t *new_stack = (stack_t *)malloc(sizeof(stack_t));
	MALLOC_CHECK(new_stack);
	new_stack->elements_size = elements_size;
	new_stack->head = (char *)malloc(num_of_elements * elements_size);	
	MALLOC_CHECK(new_stack->head);
	new_stack->current = new_stack->head;
	new_stack->tail = (new_stack->head)+(num_of_elements * elements_size);
	
	return new_stack;
}

void StackDestroy(stack_t *stack)
{
	assert(NULL != stack);
	
	free(stack->head);
	free(stack);
}

int StackPush(stack_t *stack, const void *n)
{
	char *new_p = NULL;
	
	assert(NULL != stack);
	assert(NULL != n);
	
	if (stack->tail > stack->current)
	{
		stack->current = memcpy(stack->current, n, stack->elements_size);
		new_p = (char *)stack->current + stack->elements_size;
		stack->current = (void *)new_p;
		return(0);
	}
	else
	{
		printf("Error: stack is full!\n");
		return(1);
	}
}

void StackPop(stack_t *stack)
{
	assert(NULL != stack);
	
	stack->current = (void *)((size_t)stack->current - stack->elements_size);
}

void *StackPeek(const stack_t *stack)
{
	assert(NULL != stack);
	
	return ((void *)((size_t)stack->current - stack->elements_size));
}

int StackIsEmpty(const stack_t *stack)
{
	assert(NULL != stack);
	
	return (stack->current == stack->head);
}

size_t StackSize(const stack_t *stack)
{
	assert(NULL != stack);
	
	return ((char *)stack->current - stack->head) / stack->elements_size;
}
