/************************************
*		Author: ChenR				  *
*		Reviewer: Maoz				  *
*		quicksort					  *
*		2/1/2020					  *
*									  *
************************************/
#include <string.h> /* memcpy */
#include <alloca.h> /* malloc */

#include "../sorting.h"
#include "../../heap/heapify.h"
#include "../../MyUtils.h" /* MAX2,MIN2 */

#define FIRST_INDEX 0
#define ARR_ELEMENT(x) ((char *)arr + ((x) * element_size))

/* generic swap */			   				  
static void SwapIMP(void *ptr1, void *ptr2, size_t element_size);

void QuickSort(void *arr, 
			   size_t arr_size, 
			   size_t element_size, 
			   int (*compar)(const void *, const void *))
{
	size_t pivot_index = 0;
	size_t big_runner_index = 0;
	size_t small_runner_index = 0;
	
	if (arr_size <= 1)
	{
		return;
	}
	
	pivot_index = arr_size - 1;
	small_runner_index = pivot_index;
	
	while (big_runner_index != small_runner_index)
	{
		while ((0 > compar(ARR_ELEMENT(big_runner_index), 
						   ARR_ELEMENT(pivot_index))) &&
			   			   (big_runner_index != small_runner_index))
		{
			++big_runner_index;
		}
		while ((0 <= compar(ARR_	ELEMENT(small_runner_index), 
						   ARR_ELEMENT(pivot_index))) &&
			   			   (big_runner_index != small_runner_index))
		{
			--small_runner_index;
		}
		SwapIMP(ARR_ELEMENT(big_runner_index), 
				ARR_ELEMENT(small_runner_index), 
				element_size);
	}
	SwapIMP(ARR_ELEMENT(big_runner_index), 
					ARR_ELEMENT(pivot_index), 
					element_size);
	
	QuickSort(arr, 
			  big_runner_index, 
			  element_size, 
			  compar);
	QuickSort(ARR_ELEMENT(big_runner_index + 1), 
			  arr_size - big_runner_index - 1, 
			  element_size, 
			  compar);		   
}			   

static void SwapIMP(void *ptr1, void *ptr2, size_t element_size)
{
	void *size = (void *)alloca(element_size);
	
	memcpy(size, ptr1, element_size);
	memcpy(ptr1, ptr2, element_size);
	memcpy(ptr2, size, element_size);
}

