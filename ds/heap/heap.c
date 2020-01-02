 /********************************************
*	Author : Maoz Roytman	
*	Reviewer : 	Gal Salemon
*	heap					
*	Date: 29/12/2019	                
*																				
********************************************/

#include <stdlib.h> /*MALLOCING*/
#include <assert.h> /*asserting*/
#include <unistd.h> /* ssize_t */

#include "heap.h" 
#include "heapify.h" /*heapifying*/
#include "../vector/vector.h" /*engine*/

#define START_CAPACITY (1)
#define THE_VECTOR (heap -> vector)
#define ROOT (0)
#define LAST_INDEX (VectorSize(THE_VECTOR) - 1)
#define ELEM_SIZE (sizeof(void *))
#define NOT_FOUND -1

typedef enum status
{
	SUCCESS = 0,
	FAIL = 1
}status_t;

enum is_before
{
	IS_BEFORE = 1,
	IS_NOT_BEFORE = 0
};

struct heap
{
	d_vector_t *vector;
	comparison_t comparison_func;
	void *param;
};

static void SwapElementIMP(void **data1, void **data2);
int HeapComparisonIMP(const void *new_data, const void *src_data, 
						void *compare_param);

heap_t *HeapCreate(comparison_t comparison_func, void *comparison_param)
{
	heap_t *heap = NULL;
	d_vector_t *array = NULL;
	
	assert(comparison_func);
	
	heap = (heap_t *)malloc(sizeof(heap_t));
	if (NULL == heap)
	{
		return NULL;
	}
	
	array = VectorCreate(START_CAPACITY, ELEM_SIZE);
	if (NULL == array)
	{
		free(heap);
		return NULL;
	}
	
	heap -> comparison_func = comparison_func;
	heap -> param = comparison_param;
	THE_VECTOR = array;
	
	return heap;
}

void HeapDestroy(heap_t *heap)
{
	assert(heap);
	
	VectorDestroy(THE_VECTOR);
	heap -> vector = NULL;
	heap -> comparison_func = NULL;
	heap -> param = NULL;
	free(heap);
}

int HeapPush(heap_t *heap, void *data)
{
	assert(data);
	assert(heap);

	if (VectorPushBack(THE_VECTOR, &data) == FAIL)
	{
		return FAIL;
	}
	HeapifyUp(VectorGetItemAddress(THE_VECTOR, ROOT), VectorSize(THE_VECTOR), 
				ELEM_SIZE, LAST_INDEX, 
				HeapComparisonIMP, heap);

	return SUCCESS;
}

void *HeapPeek(const heap_t *heap)
{
	assert(heap);
	if (HeapIsEmpty(heap))
	{
		return NULL;
	}
	return *(void **)VectorGetItemAddress(THE_VECTOR, ROOT);

}

int HeapComparisonIMP(const void *new_data, const void *src_data, 
						void *compare_param)
{
	heap_t *heap = (void *)compare_param;
	comparison_t func = heap -> comparison_func;
	
	return (func(*(void **)new_data,
				 *(void **)src_data,
				 heap -> param));
}

void HeapPop(heap_t *heap)
{
	void *root = NULL;
	void *last_element = NULL;
	
	assert(heap);
	
	root = VectorGetItemAddress(THE_VECTOR, ROOT);
	last_element = VectorGetItemAddress(THE_VECTOR, LAST_INDEX);
	
	SwapElementIMP(root, last_element);
	VectorPopBack(THE_VECTOR);	
	
	HeapifyDown(VectorGetItemAddress(THE_VECTOR, ROOT), 
				VectorSize(THE_VECTOR), ELEM_SIZE, ROOT, 
				HeapComparisonIMP, heap);
}

void SwapElementIMP(void **data1, void **data2)
{
	void *temp = data1;
	
	*data1 = *data2;
	*data2 = temp;
}

size_t HeapSize(const heap_t *heap)
{
	assert(heap);

	return (VectorSize(THE_VECTOR));
}

int HeapIsEmpty(const heap_t *heap)
{
	assert(heap);

	return (VectorSize(THE_VECTOR) == 0);
}

static ssize_t FindIndexToRemoveIMP(d_vector_t *vector, 
									is_match_t func, void *param)
{
	ssize_t i = 0;
	ssize_t size = 0;
	ssize_t result = NOT_FOUND;
	
	assert(vector);
	assert(func);
	
	size = VectorSize(vector);
	for (i = 0 ; i < size ; ++i)
	{
		if (func(*(char **)VectorGetItemAddress(vector, i), param) == IS_BEFORE)
		{
			result = i;
			break;
		}
	}
	
	return result;
}

void *HeapRemove(heap_t *heap, is_match_t is_match_func, void *param)
{
	ssize_t index_to_remove = 0;
	void *elem_to_remove = NULL;
	void *last_element = NULL;
	void *parent = NULL;
	void *data_to_return = NULL;
	void *root = NULL;
	
	assert(heap);
	assert(is_match_func);
	
	index_to_remove = FindIndexToRemoveIMP(THE_VECTOR, is_match_func, param);

	if (NOT_FOUND == index_to_remove)
	{	
		return NULL;
	}
	elem_to_remove = VectorGetItemAddress(THE_VECTOR, index_to_remove);
	data_to_return = *(void **)elem_to_remove;		
	
	if (index_to_remove == ROOT)
	{
		HeapPop(heap);
	}
	else
	{
		parent = VectorGetItemAddress(THE_VECTOR, index_to_remove / 2);
		last_element = VectorGetItemAddress(THE_VECTOR, LAST_INDEX);

		SwapElementIMP(elem_to_remove, last_element);
		VectorPopBack(THE_VECTOR);
		root = VectorGetItemAddress(THE_VECTOR, ROOT);
		
		if (HeapComparisonIMP(elem_to_remove, parent, heap) == IS_BEFORE)
		{
			HeapifyUp(root, VectorSize(THE_VECTOR), 
				ELEM_SIZE, index_to_remove, 
				HeapComparisonIMP, heap);
		}
		else
		{
			HeapifyDown(root, VectorSize(THE_VECTOR), 
				ELEM_SIZE, index_to_remove, 
				HeapComparisonIMP, heap);
		}
	}

	return data_to_return;
}
