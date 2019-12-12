#include <stdlib.h> /* rand */
#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <stdio.h> /* printf puts */
#include <string.h> /* qsort */
#include <limits.h> /* INT_MIN INT_MAX */
#include "sorting.h"

#define ARR_SIZE (12)
#define SHUFFLE_TIMES (1)

void BubbleSortTest();
void BubbleSortVsQsort();
void InsertionSortTest();
void SelectionSortTest();
void QSortTest();
void CountingSortTest();
void RadixSortTest();

static int IsSorted(int *arr, size_t size);
static void PrintArr(int *arr, size_t size);
static void ShuffleArray(int *arr, size_t size);
static void SwapElements(int *elem1, int *elem2);
static int CmpFunc(const void *elem1, const void *elem2);
static int GetMinVal(int *arr, size_t size);
static int GetMaxVal(int *arr, size_t size);


int main()
{
  /*  BubbleSortVsQsort();
    BubbleSortTest();
    InsertionSortTest();*/
    SelectionSortTest();
  /*  QSortTest();*/
/*     CountingSortTest();
    RadixSortTest(); */

    return 0;
}

void BubbleSortTest()
{
    int arr[ARR_SIZE] = {0};
    size_t i = 0;
    size_t j = 0;

    for (j = 0; j < SHUFFLE_TIMES; ++j)
    {
        ShuffleArray(arr, ARR_SIZE);
        for (i = 0; i < ARR_SIZE; ++i)
        {
            arr[i] = rand()%100;
        }
        
        BubbleSort(arr, ARR_SIZE);

        assert(IsSorted(arr, ARR_SIZE));
    }
}

void InsertionSortTest()
{
    int arr[ARR_SIZE] = {0};
    size_t i = 0;
    size_t j = 0;

    for (j = 0; j < SHUFFLE_TIMES; ++j)
    {
        ShuffleArray(arr, ARR_SIZE);
        for (i = 0; i < ARR_SIZE; ++i)
        {
            arr[i] = rand()%100;
        }
 
        InsertionSort(arr, ARR_SIZE);

        assert(IsSorted(arr, ARR_SIZE));
    }    
}

void SelectionSortTest()
{
    int arr[ARR_SIZE] = {0};
    size_t i = 0;
    size_t j = 0;

    for (j = 0; j < SHUFFLE_TIMES; ++j)
    {
        ShuffleArray(arr, ARR_SIZE);
        for (i = 0; i < ARR_SIZE; ++i)
        {
            arr[i] = rand()%100;
        }
        PrintArr(arr, ARR_SIZE);
        SelectionSort(arr, ARR_SIZE);
        PrintArr(arr, ARR_SIZE);
        assert(IsSorted(arr, ARR_SIZE));
    }   
}

void QSortTest()
{
    int arr[ARR_SIZE] = {0};
    size_t i = 0;
    size_t j = 0;

    for (j = 0; j < SHUFFLE_TIMES; ++j)
    {
        ShuffleArray(arr, ARR_SIZE);
        for (i = 0; i < ARR_SIZE; ++i)
        {
            arr[i] = rand()%100;
        }
        
        qsort(arr, ARR_SIZE, sizeof(int), &CmpFunc);

        assert(IsSorted(arr, ARR_SIZE));
    }   
}
/*
void CountingSortTest()
{
    int arr[ARR_SIZE] = {0};
    int min_val = 0;
    int max_val = 0;
    size_t i = 0;
    size_t j = 0;

    int *result = (int *)malloc(ARR_SIZE * sizeof(int));

    for (j = 0; j < SHUFFLE_TIMES; ++j)
    {
        ShuffleArray(arr, ARR_SIZE);
        for (i = 0; i < ARR_SIZE; ++i)
        {
            arr[i] = (rand()%100) - 50;
        }

        min_val = GetMinVal(arr, ARR_SIZE);
        max_val = GetMaxVal(arr, ARR_SIZE);

        CountingSort(arr, ARR_SIZE, min_val, max_val, result);

        assert(IsSorted(result, ARR_SIZE));
     }

    free(result);   
}

void RadixSortTest()
{
     int arr[ARR_SIZE] = {0};
    size_t i = 0;
    size_t j = 0;

    for (j = 0; j < SHUFFLE_TIMES; ++j)
    {
        ShuffleArray(arr, ARR_SIZE);
        for (i = 0; i < ARR_SIZE; ++i)
        {
            arr[i] = (rand()%100) - 50;
        }

        PrintArr(arr, ARR_SIZE);
        RadixSort(arr, ARR_SIZE);
        PrintArr(arr, ARR_SIZE);
        assert(IsSorted(arr, ARR_SIZE));
     }   
}
*/
static int GetMinVal(int *arr, size_t size)
{
    size_t i = 0;
    int min_val = INT_MAX;

    for (i = 0; i < size; ++i)
    {
        if (min_val > arr[i])
        {
            min_val = arr[i];
        }
    }

    return min_val;
}

static int GetMaxVal(int *arr, size_t size)
{
    size_t i = 0;
    int max_val = INT_MIN;

    for (i = 0; i < size; ++i)
    {
        if (max_val < arr[i])
        {
            max_val = arr[i];
        }
    }

    return max_val;
}

void BubbleSortVsQsort()
{
    int arr[ARR_SIZE] = {0};
    size_t i = 0;

    int *arr_q_sort = (int *)malloc(ARR_SIZE * sizeof(int));

    for (i = 0; i < ARR_SIZE; ++i)
    {
        arr[i] = rand()%100;
    }
    
    memcpy(arr_q_sort, arr, ARR_SIZE * sizeof(int));
    BubbleSort(arr, ARR_SIZE);
    qsort(arr_q_sort, ARR_SIZE, sizeof(int), &CmpFunc);
    
    assert(0 == memcmp(arr_q_sort, arr, ARR_SIZE * sizeof(int)));

    free(arr_q_sort);
}

static void PrintArr(int *arr, size_t size)
{
    size_t i = 0;

    for (i = 0; i < size; ++i)
    {
        printf("%d ", arr[i]);
    }

    puts("\n");
}

static int IsSorted(int *arr, size_t size)
{
    size_t i = 0;
    int is_sorted = 1;

    for (i = 0; (i < size - 1) && (is_sorted); ++i)
    {
        is_sorted = (arr[i] <= arr[i + 1]);
    }

    return is_sorted;
}

static void ShuffleArray(int *arr, size_t size)
{
    size_t i = 0;
    size_t rand_i = 0;

    for (i = 0; i < size; ++i)
    {
        if (size > 1)
        {
            rand_i = rand()%(size - 1);
        }

        SwapElements(&arr[i], &arr[rand_i]);
    }
}

static void SwapElements(int *elem1, int *elem2)
{
    int temp = 0;

    assert(NULL != elem1);
    assert(NULL != elem2);

    temp = *elem1;
    *elem1 = *elem2;
    *elem2 = temp;
}

static int CmpFunc(const void *elem1, const void *elem2)
{
    return ((*(int *)elem1) - (*(int *)elem2));
}
