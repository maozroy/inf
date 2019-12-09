#include <stddef.h>
#include <stdio.h>

#include "sort.h"

static void IntSwapIMP(int *a, int *b);
static int IntMinIMP(int a, int b);
void MoveAndReplace(int *arr, size_t i);


void BubbleSort(int *arr, size_t len)
{
	size_t i = 0;
	size_t j = 0;

	
	while (j < len)
	{
		while (i < (len - j) - 1)
		{
			if (arr[i] > arr[i+1])
			{
				IntSwapIMP(&arr[i], &arr[i+1]);
				printf("len is %ld, this is %ld swap\n, j is %ld",len, i, j);
			}
			++i;
		}
		
		++j;	
		i = 0;
		printf("j is %ld",j);
	}
}

void SelectionSort(int *arr, size_t len)
{
	size_t i = 0;
	size_t j = 0;
	size_t min_keeper = 0;
	
	while (i < len - 1)
	{
		while (j < len - 1)
		{
			++j;
			
			if (IntMinIMP(arr[j], arr[min_keeper]))
				{
					min_keeper = j;
				}				
		}
		IntSwapIMP(&arr[i], &arr[min_keeper]);
		
		++i;
		j = i;
		min_keeper = i;
	}
}

void InsertionSort(int *arr, size_t len)
{
	size_t i = 0;
	size_t j = 0;
	
	while (i < len - 1)
	{
		while (j < len)
		{
			if (IntMinIMP(arr[j], arr[i]))
			{
					MoveAndReplace(&arr[i], j - i);
					break;
			}
			++j;
		}
		++i;
		j = i;
	}
	
}

void MoveAndReplace(int *arr, size_t i)
{
	int temp = arr[i];
	
	while (i > 0)
	{
		arr[i] = arr[i-1];
		--i;
	}
	arr[0] = temp;
}

static int IntMinIMP(int a, int b)
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
	*a = *b;
	*b = temp;
}
