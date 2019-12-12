/*****************************************/
/* OL79                                  */
/* Sorting                               */
/* 10/12/19                              */
/* Author- Sharon Rottner                */
/* Reviewer- Eyal Finkelshtein           */
/*****************************************/

#include <assert.h> /* assert */
#include <stddef.h> /*size_t */

#include "sorting.h"

static int IsSortedIMP(int elements_to_sort, int swapped);
static void SwapIMP(int *first_element,int *second_element);
static int *GetMinElementIMP(int *arr, size_t size);
static int MoveBigestNumToEndIMP(int *first_element, int elements_to_sort);
static size_t FindLocToInsertIMP(int *arr, size_t index);
static void AdvenseElementsByOneIMP(int *arr,
                                    size_t index,
                                    size_t index_to_insert);

void BubbleSort(int *arr, size_t size)
{
	int *first_element = arr;
	int elements_to_sort = size - 1;
	int swapped = 1;
	
	assert(arr);
	
	while (0 == IsSortedIMP(elements_to_sort, swapped))
	{
		swapped = MoveBigestNumToEndIMP(first_element, elements_to_sort);		
		--elements_to_sort;
		first_element = arr;
	}		
}

void InsertionSort(int *arr, size_t size)
{
	size_t i = 0;
	size_t index_to_insert = 0;
	int num_to_insert = 0;
	
	assert(arr);
	
	for(i = 1; i < size; ++i)
	{
		num_to_insert = arr[i];
		index_to_insert = FindLocToInsertIMP(arr, i);
		AdvenseElementsByOneIMP(arr, i, index_to_insert);
		arr[index_to_insert] = num_to_insert;
	}
}
	
void SelectionSort(int *arr, size_t size)
{
	int *first_element = arr;
	int *min_element = NULL;
	
	assert(arr);
	
	while (size > 1)
	{
		min_element = GetMinElementIMP(first_element, size);
		SwapIMP(min_element, first_element);
		++first_element;
		--size;
	}  
}

static size_t FindLocToInsertIMP(int *arr, size_t index)
{
	size_t index_to_insert = index - 1;
	
	while ((index_to_insert != 0) &&  (arr[index] < arr[index_to_insert]))
	{
		--index_to_insert;
	}
	
	return index_to_insert + 1;
}

static void AdvenseElementsByOneIMP(int *arr,
								    size_t index,
								    size_t index_to_insert)
{
	size_t j = index;
	
	for (j = index - 1; j > index_to_insert; --j)
	{
			arr[j + 1] = arr[j];
	}
}
	
static int MoveBigestNumToEndIMP(int *first_element, int elements_to_sort)
{
		int i = 0;
		int *second_element = first_element + 1;
		int swapped = 0;
		
		for(i = 0; i < elements_to_sort; ++i, ++first_element, ++second_element)
		{
			if (*second_element < *first_element)
			{
				SwapIMP(first_element, second_element);
				swapped = 1;
			}
		}
		
		return swapped;
}

static int *GetMinElementIMP(int *arr, size_t size)
{
	int *min_element = arr;
	int *current_element = arr;
	int i = 0;
	
	for (i = size; i > 0 ; --i)
	{
		if (*min_element > *current_element)
		{
			min_element = current_element;
		}
		++current_element;
	}
	
	return min_element;	
} 
	 
static void SwapIMP(int *first_element,int *second_element)
{
	int temp = *first_element;
	*first_element = *second_element;
	*second_element = temp;
}

static int IsSortedIMP(int elements_to_sort, int swapped)
{
	return (!((elements_to_sort > 0) && (1 == swapped)));
}
