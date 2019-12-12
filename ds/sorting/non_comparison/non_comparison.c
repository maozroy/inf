#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "../sorting.h"

#define BASE 10

void CountingSortIMP(const void *arr_to_sort,
					 size_t element_size,
					 size_t arr_size,
					 unsigned int *histogram,
					 size_t his_size,
					 size_t (*GetBucket)(const void *element, const void *func_param),
					 void *func_param,
					 void *arr_result);
					 
					 
static size_t IntGetBucketIMP(const void *element,const void *func_param);
static size_t IntRadixGetBucketIMP(const void *element,const void *func_param);
static size_t NumOfDigitsIMP(int num);
static size_t MaxNumOfDigitsInArrIMP(int *arr, size_t arr_size);

int CountingSort(const int *arr, size_t arr_size, 
				int min_val, int max_val, int *result)
{
	size_t his_size = max_val - min_val + 1;
	unsigned int *histogram = (unsigned int *)
								calloc(his_size, sizeof(unsigned int));
	if (NULL == histogram)
	{
		return 1;
	}
	
	CountingSortIMP(arr, sizeof(int), arr_size, histogram, his_size, 
					IntGetBucketIMP, (void *)&min_val, result);
	
	free(histogram);				
	
	
	return 0;
}

void CountingSortIMP(const void *arr_to_sort,
					 size_t element_size,
					 size_t arr_size,
					 unsigned int *histogram,
					 size_t his_size,
					 size_t (*GetBucket)(const void *element,
										 const void *func_param),
					 void *func_param,
					 void *arr_result)
{
	ssize_t i = 0;
	char *arr_to_sort_runner = NULL;
	char *arr_result_runner = NULL;
	int insert_index = 0;
	
	
	arr_to_sort_runner = (char *)arr_to_sort;
	arr_result_runner = (char *)arr_result;
	
	for (i = 0 ; i < (ssize_t)arr_size ; i++)
	{
		
		++histogram[GetBucket(arr_to_sort_runner, func_param)];
		arr_to_sort_runner += element_size;
		
	}
	
	for(i = 1 ; i < (ssize_t)his_size ; i++)
	{
		histogram[i] += histogram[i - 1];
		
	}
	for (i = arr_size - 1 ; i >= 0; --i)
	{
		arr_to_sort_runner -= element_size;
		insert_index = GetBucket(arr_to_sort_runner, func_param);
		memcpy(arr_result_runner + ((histogram[insert_index] - 1) * element_size),
			 arr_to_sort_runner, element_size);
		 histogram[insert_index] -= 1;
	}

}


int RadixSort(int *arr, size_t arr_size)
{
	size_t max_digits = 0;	
	size_t i = 0;
	unsigned int *histogram = (unsigned int *)
								calloc(19, sizeof(unsigned int));
	int *sorted_arr = (int*) calloc(arr_size, sizeof(int));
	if (NULL == histogram || NULL == sorted_arr)
	{
		return 1;
	}
	max_digits = MaxNumOfDigitsInArrIMP(arr, arr_size);
	
	for (i = 0 ; (i < max_digits) ; i++)
	{
		CountingSortIMP(arr, sizeof(int), arr_size, histogram, 19, 
						IntRadixGetBucketIMP,&i, sorted_arr);
		memcpy(arr, sorted_arr, arr_size*sizeof(int));
		memset(histogram, 0, 19 * sizeof(unsigned int));
	}
	free(histogram);
	free(sorted_arr);
	
	return 0;

}

static size_t MaxNumOfDigitsInArrIMP(int *arr, size_t arr_size)
{
	size_t max_digits = 0;
	
	while (arr_size > 0)
	{
		if (NumOfDigitsIMP(arr[arr_size - 1]) > max_digits)
		{
			max_digits = NumOfDigitsIMP(arr[arr_size - 1]);
		}
	--arr_size;
	}
	
	return max_digits;
}

static size_t NumOfDigitsIMP(int num)
{
	size_t result = 0;
	
	while (num != 0)
	{
		++result;
		num /= 10;
	}
	
	return result;
}

static size_t IntRadixGetBucketIMP(const void *element,const void *func_param)
{
	int digit_num = *(int *)func_param;
	int num = *(int*)element;
	
	while (digit_num > 0)
	{
		num /= BASE;
		--digit_num;
	}
	
	return (num % BASE) - (-9);
}

static size_t IntGetBucketIMP(const void *element,const void *func_param)
{
	return *(int *) element - *(int *)func_param;
}

				
	


