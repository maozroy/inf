 /********************************************
*	Author : Maoz Roytman	
*	Reviewer : 	Gal Salemon
*	heapify					
*	Date: 29/12/2019	                
*																				
********************************************/

#include <stdlib.h> /*MALLOCING*/
#include <assert.h> /*asserting*/
#include <string.h>
#include <alloca.h> /* alloca */
#include "heap.h"
#include "heapify.h"
#include "../vector/vector.h"

static size_t GetParentIMP(size_t index, size_t size);
static size_t GetRightIMP(size_t index, size_t size);
static size_t GetLeftIMP(size_t index, size_t size);
static void GenericSwapIMP(void **a, void **b);
static void SwapIMP(void *val1, void *val2, size_t element_size);
enum 
{
    FALSE,
    TRUE
};
void HeapifyUp(void *arr, 
				size_t arr_size, 
				size_t elem_size, 
			    size_t index_of_heapify, 
			    comparison_t func, 
			    void *compare_param)
{
	size_t parent_index = GetParentIMP(index_of_heapify, arr_size);
	char *array = arr;
	
	if ((func((array + parent_index * elem_size), 
			  (array + index_of_heapify * elem_size), compare_param) == 1) 
				||  (index_of_heapify == 0))
	{
		return;
	}
	
	SwapIMP((array + index_of_heapify * elem_size), 
					(array + parent_index * elem_size), elem_size);
	HeapifyUp(arr, arr_size, elem_size, parent_index, func, compare_param);
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
	
	if ((arr_size <= index_of_heapify) || 
		(left_index == index_of_heapify) || 
		(right_index == index_of_heapify))
	{
		return;
	}
		
	if (func((array + left_index * elem_size), 
			(array + right_index * elem_size), compare_param) == 1 || right_index == arr_size)
	{
		child_to_cmp = left_index;
	}
	else
	{
		child_to_cmp = right_index;
	}
	if (func((array + index_of_heapify * elem_size), 
		(array + child_to_cmp * elem_size), compare_param) == 1)
	{
		return;
	}	
	SwapIMP((array + index_of_heapify * elem_size), 
					(array + child_to_cmp * elem_size), elem_size);
	HeapifyDown(arr, arr_size, elem_size, child_to_cmp, func, compare_param);
}


static int HasLeftChildIMP(size_t index_of_heapify, size_t arr_size)
{
    size_t index_left_child = (index_of_heapify * 2) + 1;
    
    if (index_left_child >= arr_size)
    {
        return FALSE;
    }
    
    return TRUE;
}

static int HasRightChildIMP(size_t index_of_heapify, size_t arr_size)
{
    size_t index_right_child = (index_of_heapify * 2) + 2;
    
    if (index_right_child >= arr_size)
    {
        return FALSE;
    }
    
    return TRUE;
}
/*
void HeapifyDown(void *arr, 
				 size_t arr_size, 
				 size_t elem_size, 
			     size_t index_of_heapify, 
			     comparison_t func, 
			     void *compare_param)
{
    char *heapify = (char *)arr + index_of_heapify * elem_size;
    char *child_to_check = NULL;
    char *left_child = NULL;
    char *right_child = NULL;
    size_t index_left_child = (2 * index_of_heapify) + 1;
    size_t index_right_child = (2 * index_of_heapify) + 2;
    size_t new_heapify_index = index_of_heapify;
    
    if (HasLeftChildIMP(index_of_heapify, arr_size) && 
        HasRightChildIMP(index_of_heapify, arr_size))
    { 
        left_child = (char *)arr + index_left_child * elem_size;
        right_child = (char *)arr + index_right_child * elem_size;
                       
        if (1 == func(left_child, right_child, compare_param))         
        {
            child_to_check = right_child;
            new_heapify_index = index_right_child;
        }
    
        else 
        {
            child_to_check = left_child;
            new_heapify_index = index_left_child;
        }
    }
    
    else if (HasLeftChildIMP(index_of_heapify, arr_size))
    {
        left_child = (char *)arr + index_left_child * elem_size;
        child_to_check = left_child;
        new_heapify_index = index_left_child;             
    }

    if ((NULL == child_to_check) ||
        (1 != func(heapify, child_to_check, compare_param)))
    {
        return;
    }
    
    SwapIMP(heapify, child_to_check, elem_size);
    HeapifyDown(arr, arr_size, elem_size, new_heapify_index, func, 
                compare_param);	     
}
*/
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
static void SwapIMP(void *val1, void *val2, size_t element_size)
{
    char *temp = (char *)alloca(element_size);

    memcpy(temp, val1, element_size);
    memcpy(val1, val2, element_size);
    memcpy(val2, temp, element_size);
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

static void GenericSwapIMP(void **a, void **b)
{
	void *temp = malloc(sizeof(void *));
	
	memcpy(temp,*a, sizeof(void*) );
		memcpy(*a,*b, sizeof(void*) );
			memcpy(b,temp, sizeof(void*) );
	free(temp);	
}








