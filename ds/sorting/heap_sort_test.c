#include <stdio.h> /* printf */
#include <string.h> /*memcmp */
#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include <time.h>

#include "sorting.h"

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#define TEST1(result1, result2)  (((result1) == (result2)) ? \
 printf(GREEN"passed\n"RESET) : printf(RED"failed\n"RESET)) 
#define TEST2(result) (result != NULL) ? \
 printf(GREEN"passed\n"RESET) : printf(RED"failed\n"RESET) 
 #define UNUSED(x) ((void)(x))
 
void TestHeapSort();
int CompareFunc(const void *a, const void *b) ;
int IsBeforeIMP(const void *data1, 
               const void *data2, 
               void *param);
 
int main()
{
	int i = 0;

	for (i = 0; i < 30; ++i)
	{
	    TestHeapSort();
	}

    return 0;
}

void TestHeapSort()
{
	int i = 0;
	int random = 0;
	int arr1[500] = {0};
	int arr2[500] = {0};
	srand(time(NULL));
	for (i = 0; i < 500; ++i)
	{

		random = rand();
		arr1[i] = random;
		arr2[i] = random;
	}

	HeapSort(arr1, 500, sizeof(int),IsBeforeIMP, NULL);
	qsort(arr2, 500, sizeof(int), CompareFunc);
	printf("HeapSort\n");
	TEST1(memcmp(arr1, arr2, 500*sizeof(int)), 0);
}

int IsBeforeIMP(const void *data1, 
               const void *data2, 
               void *param)
{
     UNUSED(param);
     
    if (*(int *)data1 < *(int *)data2)
    {
        return 1;
    }
    
    else 
    {
        return 0;
    }
}  

int CompareFunc(const void *a, const void *b) 
{
    int va = *(const int*) a;
    int vb = *(const int*) b;
  
    return (va > vb) - (va < vb);
}        
