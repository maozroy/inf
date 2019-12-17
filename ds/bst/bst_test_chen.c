#include <stdio.h> /* printf */
#include <stdlib.h> /* printf */
#include <string.h> /* memcpy */

#include "bst.h"
#include "MyUtils.h" /* MAX2,MIN2 */

#define ARR_LENGTH 20
#define RAND_RANGE 1000
#define NUM_OF_ITERATIONS 20

int MyComparisonFunc(const void *new_data, 
					 const void *src_data, 
					 const void *comp_param);
int QSortCompare(const void *num1, const void *num2);
int MyForEachFunc(void *data, void *for_each_param);
void swap(int *a, int *b);
void randomize(int arr[], int n);

void TestCreateDestroy();
void TestInsertDestroy();
void TestNextPrev();
void TestSizeIsEmpty();
void TestFind();
void TestForEach();
void TestRemove();
void TestCrazy();

int main()
{
	TestCreateDestroy();
	TestInsertDestroy();
	TestNextPrev();
	TestSizeIsEmpty();
	TestFind();
	TestForEach();
	TestRemove();		
	TestCrazy();
	
	return 0;
}

void TestCreateDestroy()
{
	int param = 0;
	
	bst_t *new_bst = BSTCreate(MyComparisonFunc, &param);
	printf("Create bst\n");
	
	BSTDestroy(new_bst);
	printf("Destroy bst\n\n");
}

void TestInsertDestroy()
{
	int param = 0;
	int values[] = {100,150,300,-20,-50,452,465,0,101,102,103};
	
	size_t i = 0;
	
	bst_t *new_bst = BSTCreate(MyComparisonFunc, &param);
	
	for (i = 0; i < sizeof(values)/sizeof(int); ++i)
	{
		PRINTTESTRESULTS("TestInsertDestroy_Insert",i, 
		values[i] == *(int *)BSTGetData(BSTInsert(new_bst, &values[i])));
	}
	
	BSTDestroy(new_bst);
	printf("Destroy bst\n\n");
}

void TestNextPrev()
{
	int param = 0;
	int values[] = {100,150,300,-20,-50,452,465,0,101,102,103};
	
	bst_t *new_bst = BSTCreate(MyComparisonFunc, &param);
	
	bst_iter_t iter4 = BSTInsert(new_bst, &values[4]);
	bst_iter_t iter3 = BSTInsert(new_bst, &values[3]);
	bst_iter_t iter7 = BSTInsert(new_bst, &values[7]);
	bst_iter_t iter0 = BSTInsert(new_bst, &values[0]);
	bst_iter_t iter8 = BSTInsert(new_bst, &values[8]);
	bst_iter_t iter9 = BSTInsert(new_bst, &values[9]);
	bst_iter_t iter10 = BSTInsert(new_bst, &values[10]);
	bst_iter_t iter1 = BSTInsert(new_bst, &values[1]);
	bst_iter_t iter2 = BSTInsert(new_bst, &values[2]);
	bst_iter_t iter5 = BSTInsert(new_bst, &values[5]);
	bst_iter_t iter6 = BSTInsert(new_bst, &values[6]);
	
	PRINTTESTRESULTS("TestNextPrev_next",1, 1 == BSTIsSameIterator(BSTNext(iter4), iter3));
	PRINTTESTRESULTS("TestNextPrev_next",2, 1 == BSTIsSameIterator(BSTNext(iter3), iter7));
	PRINTTESTRESULTS("TestNextPrev_next",3, 1 == BSTIsSameIterator(BSTNext(iter7), iter0));
	PRINTTESTRESULTS("TestNextPrev_next",4, 1 == BSTIsSameIterator(BSTNext(iter0), iter8));
	PRINTTESTRESULTS("TestNextPrev_next",5, 1 == BSTIsSameIterator(BSTNext(iter8), iter9));
	PRINTTESTRESULTS("TestNextPrev_next",6, 1 == BSTIsSameIterator(BSTNext(iter9), iter10));
	PRINTTESTRESULTS("TestNextPrev_next",7, 1 == BSTIsSameIterator(BSTNext(iter10), iter1));
	PRINTTESTRESULTS("TestNextPrev_next",8, 1 == BSTIsSameIterator(BSTNext(iter1), iter2));
	PRINTTESTRESULTS("TestNextPrev_next",9, 1 == BSTIsSameIterator(BSTNext(iter2), iter5));
	PRINTTESTRESULTS("TestNextPrev_next",10, 1 == BSTIsSameIterator(BSTNext(iter5), iter6));
	
	PRINTTESTRESULTS("TestNextPrev_prev",1, 1 == BSTIsSameIterator(BSTPrev(iter3), iter4));
	PRINTTESTRESULTS("TestNextPrev_prev",2, 1 == BSTIsSameIterator(BSTPrev(iter7), iter3));
	PRINTTESTRESULTS("TestNextPrev_prev",3, 1 == BSTIsSameIterator(BSTPrev(iter0), iter7));
	PRINTTESTRESULTS("TestNextPrev_prev",4, 1 == BSTIsSameIterator(BSTPrev(iter8), iter0));
	PRINTTESTRESULTS("TestNextPrev_prev",5, 1 == BSTIsSameIterator(BSTPrev(iter9), iter8));
	PRINTTESTRESULTS("TestNextPrev_prev",6, 1 == BSTIsSameIterator(BSTPrev(iter10), iter9));
	PRINTTESTRESULTS("TestNextPrev_prev",7, 1 == BSTIsSameIterator(BSTPrev(iter1), iter10));
	PRINTTESTRESULTS("TestNextPrev_prev",8, 1 == BSTIsSameIterator(BSTPrev(iter2), iter1));
	PRINTTESTRESULTS("TestNextPrev_prev",9, 1 == BSTIsSameIterator(BSTPrev(iter5), iter2));
	PRINTTESTRESULTS("TestNextPrev_prev",10, 1 == BSTIsSameIterator(BSTPrev(iter6), iter5));
	
	
	BSTDestroy(new_bst);
	
	printf("\n\n");
}

void TestSizeIsEmpty()
{
	int param = 0;
	int values[] = {100,150,300,-20,-50,452,465,0,101,102,103};
	bst_t *new_bst = BSTCreate(MyComparisonFunc, &param);
	size_t i = 0;
	
	for (i = 0; i < sizeof(values) / sizeof(int); ++i)
	{
		if (i == 0)
		{
			PRINTTESTRESULTS("TestSizeISEmpty_IsEmpty",2 * i, 1 == BSTIsEmpty(new_bst));
		}
		else
		{
			PRINTTESTRESULTS("TestSizeISEmpty_IsEmpty",2 * i, 0 == BSTIsEmpty(new_bst));
		}
		
		PRINTTESTRESULTS("TestSize_SizeAndThenInsert",2*(i + 1), i == BSTSize(new_bst));
		BSTInsert(new_bst, &values[i]);
	}
	
	PRINTTESTRESULTS("TestSize_size",i + 1, i == BSTSize(new_bst));
	
	BSTDestroy(new_bst);
	
	printf("\n\n");
}

void TestFind()
{
	int param = 0;
	int values[] = {100,150,300,-20,-50,452,465,0,101,102,103};
	bst_t *new_bst = BSTCreate(MyComparisonFunc, &param);
	size_t i = 0;
	int member_not_to_find = 500;
	
	printf("Insert members into bst\n");
	for (i = 0; i < sizeof(values) / sizeof(int); ++i)
	{
		BSTInsert(new_bst, &values[i]);
	}
	
	for (i = 0; i < sizeof(values) / sizeof(int); ++i)
	{
		PRINTTESTRESULTS("TestFind_Find", i + 1, 
		values[i] == *(int *)BSTGetData(BSTFind(new_bst, &values[i])));
	}
	
	PRINTTESTRESULTS("TestFind_Find", i + 1, 
	BSTIsSameIterator(BSTEnd(new_bst),  BSTFind(new_bst, &member_not_to_find)));
	
	BSTDestroy(new_bst);
	
	printf("\n\n");
}

void TestForEach()
{
	int param = 0;
	int values[] = {100,150,300,-20,-50,452,465,0,101,102,103};
	int values_qsort[] = {100,150,300,-20,-50,452,465,0,101,102,103};
	int values_sorted[] = {-40,-10,10,110,111,112,113,160,310,462,475};
	bst_t *new_bst = BSTCreate(MyComparisonFunc, &param);
	size_t i = 0;
	int for_each_param = 10;
	bst_iter_t runner = NULL;
	
	printf("Insert members into bst\n");
	for (i = 0; i < sizeof(values) / sizeof(int); ++i)
	{
		BSTInsert(new_bst, &values[i]);
	}

	qsort(values_qsort, sizeof(values_qsort)/sizeof(int),
		   	   sizeof(int), QSortCompare);
	
	PRINTTESTRESULTS("TestForEach_ForEach", 1, 
	0 == BSTForEach(BSTBegin(new_bst), BSTEnd(new_bst), 
					MyForEachFunc, &for_each_param));
	
	i = 0;					
	for (runner = BSTBegin(new_bst); 
		 !BSTIsSameIterator(runner, BSTEnd(new_bst));
		 runner = BSTNext(runner))
	{
		PRINTTESTRESULTS("TestForEach_GetData", i + 1, 
		values_sorted[i] == *(int *)BSTGetData(runner));
		++i;
	}
	
	BSTDestroy(new_bst);
	
	printf("\n\n");
}

void TestRemove()
{
	int param = 0;
	int values[] = {100,150,300,-20,-50,452,465,0,101,102,103};
	int values_sorted[] = {-50,-20,0,100,101,102,103,150,300,452,465};
	int value_removed1 = 465;
	int values_after_remove1[] = {-50,-20,0,100,101,102,103,150,300,452};
	int value_removed2 = -50;
	int values_after_remove2[] = {-20,0,100,101,102,103,150,300,452};
	int value_removed3 = 101;
	int values_after_remove3[] = {-20,0,100,102,103,150,300,452};
	bst_t *new_bst = BSTCreate(MyComparisonFunc, &param);
	size_t i = 0;
	size_t j = 0;
	bst_iter_t removed_iter = NULL;
	bst_iter_t runner = NULL;
	
	printf("Insert members into bst\n");
	for (i = 0; i < sizeof(values) / sizeof(int); ++i)
	{
		BSTInsert(new_bst, &values[i]);
	}

	printf("Remove %d from bst\n", value_removed1);
	BSTRemove(BSTFind(new_bst, &value_removed1));

	for (runner = BSTBegin(new_bst); 
	 !BSTIsSameIterator(runner, BSTEnd(new_bst));
	 runner = BSTNext(runner))
	{
		PRINTTESTRESULTS("TestRemove_GetData", j + 1, 
		values_after_remove1[j] == *(int *)BSTGetData(runner));
		++j;
	}
	
	printf("Remove %d from bst\n", value_removed2);
	BSTRemove(BSTFind(new_bst, &value_removed2));

	j = 0;
	for (runner = BSTBegin(new_bst); 
	 !BSTIsSameIterator(runner, BSTEnd(new_bst));
	 runner = BSTNext(runner))
	{
		PRINTTESTRESULTS("TestRemove_GetData", j + 1, 
		values_after_remove2[j] == *(int *)BSTGetData(runner));
		++j;
	}
	
	printf("Remove %d from bst\n", value_removed3);
	BSTRemove(BSTFind(new_bst, &value_removed3));

	j = 0;
	for (runner = BSTBegin(new_bst); 
	 !BSTIsSameIterator(runner, BSTEnd(new_bst));
	 runner = BSTNext(runner))
	{
		PRINTTESTRESULTS("TestRemove_GetData", j + 1, 
		values_after_remove3[j] == *(int *)BSTGetData(runner));
		++j;
	}
	
	BSTDestroy(new_bst);
	
	printf("\n\n");
}

void TestCrazy()
{
	int param = 0;
	int values[ARR_LENGTH] = {0};
	int values_copy[ARR_LENGTH] = {0};
	int values_after_qsort[ARR_LENGTH] = {0};
	int index_of_values_in_qsort_arr[ARR_LENGTH] = {0};
	int i = 0;
	int j = 0;
	int k = 0;
	int temp = 0;
	int status = 1;
	bst_iter_t runner = NULL;
	bst_t *new_bst = NULL;
	
	for (k = 0; k < NUM_OF_ITERATIONS ; ++k)
	{
		sleep(1);	
		new_bst = BSTCreate(MyComparisonFunc, &param);
		srand(time(NULL));
		for (i = 0; i < ARR_LENGTH; ++i)
		{
			temp = rand()%RAND_RANGE - RAND_RANGE/2;
			values[i] = temp;
			values_after_qsort[i] = temp;
		}
		
		memcpy (values_copy, values, ARR_LENGTH * sizeof(int));
		printf("\n\nInsert to bst\n");
		for (i = 0; i < ARR_LENGTH; ++i)
		{
			BSTInsert(new_bst, &values_copy[i]);
		}
		
		randomize(values, ARR_LENGTH);
		for (i = 0 ; i < ARR_LENGTH ; i++)
		{
		printf("%d ,",values[i]);
		}
		qsort(values_after_qsort, sizeof(values)/sizeof(int), 
						  		  sizeof(int), QSortCompare);
		printf("\n");
		/* finding indice */
		for (i = 0; i < ARR_LENGTH; ++i)
		{
			for (j = 0; j < ARR_LENGTH; ++j)
			{
				if (values[i] == values_after_qsort[j])
				{
					index_of_values_in_qsort_arr[i] = j;
					break;
				}
			}
		}
		
		j = 0;
		PRINTTESTRESULTS("TestCrazy_size",1, ARR_LENGTH == BSTSize(new_bst));
		printf("Remove %d from bst\n", values[i]);
		BSTRemove(BSTFind(new_bst, &values[0]));
		PRINTTESTRESULTS("TestCrazy_size",2, ARR_LENGTH - 1 == BSTSize(new_bst));
		
		i = 0;
		for (runner = BSTBegin(new_bst); 
		 !BSTIsSameIterator(runner, BSTEnd(new_bst));
		 runner = BSTNext(runner))
		{
			if (j == index_of_values_in_qsort_arr[0])
			{
				j += 1;
			}
			status |= (values_after_qsort[j] == *(int *)BSTGetData(runner));
			/*PRINTTESTRESULTS("TestCrazy_GetData", i + 1, 
			values_after_qsort[j] == *(int *)BSTGetData(runner));*/
			++j;
			++i;
		}
		
		PRINTTESTRESULTS("TestCrazy_Remove", i + 1, status);

		BSTDestroy(new_bst);
	}
	

	printf("\n\n");
}

int MyComparisonFunc(const void *new_data, 
					 const void *src_data, 
					 const void *comp_param)
{
	(void)comp_param;
	
	if (*(int *)new_data > *(int *)src_data)
	{
		return 1;
	}
	else if (*(int *)src_data > *(int *)new_data)
	{
		return -1;
	}
	
	return 0;
}

int MyForEachFunc(void *data, void *for_each_param)
{	
	*(int *)data += *(int *)for_each_param;

	return 0;
}
							   
int QSortCompare(const void *num1, const void *num2)
{
	return (*(int *)num1 - *(int *)num2);
}

void swap(int *a, int *b) 
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void randomize(int arr[], int n) 
{
    int i = 0;
    
    srand(time(NULL));
    for(i = n-1; i > 0; i--) 
    {
        int j = rand() % (i+1);
        swap(&arr[i], &arr[j]);
    }
}



















