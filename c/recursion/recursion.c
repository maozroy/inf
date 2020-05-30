 /********************************************
*	Author : 			
*	Reviewer : 	
*	recursion					
*	Date: Sun Dec 22 11:49:14 IST 2019		                
*																				
********************************************/

#include <stdlib.h> /*MALLOCING*/
#include <assert.h> /*asserting*/
#include <string.h>

#include "recursion.h"

static stack_t *SortStackInsertIMP(stack_t *stack,cmp_func_t func, 
									size_t element_size, void *element1);

unsigned long IterativeFibonacci(unsigned int n)
{
	size_t odd = 1;
	size_t even = 0;
	size_t i = 0;
	
	for (i = 0 ; i < (n/2) ; i++)
	{
		odd +=even;
		even +=odd;
	}
	
	if ((n % 2) == 0)
	{
		return (even);
	}

	return (odd + even);
}

unsigned long RecursiveFibonacci(unsigned int n)
{
	if (n <= 1)
	{
		return (n);
	}
	
	return (RecursiveFibonacci(n - 1) + RecursiveFibonacci(n - 2));
}

size_t RecStrlen(const char *str)
{
	assert(str);

	if (*str == '\0')
	{
		return (0);
	}
	
	return (RecStrlen(str + 1) + 1); 
}

int RecStrcmp(const char *str1, const char *str2)
{
	assert(str1);
	assert(str2);

	if ((*str1 == '\0') && (*str2 == '\0'))
	{
		return (0);
	}
	if (*str1 != *str2)
	{
		return (*str1 - *str2);
	}
	
	return (RecStrcmp(str1 + 1, str2 + 1));
}

char *RecStrcpy(char *dest, const char *src)
{
	assert(dest);
	assert(src);
	
	if (*src == '\0')
	{
		*dest = *src;
		return (dest);
	}
	
	RecStrcpy(dest + 1, src + 1);
	*dest = *src;
	
	return (dest);	
}

char *RecStrcat(char *dest, const char *src)
{
	assert(dest);
	assert(src);

	*(dest + strlen(dest)) = *src;
	
	if ('\0' != *src)
	{
		return (RecStrcat(dest, src + 1));
	}
	
	return (dest);
}

char *RecStrstr(const char *haystack, const char *needle)
{
	assert(haystack);
	assert(needle);

	if (*needle == '\0')
	{
		return ((char *)haystack);
	}
	if (*haystack == '\0')
	{
		return NULL;
	}
	if (*haystack == *needle &&
		(RecStrstr(haystack + 1, needle + 1)) == (haystack + 1))
	{
		return ((char *)haystack);
	}
	
	return (RecStrstr(haystack + 1, needle));
}

sl_node_t *RecFlip(sl_node_t *head)
{
	sl_node_t *new_head = NULL;
	
	assert(head);
	
	if (head->next == NULL)
	{
		return (head);
	}
	
	new_head = RecFlip(head->next);
	head->next->next = head;
	head->next = NULL;

	return (new_head);
}

stack_t *RecSortStack(stack_t *stack, size_t element_size, cmp_func_t func)
{
	void *element1 = malloc(element_size);
	if(NULL == element1)
	{
		return NULL;
	}
	
	assert(stack);
	assert(func);
	assert(element_size);	

	memcpy(element1, StackPeek(stack), element_size);
	StackPop(stack);
	
	if (StackSize(stack) > 1)
	{
		RecSortStack(stack, element_size, func);
	}
	
	SortStackInsertIMP(stack, func, element_size, element1);
	free(element1);
	
	return (stack);
}

static stack_t *SortStackInsertIMP(stack_t *stack,cmp_func_t func, 
									size_t element_size, void *element1)
{
	int compare = 0;
	void *element2 = malloc(element_size);
	if(NULL == element2)
	{
		return NULL;
	}
	
	assert(stack);
	assert(func);
	assert(element_size);

	compare = func(element1, StackPeek(stack));
	if (compare > 0)
	{
		memcpy(element2, StackPeek(stack), element_size);
		StackPop(stack);
		StackPush(stack, element1);
		StackPush(stack, element2);
		RecSortStack(stack, element_size, func);
	}
	else
	{
		StackPush(stack, element1);
	}
	
	free(element2);
	
	return (stack);
}
