/****************************************************************************
                                                                           
			                      searching	                        
                               OL79 - 7/1/20                              
                                                                           
                             Writer:    Gal Salemon                       
                             Reviewer:  Maoz                             
*****************************************************************************/
#include <stdlib.h> /* size_t */
#include <assert.h> /* assert */
#include <math.h>   /* sqrt */

#include "searching.h"

#define MIN2(a,b) ((a) <= (b) ? (a) : (b))

static int *RecBSearch(const int *arr, int val, size_t left, size_t right);

int *BSearch(const int *arr, size_t len, int val)
{
	return RecBSearch(arr, val, 0, len - 1);
}

static int *RecBSearch(const int *arr, int val, size_t left, size_t right)
{
	size_t mid = (left + right) / 2;
	
	if (left > right)
	{
		return NULL;
	}
	
	if (arr[mid] == val)
	{		
		return (int*)(arr + mid);
	}
	
	if (val < arr[mid])
	{
		return RecBSearch(arr, val, left, mid - 1);
	}
	
	return RecBSearch(arr, val, mid + 1, right);
}

int *JSearch(const int *arr, size_t size, int val)
{
	size_t jump = sqrt(size);
	size_t i = 0;
    size_t left = 0;
    size_t right = 0;
    int *arr_runner = (int *)arr;

    assert(arr);
	
    while (left < size && arr_runner[left] <= val)
    {
        right = MIN2(size - 1, left + jump);

        if ((arr_runner[left] <= val) && (arr_runner[right] >= val))
        {
            break;
        }

        left += jump;
    }

    if (left >= size || arr_runner[left] > val)
    {
		return NULL;
    }

    right = MIN2(size - 1, right);
		
    for (i = left; i <= right && arr_runner[i] <= val; ++i)
    {
        if (arr_runner[i] == val)
        {
            return arr_runner + i;
        }
    }

	return NULL;
}


