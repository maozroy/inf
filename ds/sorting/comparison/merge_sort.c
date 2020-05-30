 /********************************************
*	Author : Maoz Roytman		
*	Reviewer : 	Sharon Rottner
*	MergeSort					
*	Date: 29/12/2019		                
*																				
********************************************/

#include <stdlib.h> /* Mallocing */
#include <string.h> /* Memcpy*/
#include <assert.h> /* asserting*/

#include "../sorting.h"

static void MergingIMP(int *arr1, int *arr2, size_t len1, size_t len2, int *sorted_arr);

typedef enum status
{
	FAIL = 1,
	SUCCESS = 0
}status_t;

int MergeSort(int *arr, size_t arr_size)
{
	int *sorted_arr = NULL;
	size_t half_size = arr_size - (arr_size / 2);
	status_t status = SUCCESS;
	
	assert(arr);
	assert(arr_size);
	
	if (arr_size <= 1)
	{
		return SUCCESS;
	}
	sorted_arr = (int *)calloc(arr_size, sizeof(int));
	if (NULL == sorted_arr)
	{
		return FAIL;
	}
	status |= MergeSort(arr, (arr_size / 2));
	status |= MergeSort(arr + (arr_size / 2), half_size);
	
	MergingIMP(arr, arr + (arr_size / 2), (arr_size / 2), half_size, sorted_arr);
	memcpy(arr, sorted_arr, arr_size * sizeof(int));
	free(sorted_arr);
	
	return status;
}

static void MergingIMP(int *arr1, int *arr2, size_t len1, size_t len2, int *sorted_arr)
{
	size_t len2_index = 0;
	size_t len1_index = 0;
	size_t sort_index = 0;
	
	while ((len2_index < len2) && (len1_index < len1))
	{
		if (*(arr1 + len1_index) > *(arr2 + len2_index))
		{
			*(sorted_arr + sort_index) = *(arr2 + len2_index);
			++len2_index;
		}
		else
		{
			*(sorted_arr + sort_index) = *(arr1 + len1_index);
			++len1_index;
		}
		++sort_index;
	}
	while (len2_index < len2)
	{
		*(sorted_arr + sort_index) = *(arr2 + len2_index);
		++len2_index;
		++sort_index;
	}
	while (len1_index < len1)
	{
		*(sorted_arr + sort_index) = *(arr1 + len1_index);
		++len1_index;
		++sort_index;
	}
}
