
#include "dllist.h"
#include <stdio.h>

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
void TestDLListCreate(void);
void TestDLListPushBack(void);
void TestDLListPushFront(void);
void TestDLListInsert(void);
void TestDLListPopFront(void);
void TestDLListPopBack(void);
void TestDLListRemove(void);
void TestDLListSplice(void);
int AddToNode(dll_node_t *node, void *param);
void PrintIntList(dl_list_t *mylist);
void TestDLListForEach(void);
void TestDLListFind(void);

int main()
{

	TestDLListCreate();
	TestDLListPushBack();
	TestDLListPushFront();
	TestDLListInsert();
	TestDLListPopFront();
	TestDLListPopBack();
	TestDLListRemove();
	TestDLListSplice();
	TestDLListForEach();
	TestDLListFind();
	return 0;
}

void TestDLListCreate(void)
{
	dl_list_t *my_list = DLListCreate();
	
	PRINTTESTRESULTS("Create - begin->next is last", 1, 
			DLListIsSameIterator((((DLListBegin(my_list)))), 
			(DLListEnd(my_list))));
			
	PRINTTESTRESULTS("Create - begin->prev is null", 2, 
			DLListPrev((DLListPrev((DLListBegin(my_list))))) == (NULL) );
			
	PRINTTESTRESULTS("Create - end->next is null", 3, 
			DLListNext((((DLListEnd(my_list))))) == (NULL) );
			
	DLListDestroy(my_list);
}



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

/*The function inserts a new node before iterator.
 * The function return the new node.
 */
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

/* The function pops the front node.
 * The function return pointer to the data that was in the node.
 */
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
			DLListSize(my_list) == 8);		
		PRINTTESTRESULTS("Splice - size test", 2, 
			DLListSize(my_list2) == 2);		
		PRINTTESTRESULTS("Splice", 2, 
			*(int*)DLListGetData(DLListNext(iter)) == *(arr2 + 1));	
		PRINTTESTRESULTS("Splice", 2, 
			*(int*)DLListGetData(DLListNext(DLListBegin(my_list2))) 
				== *(arr2 + 4));
							

	
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




