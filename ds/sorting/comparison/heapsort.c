 /********************************************
*	Author : Maoz Roytman		
*	Reviewer : 	Sharon Rottner
*	HeapSort					
*	Date: 29/12/2019		                
*																				
********************************************/

#include <stdlib.h> /* Mallocing */
#include <string.h> /* Memcpy*/
#include <assert.h> /* asserting*/
#include <alloca.h> /* ALLO(C)A */

#include "../../heap/heapify.h"
#include "../sorting.h"

static void SwapFirstLastIMP(void *arr, size_t arr_size, size_t elem_size);
static int UpSideIsBeforeIMP(const void *new_data, const void *src_data, 
								void *compare_param);
static void HippiDownFromMiddleIMP(void *arr,size_t arr_size, size_t elem_size, 
									is_before_t func, void *wrapper);
									
typedef struct upside_comp
{
	is_before_t func;
	void *compare_param;
}upside_t;


void HeapSort(void *arr, size_t arr_size, size_t elem_size, 
				is_before_t func, void *param)
{
	upside_t wrapper = {0};
	
	assert(arr);
	assert(elem_size);
	assert(func);
	
	wrapper.func = func;
	wrapper.compare_param = param;
	
	HippiDownFromMiddleIMP(arr, arr_size, elem_size, 
							UpSideIsBeforeIMP, &wrapper);
	
	while (arr_size > 0)
	{
		SwapFirstLastIMP(arr, arr_size, elem_size);
		--arr_size;
		HeapifyDown(arr, arr_size, elem_size, 0, UpSideIsBeforeIMP, &wrapper);
	}
}

static void SwapFirstLastIMP(void *arr, size_t arr_size, size_t elem_size)
{
	char *runner = arr;
	void *holder = alloca(elem_size);
	
	memcpy(holder, runner, elem_size);
	memcpy(runner, (runner + ((arr_size - 1) * elem_size)), elem_size);
	memcpy(runner + ((arr_size - 1) * elem_size), holder, elem_size);
}

static int UpSideIsBeforeIMP(const void *new_data, const void *src_data, 
								void *compare_param)
{
	upside_t *wrapper = compare_param;
	
	return !(wrapper -> func(new_data, src_data, wrapper -> compare_param));
}

static void HippiDownFromMiddleIMP(void *arr,size_t arr_size, size_t elem_size, 
			is_before_t func, void *wrapper)
{
	int i = 0;

	for (i = (arr_size / 2) - 1; i >= 0 ; --i)
	{
		HeapifyDown(arr, arr_size, elem_size, i, func, wrapper);
	}
}
