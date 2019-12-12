#include <stddef.h>
#include <assert.h>
#include <sys/types.h>

#include "../sorting.h"

static void IntSwapIMP(int *a, int *b);
static int IsSmallerIMP(int a, int b);
static size_t SmallestIndexInArrIMP(int *arr, size_t len);

void BubbleSort(int *arr, size_t len)
{
	size_t inner_loop = 0;
	size_t outer_loop = 0;

	assert(arr);
	
	for (outer_loop = 0; outer_loop < len ; ++outer_loop)
	{
		for (inner_loop = 0 ; 
			inner_loop < (len - outer_loop - 1) ;
			 ++inner_loop)
		{
			if (arr[inner_loop] > arr[inner_loop+1])
			{
				IntSwapIMP(&arr[inner_loop], &arr[inner_loop+1]);
			}
		}
	}
}

void SelectionSort(int *arr, size_t len)
{
	size_t sorted_list = 0;
	size_t unsorted_list_i = 0;
	size_t min_index = 0;
	
	assert(arr);

	for (sorted_list = 0 ; sorted_list < len - 1 ; ++sorted_list)
	{
		unsorted_list_i = sorted_list;
		min_index = SmallestIndexInArrIMP(&arr[unsorted_list_i], 
										  len - unsorted_list_i);
		IntSwapIMP(&arr[sorted_list], &arr[min_index+sorted_list]);
	}
}

void InsertionSort(int *arr, size_t len)
{
	size_t unsorted_i = 1;
	int sorted_i = 1;
	ssize_t element_to_sort = 0;
	
	assert(arr);
		
	for (unsorted_i = 1 ; unsorted_i < len ; unsorted_i ++)
	{
		element_to_sort = arr[unsorted_i];
		
		for (sorted_i = unsorted_i - 1 ; 
			 (sorted_i >= 0) && (IsSmallerIMP(element_to_sort, arr[sorted_i])) ;
			 --sorted_i)
			 {
			 	arr[sorted_i + 1] = arr[sorted_i];
			 }
			 
			 arr[sorted_i + 1] = element_to_sort;
	}
}

static size_t SmallestIndexInArrIMP(int *arr, size_t len)
{
	size_t i = 0;
	size_t min_index = 0;
	
	for (i = 0 ; i < len ; ++i)
	{
		if (IsSmallerIMP(arr[i], arr[min_index]))
		{
			min_index = i;
		}
	}
	
	return min_index;
}

static int IsSmallerIMP(int a, int b)
{
	if (a < b)
	{
		return 1;
	}
	
	return 0;
}

static void IntSwapIMP(int *a, int *b)
{
	int temp = *a;
	
	assert(a);
	assert(b);
	
	*a = *b;
	*b = temp;
}
