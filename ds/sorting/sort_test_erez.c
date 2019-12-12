/****************************************************************************
*                                                                           *
*                               OL79 - 01/12/19                             *
*                               Auther - Erez                               *	
*                               Memrory Locations 	                        *
*																			*
*****************************************************************************/


#include <stdlib.h> /* malloc */
#include <stdio.h> /* printf */
#include <time.h> /*clock*/


#define SIZEOFARRAY 200
#define ITERATIONS 200

static void ShortTest();
static void LongTest();
static void TimeTestQueueSort();
static void TimeTestBubbleSort();
static void TimeTestInsertionSort();
static void TimeTestSelectionSort();
int CompareFunc(const void *x, const void *y);
/*************************************************/
int main ()
{
	ShortTest(); 
	LongTest(); 	
	TimeTestQueueSort();
	TimeTestBubbleSort();
	TimeTestInsertionSort();
	TimeTestSelectionSort();
	
	return (0);
}
/*************/
static void ShortTest()
{
	int arr1[] = {9,8,7,6,5,4,3,2,1,0};
	int arr2[] = {8,9,5,6,7,1,2,3,0,4};
	int arr3[] = {9,1,2,3,4,5,6,7,8,0};
	int arr4[] = {9,8,7,6,5,4,3,2,1,0};
	int arr5[] = {8,9,5,6,7,1,2,3,0,4};
	int arr6[] = {9,1,2,3,4,5,6,7,8,0};
	int arr7[] = {9,8,7,6,5,4,3,2,1,0};
	int arr8[] = {8,9,5,6,7,1,2,3,0,4};
	int arr9[] = {9,1,2,3,4,5,6,7,8,0};
	int sort[] = {0,1,2,3,4,5,6,7,8,9};
	size_t i = 0;
	int test_ok = 1;
	
	BubbleSort(arr1, 10);
	BubbleSort(arr2, 10);	
	BubbleSort(arr3, 10);	

	for (i = 0; i < 10; ++i)
	{
		if (*(sort + i) != *(arr1 + i) || *(sort + i) != *(arr2 + i)|| *(sort + i) != *(arr3 + i))
		{
			test_ok = 0;	
		}
	}

	RUN_TEST(test_ok, "Bubble Sort");

	InsertionSort(arr4, 10);
	InsertionSort(arr5, 10);	
	InsertionSort(arr6, 10);

	for (i = 0; i < 10; ++i)
	{
		if (*(sort + i) != *(arr4 + i) || *(sort + i) != *(arr5 + i)|| *(sort + i) != *(arr6 + i))
		{
			test_ok = 0;	
		}
	}

	RUN_TEST(test_ok, "Insertion Sort");

	SelectionSort(arr7, 10);
	SelectionSort(arr8, 10);	
	SelectionSort(arr9, 10);

	for (i = 0; i < 10; ++i)
	{
		if (*(sort + i) != *(arr7 + i) || *(sort + i) != *(arr8 + i)|| *(sort + i) != *(arr9 + i))
		{
			test_ok = 0;	
		}
	}

	RUN_TEST(test_ok, "Selection Sort");
}
/*************/
static void LongTest()
{
	int *my_array = (int*)malloc(SIZEOFARRAY);
	int *qs_array = (int*)malloc(SIZEOFARRAY);
	int i = 0;
	int j = 0;	
	int bubble_ok = 1;
	int insert_ok = 1;	
	int select_ok = 1;
		
	for (i = 0; i < ITERATIONS && 1 == insert_ok; ++i)
	{	
		for (j = 0; j < SIZEOFARRAY; ++ j)
		{
			*(my_array + j) = rand() % RAND_MAX;
			*(qs_array + j) = *(my_array + j) ;		
		}
	
			BubbleSort(my_array, SIZEOFARRAY);			
			qsort(qs_array, SIZEOFARRAY, sizeof(int), CompareFunc);			
	
		for (j = 0; j < SIZEOFARRAY; ++ j)
		{
			if (*(qs_array + j) != *(my_array + j))
			{
				bubble_ok = 0;
				break;
			}
		}
	}

	RUN_TEST(bubble_ok, "Bubble Sort - Long");

	for (i = 0; i < ITERATIONS && 1 == insert_ok; ++i)
	{	
		for (j = 0; j < SIZEOFARRAY; ++ j)
		{
			*(my_array + j) = rand() % RAND_MAX;
			*(qs_array + j) = *(my_array + j) ;		
		}
	
			InsertionSort(my_array, SIZEOFARRAY);			
			qsort(qs_array, SIZEOFARRAY, sizeof(int), CompareFunc);			
	
		for (j = 0; j < SIZEOFARRAY; ++ j)
		{
			if (*(qs_array + j) != *(my_array + j))
			{
				bubble_ok = 0;
				break;
			}
		}
	}

	RUN_TEST(insert_ok, "Insertion Sort - Long");
	
	for (i = 0; i < ITERATIONS && 1 == select_ok; ++i)
	{	
		for (j = 0; j < SIZEOFARRAY; ++ j)
		{
			*(my_array + j) = rand() % RAND_MAX;
			*(qs_array + j) = *(my_array + j) ;		
		}
	
			SelectionSort(my_array, SIZEOFARRAY);			
			qsort(qs_array, SIZEOFARRAY, sizeof(int), CompareFunc);			
	
		for (j = 0; j < SIZEOFARRAY; ++ j)
		{
			if (*(qs_array + j) != *(my_array + j))
			{
				bubble_ok = 0;
				break;
			}
		}
	}

	RUN_TEST(select_ok, "Selection Sort - Long")
	
	free(my_array);
/*	free(qs_array);*/
}
/*************/
static void TimeTestBubbleSort()
{
	int *array = (int*)malloc(SIZEOFARRAY / 2);
	int i = 0;
	int j = 0;
	clock_t time = clock();
	double how_much_time = 0;
	
	for (i = 0; i < ITERATIONS; ++ i) 
	{
		for (j = 0; j < SIZEOFARRAY; ++ j)
		{
			*(array + j) = rand() % RAND_MAX;
		}
		
		BubbleSort(array, SIZEOFARRAY);			
	}

	how_much_time = (double)(clock() - time) / CLOCKS_PER_SEC;
	
	printf("DONE %d BUBBLES SORT FOR %d ARRAYS IN %f ms \n", 
				ITERATIONS, SIZEOFARRAY, how_much_time * 1000);
	free(array);
}
/*************/
static void TimeTestInsertionSort()
{
	int *array = (int*)malloc(SIZEOFARRAY / 2);
	int i = 0;
	int j = 0;
	clock_t time = clock();
	double how_much_time = 0;

	for (i = 0; i < ITERATIONS; ++ i) 
	{
		for (j = 0; j < SIZEOFARRAY; ++ j)
		{
			*(array + j) = rand() % RAND_MAX;
		}
		
		InsertionSort(array, SIZEOFARRAY);			
	}

	how_much_time = (double)(clock() - time) / CLOCKS_PER_SEC;

	printf("DONE %d INSERTIONS SORT FOR %d ARRAYS IN %f ms \n", 
				ITERATIONS, SIZEOFARRAY, how_much_time * 1000);
	free(array);
}
/*************/
static void TimeTestSelectionSort()
{
	int *array = (int*)malloc(SIZEOFARRAY / 2);
	int i = 0;
	int j = 0;
	clock_t time = clock();
	double how_much_time = 0;
		
	for (i = 0; i < ITERATIONS; ++ i) 
	{
		for (j = 0; j < SIZEOFARRAY; ++ j)
		{
			*(array + j) = rand() % RAND_MAX;
		}
		
		SelectionSort(array, SIZEOFARRAY);			
	}

	how_much_time = (double)(clock() - time) / CLOCKS_PER_SEC;
	
	printf("DONE %d SELECTION SORT FOR %d ARRAYS IN %f ms \n", 
				ITERATIONS, SIZEOFARRAY, how_much_time * 1000);
	free(array);
}
static void TimeTestQueueSort()
{
	int *array = (int*)malloc(SIZEOFARRAY / 2);
	int i = 0;
	int j = 0;
	clock_t time = clock();
	double how_much_time = 0;
		
	for (i = 0; i < ITERATIONS; ++ i) 
	{
		for (j = 0; j < SIZEOFARRAY; ++ j)
		{
			*(array + j) = rand() % RAND_MAX;
		}
		
		qsort(array, SIZEOFARRAY, sizeof(int), CompareFunc);			
	}

	how_much_time = (double)(clock() - time) / CLOCKS_PER_SEC;
	
	printf("DONE %d QSORT FOR %d ARRAYS IN %f ms \n",
				 ITERATIONS, SIZEOFARRAY, how_much_time * 1000);
	free(array);
}
int CompareFunc(const void *x, const void *y)
{
	return (*(int*)x > *(int*)y);
}














