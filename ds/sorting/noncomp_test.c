#include <stdio.h> /* printf */
#include <string.h> /*memcmp */
#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include <time.h> /* time */

#include "sorting.h"

#define MIN(a,b) ((a) <= (b)) ? (a) : (b)
#define MAX(a,b) ((a) >= (b)) ? (a) : (b)

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#define TEST1(result1, result2)  (((result1) == (result2)) ? \
 printf(GREEN"passed\n"RESET) : printf(RED"failed\n"RESET)) 
#define TEST2(result) (result != NULL) ? \
 printf(GREEN"passed\n"RESET) : printf(RED"failed\n"RESET) 

int CompareFunc(const void *a, const void *b);
void TestCountingSort();
void TestRadixSort();

int main()
{
	int i = 0;
	
	for (i = 0; i < 10; ++i)
	{
		/*TestCountingSort();*/
		TestRadixSort();
	}

	return 0;
}
void TestCountingSort()	
{
	int i = 0;
	int random = 0;
	int arr1[300] = {0};
	int arr2[300] = {0};
	int result[300] = {0};
	int min = 0;
	int max = 0;
	
	srand(time(NULL));
	for (i = 0; i < 300; ++i)
	{
		random = rand() % 200;
		arr1[i] = random;
		arr2[i] = random;
	}
	
	min = arr1[0];
	max = arr1[0];

	for (i = 0; i < 300; ++i)
	{
		min = MIN(min, arr1[i]);
		max = MAX(max, arr1[i]);
	}
	
	CountingSort(arr1, 300, min, max, result);
	qsort(arr2, 300, sizeof(int), CompareFunc);
	printf("CountingSort\n");
	TEST1(memcmp(result, arr2, 300*sizeof(int)), 0);
}

void TestRadixSort()	
{

	int test_arr[5] = {-1, -44, 5, -6, -2};
	int test_arr2[5] = {-1, -44, 5, -6, -2};
	int i = 0;
	int random = 0;
	int arr1[10] = {0};
	int arr2[10] = {0};

	srand(time(NULL));
	for (i = 0; i < 10; ++i)
	{
		random = rand() % 200 - 100;
		arr1[i] = random;
		arr2[i] = random;
	}

	RadixSort(arr1, 10);
	qsort(arr2, 10, sizeof(int), CompareFunc);
	printf("RadixSort\n");
	TEST1(memcmp(arr1, arr2, 10*sizeof(int)), 0);
}

int CompareFunc(const void *a, const void *b) 
{
  int va = *(const int*) a;
  int vb = *(const int*) b;
  return (va > vb) - (va < vb);
}
	
