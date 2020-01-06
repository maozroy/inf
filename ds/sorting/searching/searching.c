 /********************************************
*	Author : Maoz Roytman		
*	Reviewer : 	Tamir Dayan
*	HeapSort					
*	Date: 06/01/2020		                
*																				
********************************************/

#include <math.h>
#include <stddef.h>

#include "searching.h"

int *RecJSearchIMP(const int *arr, size_t size, int val, size_t jump);

int *BSearch(const int *arr, size_t size, int val)
{
	size_t half_size = (size / 2);
	int *element_to_search = ((int*)arr + half_size - 1);
	
	if (*element_to_search == val)
	{
		return element_to_search;
	}
	
	if (size <= 1)
	{
		return NULL;
	}
	
	if (*((int*)arr + half_size - 1) < val)
	{
		return BSearch((int*)arr + half_size + 1, half_size, val);
	}
	else
	{
		return BSearch((int*)arr, half_size, val);
	}
}

int *JSearch(const int *arr, size_t size, int val)
{
	size_t jump = sqrt(size);
	
	return RecJSearchIMP(arr, size, val, jump);
	


}

int *RecJSearchIMP(const int *arr, size_t size, int val, size_t jump)
{		
	if (*(int*)arr == val)
	{
		return (int*)arr;
	}
	if ((*(int*)arr > val) || (jump < size))
	{
		return RecJSearchIMP((int*)arr - jump, size - 1, val, 1);
	}
	else
	{
		return RecJSearchIMP((int*)arr + jump, size - jump, val, jump);
	}
}












