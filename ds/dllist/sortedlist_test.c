#include <stdio.h>

#include "dllist.h"
#include "sortedlist.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define PRINTTESTRESULTS(description,num, res) \
(printf("%-40s Test %d %s\n",description, num, ((res) == 1) ? KGRN "passed"\
 KNRM : KRED "failed" KNRM))\

#define TESTARRAY 5
void TestSrtListCreate(void);
void TestDLListPushBack(void);
void TestDLListPushFront(void);
void TestDLListInsert(void);
void TestDLListPopFront(void);
void TestDLListPopBack(void);
void TestDLListRemove(void);
void TestDLListSplice(void);
int AddToNode(dll_node_t *node, void *param);
int IntFindFunc(const void *data, const void *param);
void PrintIntList(dl_list_t *mylist);
void TestDLListForEach(void);
void TestDLListFind(void);
void TestSrtListInsert(void);
void TestSrtListPopFront(void);
void TestSrtListPopBack(void);
void TestSrtListRemove(void);
void TestSrtListMerge(void);
void TestSrtListFindIf(void);

int MyIntAlgo(const void *data1, const void *data2, void *param);

int main()
{

	TestSrtListCreate();
	TestSrtListInsert();
	TestSrtListPopFront();
	TestSrtListPopBack();
	TestSrtListRemove();
	TestSrtListMerge();
	TestSrtListFindIf();
	/*TestDLListPushBack();
	TestDLListPushFront();
	TestDLListInsert();
	TestDLListPopFront();
	TestDLListPopBack();
	TestDLListRemove();
	TestDLListSplice();
	TestDLListForEach();
	TestDLListFind();*/
	return 0;
}

int MyIntAlgo(const void *data1, const void *data2, void *param)
{
	(void)param;
	if ( (*(int*)data1) < (*(int*)data2) )
	{
		return 1;
	}
	
	return 0;
}

int IntFindFunc(const void *data, const void *param)
{
	int int_data = *(int*)data;
	int int_param = *(int*)param;
	
	return (int_data == int_param);
}

void TestSrtListCreate(void)
{
	srt_list_t *my_list = SrtListCreate(NULL, MyIntAlgo);
	SrtListDestroy(my_list);
	
}

void TestSrtListFindIf(void)
{
	int arr[] = {15, 234, 123, 657, 11, 0, 4};
	size_t i = 0;
	int number = 234;
	srt_list_t *my_list = SrtListCreate(NULL, MyIntAlgo);
	srt_iter_t my_iter = {0};
	
	for (i = 0 ; i < 7 ; i++)
	{
		my_iter = SrtListInsert( &arr[i], my_list);
	}
	
	/*my_iter = SrtListFindIf(SrtListBegin(my_list), SrtListEnd(my_list), 
						 &number, IntFindFunc);
	PRINTTESTRESULTS("FindIf",1 ,SrtListIsSameIterator(my_iter, 
					SrtListNext(SrtListBegin(my_list))));
	*/
	SrtListDestroy(my_list);
}

void TestSrtListInsert(void)
{
	int arr[] = {15, 234, 123, 657, 11, 0, 4};
	size_t i = 0;
	srt_list_t *my_list = SrtListCreate(NULL, MyIntAlgo);
	srt_iter_t my_iter = {0};
	
	for (i = 0 ; i < 7 ; i++)
	{
		my_iter = SrtListInsert( &arr[i], my_list);
	}
	PRINTTESTRESULTS("Insert Return", 1, 
			*(int*)SrtListGetData(my_iter) == 4);
	PRINTTESTRESULTS("Insert First", 1, 
			*(int*)SrtListGetData((SrtListBegin(my_list))) == 657);
	PRINTTESTRESULTS("Insert Last", 1, 
			*(int*)SrtListGetData(SrtListPrev(SrtListEnd(my_list))) == 0);
	
	
	SrtListDestroy(my_list);
}

void TestSrtListPopFront(void)
{
	int arr[] = {15, 234, 123, 657, 11, 0, 4};
	int sorted_arr[] = {657, 234, 123, 15, 11, 4, 0};
	size_t i = 0;
	int res_counter = 0;
	
	srt_list_t *my_list = SrtListCreate(NULL, MyIntAlgo);
	
	for (i = 0 ; i < 7 ; i++)
	{
		SrtListInsert(&arr[i], my_list);
	}
	for (i = 0 ; i < 7 ; i++)
	{
		if(sorted_arr[i] != *(int*)SrtListPopFront(my_list))
		{
			res_counter++;

		}
		
		PRINTTESTRESULTS("PopFront Size", 1, SrtListSize(my_list) == 6 - i);
	}
	PRINTTESTRESULTS("PopFront test", 1, res_counter == 0);
	
	SrtListDestroy(my_list);
}

void TestSrtListPopBack(void)
{
	int arr[] = {15, 234, 123, 657, 11, 0, 4};
	int sorted_arr[] = {657, 234, 123, 15, 11, 4, 0};
	size_t i = 0;
	int res_counter = 0;
	
	srt_list_t *my_list = SrtListCreate(NULL, MyIntAlgo);
	
	for (i = 0 ; i < 7 ; i++)
	{
		SrtListInsert(&arr[i], my_list);
	}
	for (i = 0 ; i < 7 ; i++)
	{
		if(sorted_arr[6 - i] != *(int*)SrtListPopBack(my_list))
		{
			res_counter++;

		}
		
		PRINTTESTRESULTS("PopBack Size", 1, SrtListSize(my_list) == 6 - i);
	}
	PRINTTESTRESULTS("PopBack test", 1, res_counter == 0);
	
	SrtListDestroy(my_list);
}

void TestSrtListRemove(void)
{
	int arr[] = {15, 234, 123, 657, 11, 0, 4};
	int sorted_arr[] = {657, 234, 123, 15, 11, 4, 0};
	size_t i = 0;
	srt_iter_t my_iter = {0};
	srt_list_t *my_list = SrtListCreate(NULL, MyIntAlgo);
	
	for (i = 0 ; i < 7 ; i++)
	{
		SrtListInsert(&arr[i], my_list);
	}
	for (i = 0 ; i < 6 ; i++)
	{
		
		my_iter = SrtListRemove(SrtListBegin(my_list));
		PRINTTESTRESULTS("Remove Return", 1, 
		*(int*)SrtListGetData(my_iter) == sorted_arr[i + 1]);
	}
	PRINTTESTRESULTS("Remove Size", 1, SrtListSize(my_list) == 1);
	
	SrtListDestroy(my_list);
}

void TestSrtListMerge(void)
{
	int arr[] = {15, 234, 123, 657, 11, 0, 4};
	int arr2[] = {6157, 34, 1323, 1, 13, 5, 0};
	int sorted_arr[] = {6157, 1323, 657, 234, 123, 34, 15, 13, 11, 5, 4, 1, 0, 0};
	size_t i = 0;
	srt_iter_t my_iter = {0};
	srt_list_t *my_list = SrtListCreate(NULL, MyIntAlgo);
	srt_list_t *my_list2 = SrtListCreate(NULL, MyIntAlgo);
	
	for (i = 0 ; i < 7 ; i++)
	{
		SrtListInsert(&arr[i], my_list);
	}
	
	for (i = 0 ; i < 7 ; i++)
	{
		SrtListInsert(&arr2[i], my_list2);
	}
	SrtListMerge(my_list2, my_list);
	PRINTTESTRESULTS("Merge Size", 1, SrtListSize(my_list) == 14);
	
	for (my_iter = SrtListBegin(my_list), i = 0 ;
		!(SrtListIsSameIterator(my_iter, SrtListEnd(my_list))) ; 
		my_iter = SrtListNext(my_iter), i++ )
	{
		PRINTTESTRESULTS("Merge compare", i,
			*(int*)SrtListGetData(my_iter) == sorted_arr[i]);
	}
	SrtListDestroy(my_list);
}
/*

void TestDLListPushBack(void)
{

	size_t i = 0;
	int arr[TESTARRAY] = {0, 1, 2, 3, 4};
	dl_list_t *my_list = DLListCreate();
	
	for(i = 0 ; i < TESTARRAY ; i++)
	{

		DLListPushBack(my_list, arr+i);
			PRINTTESTRESULTS("PushBack - last node equal to data pushed", 1, 
			((DLListGetData(DLListPrev(DLListEnd(my_list))))) == (arr+i) );
	}

	DLListDestroy(my_list);
}

void TestDLListPushFront(void)
{

	size_t i = 0;
	int arr[TESTARRAY] = {0, 1, 2, 3, 4};
	dl_list_t *my_list = DLListCreate();
	
	for(i = 0 ; i < TESTARRAY ; i++)
	{

		DLListPushFront(my_list, arr+i);
			PRINTTESTRESULTS("PushFront - begin node equal to data pushed", 1, 
			((DLListGetData((DLListBegin(my_list))))) == (arr+i) );
	}

	DLListDestroy(my_list);
}


void TestDLListInsert(void)
{

	size_t i = 0;
	dll_iter_t iter = NULL;
	int arr[TESTARRAY] = {0, 1, 2, 3, 4};
	dl_list_t *my_list = DLListCreate();
	iter = DLListPushFront(my_list, arr+i);
	for(i = 1 ; i < TESTARRAY ; i++)
	{

		iter = DLListInsert(arr+i, iter, my_list);
			PRINTTESTRESULTS("Insert - begin node equal to data pushed", 1, 
			((DLListGetData(iter))) == (arr+i) );
	}

	DLListDestroy(my_list);
}

void *DLListPopFront(dl_list_t *list);

void TestDLListPopFront(void)
{

	size_t i = 0;
	int *ptr = NULL;
	int arr[TESTARRAY] = {0, 1, 2, 3, 4};
	dl_list_t *my_list = DLListCreate();
	for(i = 0 ; i < TESTARRAY ; i++)
	{

		DLListPushBack(my_list, arr+i);

	}
	for(i = 0 ; i < TESTARRAY ; i++)
	{
		ptr = DLListPopFront(my_list);
			PRINTTESTRESULTS("PopFront", 1, 
			*(ptr) == *(arr+i) );
	}

	

	DLListDestroy(my_list);
}



void TestDLListPopBack(void)
{

	size_t i = 0;
	int *ptr = NULL;
	int arr[TESTARRAY] = {0, 1, 2, 3, 4};
	dl_list_t *my_list = DLListCreate();
	for(i = 0 ; i < TESTARRAY ; i++)
	{

		DLListPushBack(my_list, arr+i);

	}
	for(i = 0 ; i < TESTARRAY ; i++)
	{
		ptr = DLListPopBack(my_list);
			PRINTTESTRESULTS("PopBack", 1, 
			*(ptr) == *(arr + TESTARRAY - i - 1) );
	}

	

	DLListDestroy(my_list);
}

void TestDLListRemove(void)
{

	size_t i = 0;
	dll_iter_t iter = NULL;
	int arr[TESTARRAY] = {0, 1, 2, 3, 4};
	dl_list_t *my_list = DLListCreate();
	for(i = 0 ; i < TESTARRAY ; i++)
	{

		DLListPushBack(my_list, arr+i);

	}
	iter = DLListBegin(my_list);
	for(i = 1 ; i < TESTARRAY ; i++)
	{
		iter = DLListRemove(iter);
			PRINTTESTRESULTS("Remove", 1, 
			*(int*)DLListGetData(iter) == *(arr + i) );
	}
		iter = DLListRemove(iter);
			PRINTTESTRESULTS("Remove", 2, 
			DLListNext(iter) == NULL);
	

	DLListDestroy(my_list);
}

void TestDLListSplice(void)
{

	size_t i = 0;
	dll_iter_t iter = NULL;
	int arr[TESTARRAY] = {0, 1, 2, 3, 4};
	int arr2[TESTARRAY] = {5, 6, 7, 8, 9};
	dl_list_t *my_list = DLListCreate();
	dl_list_t *my_list2 = DLListCreate();
	for(i = 0 ; i < TESTARRAY ; i++)
	{
		DLListPushBack(my_list, arr+i);
	}
	
	for(i = 0 ; i < TESTARRAY ; i++)
	{
		DLListPushBack(my_list2, arr2+i);
	}
	
	iter = DLListSplice(DLListNext(DLListBegin(my_list2)),
				DLListNext(DLListNext(DLListNext(DLListBegin(my_list2)))),
				DLListNext(DLListBegin(my_list)));
	PRINTTESTRESULTS("Splice - return value", 1, 
			*(int*)DLListGetData(iter) == *(arr + 1) );		
	PRINTTESTRESULTS("Splice - size test", 1, 
			DLListSize(my_list) == 7);		
		PRINTTESTRESULTS("Splice - size test", 2, 
			DLListSize(my_list2) == 3);		
		PRINTTESTRESULTS("Splice", 2, 
			*(int*)DLListGetData(DLListNext(iter)) == *(arr2 + 1));	
		PRINTTESTRESULTS("Splice", 2, 
			*(int*)DLListGetData(DLListNext(DLListBegin(my_list2))) 
				== *(arr2 + 3));
							

	
	DLListDestroy(my_list2);
	DLListDestroy(my_list);
}

int AddToNode(dll_node_t *node, void *param)
{
	int *holder = NULL;
	if (node == NULL)
	{
		return 1;
	}
	holder = DLListGetData(node);
	*(int*)holder += *(int*)param;

	return 0;
}

int FindInt (const dll_node_t *node, const void *param)
{
	if(*(int*)DLListGetData((void*)node) == *(int*)param)
	{
		return 1;
	}
	return 0;
}


void TestDLListForEach(void)
{
	size_t i = 0;
	dll_iter_t iter = NULL;
	int arr[TESTARRAY] = {0, 1, 2, 3, 4};
	dl_list_t *my_list = DLListCreate();
	for(i = 0 ; i < TESTARRAY ; i++)
	{
		DLListPushBack(my_list, arr+i);
	}
	DLListForEach(DLListBegin(my_list), DLListEnd(my_list), &i, AddToNode);
	for(i = 0 ; i < TESTARRAY ; i++)
	{
			PRINTTESTRESULTS("ForEach", 1, (*(arr+i) == 5 + i) );
	}
	DLListDestroy(my_list);
}

void TestDLListFind(void)
{
	size_t i = 0;
	int loc = 3;
	dll_iter_t iter = NULL;
	int arr[TESTARRAY] = {0, 1, 2, 3, 4};
	dl_list_t *my_list = DLListCreate();
	for(i = 0 ; i < TESTARRAY ; i++)
	{
		DLListPushBack(my_list, arr+i);
	}
	iter = DLListFind(DLListBegin(my_list), DLListEnd(my_list), &loc, FindInt);
		PRINTTESTRESULTS("Find", 1, DLListGetData(iter) == (arr+3) );

	DLListDestroy(my_list);
}

*/


