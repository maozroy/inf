 /********************************************
*	Author : Maoz Roytman	
*	Reviewer : 	Gal Salemon
*	heap					
*	Date: 29/12/2019	                
*																				
********************************************/

#include <stdlib.h> /*MALLOCING*/
#include <assert.h> /*asserting*/
#include <unistd.h>


#include "heap.h"
#include "heapify.h"
#include "../vector/vector.h"

#define START_CAPACITY (1)
#define THE_VECTOR (heap -> vector)
#define ROOT (0)
typedef enum status
{
	SUCCESS = 0,
	FAIL = 1
}status_t;



struct heap
{
	d_vector_t *vector;
	comparison_t comparison_func;
	void *param;
};

static void SwapElementIMP(void **data1, void **data2);
int HeapComparisonIMP(const void *new_data, const void *src_data, void *compare_param);
void PrintArrayIMP (heap_t *heap);


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
	
	array = VectorCreate(START_CAPACITY, sizeof(void *));
	if (NULL == array)
	{
		return NULL;
	}
	
	heap -> comparison_func = comparison_func;
	heap -> param = comparison_param;
	THE_VECTOR = array;
	
	return heap;
}

void HeapDestroy(heap_t *heap)
{
	VectorDestroy(THE_VECTOR);

	heap -> vector = NULL;
	heap -> comparison_func = NULL;
	heap -> param = NULL;	
		free(heap);
}

int HeapPush(heap_t *heap, void *data)
{
	if (VectorPushBack(THE_VECTOR, &data) == FAIL)
	{
		return FAIL;
	}
	HeapifyUp(VectorGetItemAddress(THE_VECTOR, 0), VectorSize(THE_VECTOR), 
				sizeof(void *), VectorSize(THE_VECTOR) - 1, 
				HeapComparisonIMP, heap);

	return SUCCESS;
}

void *HeapPeek(const heap_t *heap)
{
	return *(char **)VectorGetItemAddress(THE_VECTOR, 0);

}

int HeapComparisonIMP(const void *new_data, const void *src_data, void *compare_param)
{
	heap_t *heap = (void *)compare_param;
	comparison_t func = heap -> comparison_func;
	return (func(*(void **)new_data, *(void **)src_data, heap -> param));

}


void HeapPop(heap_t *heap)
{
	void *root = VectorGetItemAddress(THE_VECTOR, 0);
	void *last_element = VectorGetItemAddress(THE_VECTOR, VectorSize(THE_VECTOR) - 1);
	
	SwapElementIMP(root, last_element);
	VectorPopBack(THE_VECTOR);	
	
	HeapifyDown(VectorGetItemAddress(THE_VECTOR, 0), VectorSize(THE_VECTOR), 
				sizeof(void *), 0, 
				HeapComparisonIMP, (void *)heap);
}


void SwapElementIMP(void **data1, void **data2)
{
	void *temp = data1;
	
	*data1 = *data2;
	*data2 = temp;
}

size_t HeapSize(const heap_t *heap)
{
	return (VectorSize(THE_VECTOR));
}

int HeapIsEmpty(const heap_t *heap)
{
	return (VectorSize(THE_VECTOR) == 0);
}

void PrintArrayIMP (heap_t *heap)
{
	char *arr = VectorGetItemAddress(THE_VECTOR, 0);
	size_t size = VectorSize(THE_VECTOR);
	size_t i = 0;
	
	for (i = 0; i < size ; i++)
	{
		printf("%d ",**(int**)arr);
		arr += 8;
	}

}
static ssize_t FindIndexToRemoveIMP(d_vector_t *vector, is_match_t func, void *param)
{
	ssize_t i = 0;
	ssize_t size = VectorSize(vector);
	ssize_t result = -1;
	for (i = 0 ; i < size ; ++i)
	{
		if (func(*(char **)VectorGetItemAddress(vector, i), param) == 1)
		{
			result = i;
			break;
		}
	}
	return result;
}

int HeapRemove(heap_t *heap, is_match_t is_match_func, void *param)
{

	ssize_t index_to_remove = 0;
	void *elem_to_remove = NULL;
	void *last_element = VectorGetItemAddress(THE_VECTOR, VectorSize(THE_VECTOR) - 1);
	void *parent = NULL;
	index_to_remove = FindIndexToRemoveIMP(THE_VECTOR, is_match_func, param);
	if (-1 == index_to_remove)
	{	
		return FAIL;
	}
	if (index_to_remove == ROOT)
	{
		HeapPop(heap);
	}
	else
	{
		elem_to_remove = VectorGetItemAddress(THE_VECTOR, index_to_remove);
		parent = VectorGetItemAddress(THE_VECTOR, index_to_remove / 2);
		
		SwapElementIMP(elem_to_remove, last_element);
		VectorPopBack(THE_VECTOR);
		
		if (HeapComparisonIMP(elem_to_remove, parent, heap) == 1)
		{
			HeapifyUp(VectorGetItemAddress(THE_VECTOR, 0), VectorSize(THE_VECTOR), 
				sizeof(void *), index_to_remove, 
				HeapComparisonIMP, heap);
		}
		else
		{
			HeapifyDown(VectorGetItemAddress(THE_VECTOR, 0), VectorSize(THE_VECTOR), 
				sizeof(void *), index_to_remove, 
				HeapComparisonIMP, heap);
		}
	
	}

				
	return SUCCESS;
}



void HeapifyDown(void *arr, 
				  size_t arr_size, 
				  size_t elem_size, 
			      size_t index_of_heapify, 
			      comparison_t func, 
			      void *compare_param);









