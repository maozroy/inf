 /********************************************
*	Author : Maoz Roytman	
*	Reviewer : 	Gal Salemon
*	heapify					
*	Date: 29/12/2019	                
*																				
********************************************/

#include <stdlib.h> /*MALLOCING*/
#include <assert.h> /*asserting*/
#include <string.h> /* memcpy */
#include <alloca.h> /* alloca */

#include "heapify.h"

#define JUMP_TO_INDEX(i) (((char *)array) + ((i) * (elem_size)))

static size_t GetParentIMP(size_t index, size_t size);
static size_t GetRightIMP(size_t index, size_t size);
static size_t GetLeftIMP(size_t index, size_t size);
static void SwapIMP(void *val1, void *val2, size_t element_size);

void HeapifyUp(void *arr, 
				size_t arr_size, 
				size_t elem_size, 
			    size_t index_of_heapify, 
			    comparison_t func, 
			    void *compare_param)
{
	size_t parent_index = GetParentIMP(index_of_heapify, arr_size);
	char *array = arr;
	
	assert(arr);
	assert(func);
	assert(elem_size);
	
	if ((func(JUMP_TO_INDEX(parent_index), 
			  JUMP_TO_INDEX(index_of_heapify), compare_param) == 1) 
				||  (index_of_heapify == 0))
	{
		return;
	}
	
	SwapIMP(JUMP_TO_INDEX(index_of_heapify), 
			JUMP_TO_INDEX(parent_index),
			elem_size);
	HeapifyUp(arr, arr_size, elem_size, parent_index,
			  func, compare_param);
}

void HeapifyDown(void *arr, 
				  size_t arr_size, 
				  size_t elem_size, 
			      size_t index_of_heapify, 
			      comparison_t func, 
			      void *compare_param)
{
	size_t left_index = GetLeftIMP(index_of_heapify, arr_size);
	size_t right_index = GetRightIMP(index_of_heapify, arr_size);
	size_t child_to_cmp = 0;
	char *array = arr;
	
	assert(arr);
	assert(func);
	assert(elem_size);
	
	if ((arr_size <= index_of_heapify) || 
		(left_index <= index_of_heapify) || 
		(right_index <= index_of_heapify))
	{
		return;
	}
		
	if ((func(JUMP_TO_INDEX(left_index), JUMP_TO_INDEX(right_index), 
			  compare_param) == 1)
	    || right_index == arr_size)
	{
		child_to_cmp = left_index;
	}
	else
	{
		child_to_cmp = right_index;
	}
	if (func(JUMP_TO_INDEX(index_of_heapify), 
			 JUMP_TO_INDEX(child_to_cmp), compare_param) == 1)
	{
		return;
	}
	SwapIMP(JUMP_TO_INDEX(index_of_heapify), 
			JUMP_TO_INDEX(child_to_cmp),
			elem_size);
	HeapifyDown(arr, arr_size, elem_size, child_to_cmp, 
				func, compare_param);
}

static size_t GetParentIMP(size_t index, size_t size)
{
	size_t parent = (index - 1) / 2;
	
	if (parent >= size)
	{
		return index;
	}
	
	return parent;
}

static size_t GetLeftIMP(size_t index, size_t size)
{
	size_t left = (index * 2) + 1;
	
	if (left > size)
	{
		return index;
	}
	
	return left;
}

static size_t GetRightIMP(size_t index, size_t size)
{
	size_t right = (index * 2) + 2;
	
	if (right > size)
	{
		return index;
	}
	
	return right;
}

static void SwapIMP(void *val1, void *val2, size_t element_size)
{
    char *temp = (char *)alloca(element_size);

    memcpy(temp, val1, element_size);
    memcpy(val1, val2, element_size);
    memcpy(val2, temp, element_size);
}
