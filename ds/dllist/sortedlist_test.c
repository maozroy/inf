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

void PrintIntList(dl_list_t *mylist);
void TestDLListForEach(void);
void TestDLListFind(void);
void TestSrtListInsert(void);
void TestSrtListPopFront(void);
void TestSrtListPopBack(void);
void TestSrtListRemove(void);
void TestSrtListMerge(void);
void TestSrtListFindIf(void);
void TestSrtListInsertStruct(void);

int MyIntAlgo(const void *data1, const void *data2, void *param);
int AddToNode(dll_node_t *node, void *param);
int IntFindFunc(const void *data, const void *param);
int MyStructAlgo(const void *data1, const void *data2, void *param);

typedef struct countries
{
	char *name;
	long pop;
	float literacy;
}my_countries;

int main()
{
	TestSrtListMerge();
	TestSrtListCreate();
	TestSrtListInsert();
	TestSrtListPopFront();
	TestSrtListPopBack();
	TestSrtListRemove();

	TestSrtListFindIf();
	TestSrtListInsertStruct();
	return 0;
}
void TestSrtListInsertStruct(void)
{
	my_countries record[8];
	size_t i = 0;
	srt_list_t *my_list = SrtListCreate(NULL, MyStructAlgo);
	srt_iter_t my_iter = {0};
	
	
	record[0].name = "Lala Land";
	record[0].pop = 8000000001;
	record[0].literacy = 0.2;

	record[1].name = "Shaquanza";
	record[1].pop = 3;
	record[1].literacy = 1.6;
	
	record[2].name = "Mordor";
	record[2].pop = 33333333;
	record[2].literacy = 0.005;
	
	record[3].name = "DwarfLand";
	record[3].pop = 7;
	record[3].literacy = 0;
	
	record[4].name = "Shmilpatia";
	record[4].pop = 8888888888888;
	record[4].literacy = 0.95;
	
	record[5].name = "Liliput";
	record[5].pop = 123456789;
	record[5].literacy = 0.54;
	
	record[6].name = "FrupyLand";
	record[6].pop = 1;
	record[6].literacy = 1;
	
	record[7].name = "The Wall";
	record[7].pop = 1200;
	record[7].literacy = 0.4;	
	
	
	for (i = 0 ; i < 8 ; i++)
	{
		my_iter = SrtListInsert( &record[i], my_list);
	}
	
	PRINTTESTRESULTS("Insert Return Struct", 1, 
			(((my_countries *)SrtListGetData(SrtListBegin(my_list)))->pop) 
								== 8888888888888);
	PRINTTESTRESULTS("Insert Last Struct", 1, 
			(((my_countries *)SrtListGetData(SrtListPrev(SrtListEnd(my_list))))->pop) == 1);
	
	
	SrtListDestroy(my_list);
}
int MyStructAlgo(const void *data1, const void *data2, void *param)
{
	(void)param;
	if ( (((my_countries *)data1)->pop) > (((my_countries *)data2)->pop) )
	{
		return 1;
	}
	
	return 0;
}

int MyIntAlgo(const void *data1, const void *data2, void *param)
{
	(void)param;
	if ( (*(int*)data1) > (*(int*)data2) )
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
	
	my_iter = SrtListFindIf(SrtListBegin(my_list), SrtListEnd(my_list), 
						 &number, IntFindFunc);
	PRINTTESTRESULTS("FindIf",1 ,SrtListIsSameIterator(my_iter, 
					SrtListNext(SrtListBegin(my_list))));
	
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
	SrtListDestroy(my_list2);
}


