#include <stdio.h> /* printf */
#include <string.h> /*memcmp */
#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include <time.h>
#include <unistd.h>

#include "sorting.h"

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#define TEST1(result1, result2)  (((result1) == (result2)) ? \
 printf(GREEN"passed\n"RESET) : printf(RED"failed\n"RESET)) 
#define TEST2(result) (result != NULL) ? \
 printf(GREEN"passed\n"RESET) : printf(RED"failed\n"RESET) 
 #define UNUSED(x) ((void)(x))
 
 #define NUM_OF_ITER 20
 
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
	    sleep(1);
	}

    return 0;
}

void TestHeapSort()
{
	int i = 0;
	int random = 0;
	int arr1[NUM_OF_ITER] = {0};
	int arr2[NUM_OF_ITER] = {0};
	int test[4] = {2,0, 2, 8};
	srand(time(NULL));
	printf("original is\n");
	for (i = 0; i < NUM_OF_ITER; ++i)
	{

		random = rand()%10;
		arr1[i] = random;
		arr2[i] = random;
		printf("%d\n",arr1[i]);
	}
	
	QuickSort(arr2, NUM_OF_ITER, sizeof(int),IsBeforeIMP);
	qsort(arr1, NUM_OF_ITER, sizeof(int), CompareFunc);
	printf("HeapSort\n");
		printf("mine \t qsort\n");
	for (i = 0 ; i < NUM_OF_ITER ; i++)
	{

		printf("%d\t",arr2[i]);
		printf("%d",arr1[i]);
		printf("\n");
	}
	TEST1(memcmp(arr1, arr2, NUM_OF_ITER*sizeof(int)), 0);
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
