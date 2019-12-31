/****************************************************************************
*                                                                           *
*                               OL79 - 01/12/19                             *
*                               Auther - Erez                               *	
*                               Binary Sorting Tree 	                    *
*																			*
*****************************************************************************/


#include <stdlib.h> /* malloc */
#include <stdio.h>  /* printf */
#include <time.h>   /* clock  */
#include <string.h> /* memcmp */

#include "my_utils.h"
#include "heap.h"
#include "../vector/vector.h"

#define ARR_SIZE1 (5)
#define ARR_SIZE2 (20)

struct heap
{
	d_vector_t *vector;
	comparison_t comparison_func;
	void *param;
	
};

void TestCreateDestroy();
void TestEmptySize();
void TestPushPeekPop();
void PrintArray(heap_t *heap);
void TestRemove();

int Comparison(const void *new_data, const void *src_data, void *compare_param);
int IsMatch(const void *new_data, const void *src_data);

/*************************************************/
int main ()
{
	TestCreateDestroy();
	TestEmptySize();
	TestPushPeekPop();
	TestRemove();

	return (0);
}
int Comparison(const void *new_data, const void *src_data, void *compare_param)
{
	UNUSED(compare_param);
	if (*(int*)new_data > *(int*)src_data)
	{
		return (1);
	}
	else if(*(int*)new_data < *(int*)src_data)
	{
		return (-1);
	}

	return (0);
}
int IsMatch(const void *new_data, const void *src_data)
{
	if (*(int*)new_data ==  *(int*)src_data)
	{
		return (1);
	}

	return (0);
}
void PrintArray(heap_t *heap)
{
	char *array = VectorGetItemAddress((d_vector_t*)heap->vector, 0);
	size_t size = HeapSize(heap);
	size_t i = 0;
	
	for (i = 0; i < size; ++i)
	{
		printf("%d ", **(int**)array);
		if (i == 1 || i == 4 || i == 10 || i == 22)
		{
			printf("- ");
		}
		if (i == 0 || i == 2 || i == 6 || i == 14 || i == 30)
		{
			printf("|| ");
		}
		array +=8;
	}
	printf("\n");
}
void TestCreateDestroy()
{
	heap_t *my_heap = HeapCreate(Comparison, NULL);

	HeapDestroy(my_heap);
}
void TestEmptySize()
{
	heap_t *my_heap1 = HeapCreate(Comparison, NULL);
	heap_t *my_heap2 = HeapCreate(Comparison, NULL);

	int array1[ARR_SIZE1] = {6,3,9,1,0};
	int array2[ARR_SIZE2] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};

	size_t i = 0; 

	RUN_TEST(1 == HeapIsEmpty(my_heap1) && 1 == HeapIsEmpty(my_heap2) ,"Heap Is Empty")
	RUN_TEST(0 == HeapSize(my_heap1) && 0 == HeapSize(my_heap2),"Heap Is Size 0")

	for (i = 0; i < ARR_SIZE1; ++i)
	{
		HeapPush(my_heap1,&array1[i]);
	}

	for (i = 0; i < ARR_SIZE2; ++i)
	{
		HeapPush(my_heap2,&array2[i]);
	}

	RUN_TEST(0 == HeapIsEmpty(my_heap1) && 0 == HeapIsEmpty(my_heap2) ,"Heap Is Not Empty After Push")
	RUN_TEST(ARR_SIZE1 == HeapSize(my_heap1) && ARR_SIZE2 == HeapSize(my_heap2),"Heap Is Correct Size After Push")

	for (i = 0; i < ARR_SIZE1; ++i)
	{
		HeapPop(my_heap1);
	}

	for (i = 0; i < ARR_SIZE2 / 2 ; ++i)
	{
		HeapPop(my_heap2);
	}

	RUN_TEST(1 == HeapIsEmpty(my_heap1) && 0 == HeapIsEmpty(my_heap2) ,"Empty Correct After Pop")
	RUN_TEST(0 == HeapSize(my_heap1) && ARR_SIZE2 / 2 == HeapSize(my_heap2),"Heap Is Correct Size After Pop")
/*
	printf("Empty: H1 = %d, H2 = %d\n", HeapIsEmpty(my_heap1), HeapIsEmpty(my_heap2));
	printf("Size: H1 = %ld, H2 = %ld\n", HeapSize(my_heap1), HeapSize(my_heap2));
*/

	HeapDestroy(my_heap1);
	HeapDestroy(my_heap2);	
}

void TestPushPeekPop()
{
	heap_t *my_heap1 = HeapCreate(Comparison, NULL);
	heap_t *my_heap2 = HeapCreate(Comparison, NULL);

	int array1[ARR_SIZE1] = {6,3,9,1,0};
	int array2[ARR_SIZE2] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
	int priorty1[ARR_SIZE1] = {6,6,9,9,9};
	int priorty2[ARR_SIZE2] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
	int popping1[ARR_SIZE1] = {9,6,3,1,0};
	int popping2[ARR_SIZE2] = {19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};

	size_t i = 0; 
	int test1 = 1;
	int test2 = 1;

	for (i = 0; i < ARR_SIZE1; ++i)
	{
		HeapPush(my_heap1,&array1[i]);
		(*(int*)HeapPeek(my_heap1) == priorty1[i] ? (test1 = 1 * test1) : (test1 = 0));
		printf("In Entry: %ld Expected: %d Got: %d\n", i+1, priorty1[i], *(int*)HeapPeek(my_heap1));
		PrintArray(my_heap1); 
	}

	for (i = 0; i < ARR_SIZE2; ++i)
	{
		HeapPush(my_heap2,&array2[i]);
		(*(int*)HeapPeek(my_heap2) == priorty2[i] ? (test1 = 1 * test1) : (test1 = 0));		
		printf("In Entry: %ld Expected: %d Got: %d\n", i, priorty2[i], *(int*)HeapPeek(my_heap2));		
		PrintArray(my_heap2);
	}
 
	RUN_TEST(test1 && test2 ,"Priority Is Good During Push")

	for (i = 0; i < ARR_SIZE1; ++i)
	{
		(*(int*)HeapPeek(my_heap1) == popping1[i] ? (test1 = 1 * test1) : (test1 = 0));
		printf("In Entry: %ld Expected: %d Got: %d\n", i, popping1[i], *(int*)HeapPeek(my_heap1));	
		
		PrintArray(my_heap1); 						
		HeapPop(my_heap1);
		printf("--\n");
	}

	for (i = 0; i < ARR_SIZE2 ; ++i)
	{
		(*(int*)HeapPeek(my_heap2) == popping2[i] ? (test2 = 1 * test2) : (test2 = 0));
		printf("In Entry: %ld Expected: %d Got: %d\n", i, popping2[i], *(int*)HeapPeek(my_heap2));		
		PrintArray(my_heap2); 	
		HeapPop(my_heap2);
	}

	RUN_TEST(test1 && test2 ,"Priority Is Good During Pop")


	HeapDestroy(my_heap1);
	HeapDestroy(my_heap2);	
}


void TestRemove()
{
	heap_t *my_heap1 = HeapCreate(Comparison, NULL);
	heap_t *my_heap2 = HeapCreate(Comparison, NULL);

	int array1[ARR_SIZE1] = {6,3,9,1,0};
	int array2[ARR_SIZE2] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
	int priorty1[ARR_SIZE1] = {6,9,1,3,0};
	int priorty2[ARR_SIZE2] = {17,18,19,16,15,10,11,13,12,14,0,1,2,3,4,9,8,7,6,5};
	int remove_priority1[ARR_SIZE1] = {9,9,3,3,0};
	int remove_priority2[ARR_SIZE2] = {19,19,19,16,15,14,14,14,14,14,9,9,9,9,9,9,8,7,6,5};

	size_t i = 0; 
	int test1 = 1;
	int test2 = 1;

	for (i = 0; i < ARR_SIZE1; ++i)
	{
		HeapPush(my_heap1,&array1[i]);
	}

	for (i = 0; i < ARR_SIZE2; ++i)
	{
		HeapPush(my_heap2,&array2[i]);
	}
	printf("Empty: H1 = %d, H2 = %d\n", HeapIsEmpty(my_heap1), HeapIsEmpty(my_heap2));
	printf("Size: H1 = %ld, H2 = %ld\n", HeapSize(my_heap1), HeapSize(my_heap2));
	for (i = 0; i < ARR_SIZE1; ++i)
	{
		(*(int*)HeapPeek(my_heap1) == remove_priority1[i] ? (test1 = 1 * test1) : (test1 = 0));
		printf("In Entry: %ld Expected: %d Got: %d\n", i, remove_priority1[i], *(int*)HeapPeek(my_heap1));		
		PrintArray(my_heap1);  						
		HeapRemove(my_heap1, IsMatch, &priorty1[i]);
	}

	for (i = 0; i < ARR_SIZE2 / 2; ++i)
	{
		(*(int*)HeapPeek(my_heap2) == remove_priority2[i] ? (test2 = 1 * test2) : (test2 = 0));
		printf("In Entry: %ld Expected: %d Got: %d\n", i, remove_priority2[i], *(int*)HeapPeek(my_heap2));		
		PrintArray(my_heap2); 	
		HeapRemove(my_heap2, IsMatch, &priorty2[i]);
	}


	printf("Empty: H1 = %d, H2 = %d\n", HeapIsEmpty(my_heap1), HeapIsEmpty(my_heap2));
	printf("Size: H1 = %ld, H2 = %ld\n", HeapSize(my_heap1), HeapSize(my_heap2));


	RUN_TEST(1 == HeapIsEmpty(my_heap1) && 0 == HeapIsEmpty(my_heap2) ,"Empty Correct After Remove")
	RUN_TEST(0 == HeapSize(my_heap1) && ARR_SIZE2 / 2 == HeapSize(my_heap2),"Heap Is Correct Size After Remove")
	RUN_TEST(test1 && test2 ,"Priority Is Good During Remove")

	HeapDestroy(my_heap1);
	HeapDestroy(my_heap2);	
}







