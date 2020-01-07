 /********************************************
*	Author : Maoz Roytman		
*	Reviewer : 	Tamir Dayan
*	HeapSort					
*	Date: 06/01/2020		                
*																				
********************************************/

#include <math.h>
#include <stddef.h>
#include <unistd.h>


#include "searching.h"

int *RecJSearchIMP(const int *arr, ssize_t size, int val, size_t jump);
int MaxIMP(int x, int y);
int MinIMP(int x, int y);
int *RecSearchOneByOne(const int *arr, ssize_t size, int val);

int *BSearch(const int *arr, size_t size, int val)
{
	int half_size = 0;
	int *element_to_search = NULL;
	int division_flag = 0;
	half_size = size - (size / 2);
	if (size % 2 != 0)
	{
		division_flag = 1;
	}

	if (1 == size)
	{
		half_size = 0;
	}
	element_to_search = (int*)arr + half_size;
	
	if (*element_to_search == val)
	{
		return element_to_search;
	}
	
	if (size <= 1)
	{
		if (*(int*)arr == val)
		{
			return (int *)arr;
		}
	
		return NULL;
	}
	
	if (*element_to_search < val)
	{
		return BSearch((int*)arr + (size - half_size),size - half_size + division_flag, val);
	}
	else
	{
		return BSearch((int*)arr, half_size, val);
	}
}

int *JSearch(const int *arr, size_t size, int val)
{
	size_t jump = sqrt(size);
	size_t i = 0;
	
	for (i = 0; i < size + jump; i += jump)
	{
		if (i >= size - 1)
		{
			i = size - 1;
		}
	
		if (*(int *)(arr + i) >= val)
		{
			return RecSearchOneByOne((int *)arr + i, i, val);
		}
	}
	
	return NULL;
}

int *RecJSearchIMP(const int *arr, ssize_t size, int val, size_t jump)
{		
	if (size <= 0)
	{
		return NULL;
	}
	if (*(int*)arr == val)
	{
		return (int*)arr;
	}
	if ((*(int*)arr < val) && (size >= jump))
	{
		return RecJSearchIMP((int*)arr + MinIMP(jump, size), 
							 size - MinIMP(jump, size), val, jump);
	}
	else
	{
		return RecSearchOneByOne((int*)arr, jump, val);
	}
}

int *RecSearchOneByOne(const int *arr, ssize_t size, int val)
{
	if (size < 0)
	{
		return NULL;
	}
	if (*(int *)arr == val)
	{
		return (int *)arr;
	}
	RecSearchOneByOne(arr - 1, size - 1, val);

}

int MinIMP(int x, int y)
{
	if (x > y)
	{
		return y;
	}
	else
	{
	return x;
	}

}
int MaxIMP(int x, int y)
{
	if (x > y)
	{
		return x;
	}
	else
	{
	return y;
	}

}
