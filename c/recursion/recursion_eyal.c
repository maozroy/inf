/********************************************
* OL79 - 17/12/19							*
* ursion									* 
* Author: Eyal Rabinovitz					*
*********************************************/

#include <stddef.h>  /* size_t */
#include <assert.h>  /* assert */ 
#include <string.h>  /* strlen */
#include <stdlib.h>  /* For malloc	*/

#include "recursion.h"

void StackSwapIMP(stack_t *stack, void *element);

unsigned long RecursiveFibonacci(unsigned int n)
{
	if((0 == n) || (1 == n))
	{
		return n;
	}
	
	return (RecursiveFibonacci(n - 1) + RecursiveFibonacci(n - 2));
}

size_t Strlen(const char *str)
{
	assert(NULL != str);

	if('\0' == *str)
	{
		return 0;
	}

	return(Strlen(str + 1) + 1);
}

int Strcmp(const char *str1, const char *str2)
{
	assert(NULL != str1);
	assert(NULL != str2);

	if((*str1 != *str2) || ('\0' == *str1) || ('\0' == *str2))
	{
		return (*str1 - *str2);
	}

	return Strcmp(str1 + 1, str2 + 1);
}

char *Strcpy(char *dest, const char *src)
{
	if('\0' != *src)
	{
		*dest = *src;

		Strcpy(dest + 1, src + 1);
	}

	return dest;
}

char *Strcat(char *dest, const char *src)
{
	if('\0' != *dest)
	{
		Strcat(dest + 1, src);
	}
	else if('\0' != *src)
	{
		*dest = *src;
		*(dest + 1) = '\0';
		Strcat(dest + 1, src + 1);
	}

	return dest;
}

char *Strstr(const char *haystack, const char *needle)
{
	if('\0' == *needle)
	{
		return (char *)haystack;
	}
	if('\0' == *haystack)
	{
		return NULL;
	}

    if((*haystack == *needle) && 
    	 (Strstr(haystack + 1, needle + 1) == haystack + 1))
    {
    	return (char *)haystack;
    }

    return Strstr(++haystack, needle);
}


sl_node_t *Flip(sl_node_t *head)
{
	sl_node_t *node = NULL;

	if(NULL == head -> next || NULL == head)
	{
		return head;
	}

	node = Flip(head -> next);

	head -> next -> next = head;
	head -> next = NULL;

	return node;
}

stack_t *SortStack(stack_t *stack,compare_func func, size_t element_size)
{
	void *element = NULL;

	assert(NULL != stack);

	element = StackPeek(stack);
	StackPop(stack);
	
	if(StackSize(stack)/element_size > 1)
	{
		SortStack(stack, func, element_size);
	}

	if(1 == func(StackPeek(stack), element))
	{
		StackPush(stack, element);
	}
	else if(-1 == func(StackPeek(stack), element))
	{
		StackSwapIMP(stack, element);
		SortStack(stack, func, element_size);
	}

	return stack;
}


void StackSwapIMP(stack_t *stack, void *element)
{
	void *stack_element = malloc(sizeof(StackPeek(stack)));
	stack_element = memcpy(stack_element, StackPeek(stack), sizeof(StackPeek(stack)));
	StackPop(stack);

	StackPush(stack, element);
	StackPush(stack, stack_element);
}





