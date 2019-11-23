/****************************************************************************
*                                                                           *
*                                Tamir Dayan                                *
*                              OL79 - 13/11/19                              *
*                                                                           *	
*                                Sorted List                                *
*                           Reviewer: Maoz Roytman                          *
*****************************************************************************/
#include <stdio.h> /* printf */

#include "sortedlist.h" /* sortedlist.h */
#include "dllist.h" /* dllist.h */
#include "myutils.h" /* myutils.h */

static int IsBefore(const void *new_data, const void *src_data, void *param);
static int IncreaseByParameter(void *data, void *param);
static int FindValue(const void *data, const void *param);

void TestSrtListCreate();
void TestSrtListDestroy();
void TestSrtListInsert();
void TestSrtListPopFront();
void TestSrtListPopBack();
void TestSrtListRemove();
void TestSrtListSize();
void TestSrtListIsEmpty();
void TestSrtListFind();
void TestSrtListFindIf();
void TestSrtListForEach();
void TestSrtListMerge();
void TestSrtListBegin();
void TestSrtListEnd();
void TestSrtListNext();
void TestSrtListPrev();
void TestSrtListGetData();
void TestSrtListIsSameIterator();

int main()
{
	TestSrtListCreate();
	TestSrtListDestroy();
	TestSrtListSize();
	TestSrtListIsEmpty();
	TestSrtListInsert();
	TestSrtListPopFront();
	TestSrtListPopBack();
	TestSrtListRemove();
	
	TestSrtListMerge(); 
	TestSrtListForEach();
	
/*	TestSrtListFind();
	TestSrtListFindIf();
*/
    return 0;
}

void TestSrtListCreate()
{
	srt_list_t *list = NULL;
	void *param = NULL;
	is_before_t is_before_ptr = IsBefore;
	 
	printf("\n*****" BBLU "TestSrtListCreate" KNRM "*****\n");
	TEST("TestSrtListCreate", 1, NULL == list);
	list = SrtListCreate(param, is_before_ptr);
	TEST("TestSrtListCreate", 2, NULL != list);
	TEST("TestSrtListCreate", 3, 1 == SrtListIsEmpty(list));
	TEST("TestSrtListCreate", 4, 0 == SrtListSize(list));
	printf("***** ********* *****\n");
	SrtListDestroy(list);
}

void TestSrtListDestroy()
{
	srt_list_t *list = NULL;
	void *param = NULL;
	is_before_t is_before_ptr = IsBefore;
	 
	int num1 = 101;
	int num2 = 102;
	int num3 = 103;
    int num4 = 104;
    
	list = SrtListCreate(param, is_before_ptr);
	SrtListInsert(&num1, list);
	SrtListInsert(&num4, list);
	SrtListInsert(&num2, list);
	SrtListInsert(&num3, list);
	SrtListDestroy(list);
	
	printf("\n*****" BBLU "TestSrtListDestroy" KNRM "*****\n");
	printf(BRED "***** CHECK FOR VALGRIND *****" KNRM "\n");
	printf("***** ********* *****\n");
}

void TestSrtListSize()
{
	srt_list_t *list = NULL;
	void *param = NULL;
	is_before_t is_before_ptr = IsBefore;	
	int num1 = 101;
	int num2 = 102;
	int num3 = 103;
    int num4 = 104;
    
	list = SrtListCreate(param, is_before_ptr);

	printf("\n*****" BBLU "TestSrtListSize" KNRM "*****\n");
	TEST("TestSrtListSize", 1, 0 == SrtListSize(list));
	SrtListInsert(&num1, list);
	TEST("TestSrtListSize", 2, 1 == SrtListSize(list));
	SrtListInsert(&num4, list);
	TEST("TestSrtListSize", 3, 2 == SrtListSize(list));
	SrtListInsert(&num3, list);
	TEST("TestSrtListSize", 4, 3 == SrtListSize(list));
	SrtListInsert(&num2, list);
	TEST("TestSrtListSize", 5, 4 == SrtListSize(list));
	printf("***** ********* *****\n");
	SrtListDestroy(list);
}

void TestSrtListIsEmpty()
{
	srt_list_t *list = NULL;
	void *param = NULL;
	is_before_t is_before_ptr = IsBefore;
	int num1 = 101;
	int num2 = 102;
	int num3 = 103;
    int num4 = 104;
    
	list = SrtListCreate(param, is_before_ptr);

	printf("\n*****" BBLU "TestSrtListIsEmpty" KNRM "*****\n");
	TEST("TestSrtListIsEmpty", 1, 1 == SrtListIsEmpty(list));
	SrtListInsert(&num1, list);
	TEST("TestSrtListIsEmpty", 2, 0 == SrtListIsEmpty(list));
	SrtListInsert(&num4, list);
	TEST("TestSrtListIsEmpty", 3, 0 == SrtListIsEmpty(list));
	SrtListInsert(&num3, list);
	TEST("TestSrtListIsEmpty", 4, 0 == SrtListIsEmpty(list));
	SrtListInsert(&num2, list);
	TEST("TestSrtListIsEmpty", 5, 0 == SrtListIsEmpty(list));
	printf("***** ********* *****\n");
	SrtListDestroy(list);
}

void TestSrtListInsert()
{
	srt_list_t *list = NULL;
	void *param = NULL;
	is_before_t is_before_ptr = IsBefore;
	srt_iter_t iter = {0};
	int num1 = 101;
	int num2 = 102;
	int num3 = 103;
    int num4 = 104;
    
	list = SrtListCreate(param, is_before_ptr);
	SrtListInsert(&num1, list);
	SrtListInsert(&num4, list);
	SrtListInsert(&num2, list);
	SrtListInsert(&num3, list);
	
	iter = SrtListBegin(list);
	
	printf("\n*****" BBLU "TestSrtListInsert" KNRM "*****\n");
	TEST("TestSrtListInsert", 1, *((int *)(SrtListGetData(iter))) == num1);
	iter = SrtListNext(iter);
	TEST("TestSrtListInsert", 2, *((int *)(SrtListGetData(iter))) == num2);
	iter = SrtListNext(iter);
	TEST("TestSrtListInsert", 3, *((int *)(SrtListGetData(iter))) == num3);
	iter = SrtListNext(iter);
	TEST("TestSrtListInsert", 4, *((int *)(SrtListGetData(iter))) == num4);
	iter = SrtListPrev(SrtListEnd(list));
	TEST("TestSrtListInsert", 5, *((int *)(SrtListGetData(iter))) == num4);
	printf("***** ********* *****\n");
	SrtListDestroy(list);
}

void TestSrtListPopFront()
{
	srt_list_t *list = NULL;
	void *param = NULL;
	is_before_t is_before_ptr = IsBefore;
	int num1 = 1;
	int num2 = 8;
	int num3 = 103;
    int num4 = 1000;
    
	list = SrtListCreate(param, is_before_ptr);
	SrtListInsert(&num2, list);
	SrtListInsert(&num4, list);
	SrtListInsert(&num1, list);
	SrtListInsert(&num3, list);
	
	printf("\n*****" BBLU "TestSrtListPopFront" KNRM "*****\n");
	TEST("TestSrtListPopFront", 1, *((int *)(SrtListPopFront(list))) == num1);
	TEST("TestSrtListPopFront", 2, *((int *)(SrtListPopFront(list))) == num2);	
	TEST("TestSrtListPopFront", 3, *((int *)(SrtListPopFront(list))) == num3);
	TEST("TestSrtListPopFront", 4, *((int *)(SrtListPopFront(list))) == num4);
	printf("***** ********* *****\n");
	SrtListDestroy(list);
}

void TestSrtListPopBack()
{
	srt_list_t *list = NULL;
	void *param = NULL;
	is_before_t is_before_ptr = IsBefore;
	int num1 = 1;
	int num2 = 8;
	int num3 = 103;
    int num4 = 1000;
    
	list = SrtListCreate(param, is_before_ptr);
	SrtListInsert(&num2, list);
	SrtListInsert(&num4, list);
	SrtListInsert(&num1, list);
	SrtListInsert(&num3, list);
	
	printf("\n*****" BBLU "TestSrtListPopBack" KNRM "*****\n");
	TEST("TestSrtListPopBack", 1, *((int *)(SrtListPopBack(list))) == num4);
	TEST("TestSrtListPopBack", 2, *((int *)(SrtListPopBack(list))) == num3);	
	TEST("TestSrtListPopBack", 3, *((int *)(SrtListPopBack(list))) == num2);
	TEST("TestSrtListPopBack", 4, *((int *)(SrtListPopBack(list))) == num1);
	printf("***** ********* *****\n");
	SrtListDestroy(list);
}

void TestSrtListRemove()
{
	srt_list_t *list = NULL;
	void *param = NULL;
	is_before_t is_before_ptr = IsBefore;
	
	srt_iter_t iter1 = {0};
	srt_iter_t iter2 = {0};
	srt_iter_t iter3 = {0};
	srt_iter_t iter4 = {0};
	srt_iter_t iter5 = {0};
	srt_iter_t return_iter = {0};
		
	int num1 = 1;
	int num2 = 8;
	int num3 = 103;
    int num4 = 1000;
	int num5 = 52000;
    
	list = SrtListCreate(param, is_before_ptr);
	iter1 = SrtListInsert(&num1, list);
	iter2 = SrtListInsert(&num2, list);
	iter3 = SrtListInsert(&num3, list);
	iter4 = SrtListInsert(&num4, list);
	iter5 = SrtListInsert(&num5, list);
	
	return_iter = SrtListRemove(iter2);
	
	printf("\n*****" BBLU "TestSrtListRemove" KNRM "*****\n");
	TEST("TestSrtListRemove", 1, *((int *)(SrtListGetData(iter1))) == num1);
	TEST("TestSrtListRemove", 2, 1 == SrtListIsSameIterator(return_iter, iter3));
	return_iter = SrtListRemove(iter3);
	TEST("TestSrtListRemove", 3, 1 == SrtListIsSameIterator(return_iter, iter4));
	TEST("TestSrtListRemove", 4, *((int *)(SrtListGetData(iter4))) == num4);
	TEST("TestSrtListRemove", 5, *((int *)(SrtListGetData(iter5))) == num5);
	printf("***** ********* *****\n");
	SrtListDestroy(list);
}

void TestSrtListMerge()
{
	srt_list_t *list = NULL;
	srt_list_t *dest = NULL;
	void *param = NULL;
	is_before_t is_before_ptr = IsBefore;
	srt_iter_t iter = {0};
	int num1 = 101;
	int num2 = 105;
	int num3 = 110;
    int num4 = 111;
    int num5 = 102;
	int num6 = 106;
	int num7 = 113;
    int num8 = 104;
    
	list = SrtListCreate(param, is_before_ptr);
	SrtListInsert(&num1, list);
	SrtListInsert(&num4, list);
	SrtListInsert(&num2, list);
	SrtListInsert(&num3, list);

	dest = SrtListCreate(param, is_before_ptr);
	SrtListInsert(&num5, list);
	SrtListInsert(&num7, list);
	SrtListInsert(&num6, list);
	SrtListInsert(&num8, list);
	
	SrtListMerge(list, dest);
	iter = SrtListBegin(dest);

	printf("\n*****" BBLU "TestSrtListMerge" KNRM "*****\n");
	TEST("TestSrtListMerge", 1, *((int *)(SrtListGetData(iter))) == num1);
	iter = SrtListNext(iter);
	TEST("TestSrtListMerge", 2, *((int *)(SrtListGetData(iter))) == num5);
	iter = SrtListNext(iter);
	TEST("TestSrtListMerge", 3, *((int *)(SrtListGetData(iter))) == num8);
	iter = SrtListNext(iter);
	TEST("TestSrtListMerge", 4, *((int *)(SrtListGetData(iter))) == num2);
	iter = SrtListNext(iter);
	TEST("TestSrtListMerge", 5, *((int *)(SrtListGetData(iter))) == num6);
	iter = SrtListNext(iter);
	TEST("TestSrtListMerge", 6, *((int *)(SrtListGetData(iter))) == num3);
	iter = SrtListNext(iter);
	TEST("TestSrtListMerge", 7, *((int *)(SrtListGetData(iter))) == num4);
	iter = SrtListNext(iter);
	TEST("TestSrtListMerge", 8, *((int *)(SrtListGetData(iter))) == num7);
	printf("***** ********* *****\n");
	SrtListDestroy(dest);
}


void TestSrtListFindIf()
{
    is_match_func_t match_func = FindValue;
	is_before_t is_before_ptr = IsBefore;
	srt_list_t *list = NULL;
	void *param = NULL;
	
    srt_iter_t iter1 = {0};
	srt_iter_t iter2 = {0};
	srt_iter_t iter3 = {0};
	srt_iter_t iter4 = {0};
	srt_iter_t iter5 = {0};

	int num1 = 101;
	int num2 = 102;
	int num3 = 103;
    int num4 = 104;
	int num5 = 105;
        
	list = SrtListCreate(param, is_before_ptr);
	iter1 = SrtListInsert(&num1, list);
	iter2 = SrtListInsert(&num4, list);
	iter3 = SrtListInsert(&num5, list);
	iter4 = SrtListInsert(&num2, list);
	iter5 = SrtListInsert(&num3, list);

	printf("\n*****" BBLU "TestSrtListFindIf" KNRM "*****\n");
	TEST("TestSrtListFindIf", 1, SrtListIsSameIterator(iter1, 
                              SrtListFindIf(iter1, iter5, &num1, match_func)));
	TEST("TestSrtListFindIf", 2, SrtListIsSameIterator(iter4, 
                              SrtListFindIf(iter1, iter5, &num2, match_func)));
	TEST("TestSrtListFindIf", 3, SrtListIsSameIterator(iter5,
                               SrtListFindIf(iter1, iter4, &num3, match_func)));
	TEST("TestSrtListFindIf", 4, SrtListIsSameIterator(iter2,
                               SrtListFindIf(iter1, iter5, &num4, match_func)));
	TEST("TestSrtListFindIf", 5, SrtListIsSameIterator(iter3,
                               SrtListFindIf(iter1, iter5, &num5, match_func)));
	TEST("TestSrtListFindIf", 6, SrtListIsSameIterator(iter3,
                               SrtListFindIf(iter1, iter3, &num4, match_func)));
	printf("***** ********* *****\n");

	SrtListDestroy(list);
    match_func = NULL;
    list = NULL;
}

void TestSrtListForEach()
{
	action_func_t action_ptr = IncreaseByParameter;
	is_before_t is_before_ptr = IsBefore;
	srt_list_t *list = NULL;
	void *param = NULL;
	
	srt_iter_t begin = {0};
	srt_iter_t end = {0};

	int num1 = 101;
	int num2 = 102;
	int num3 = 103;
    int num4 = 104;
	int num5 = 105;
    int increase = 100;
        
	list = SrtListCreate(param, is_before_ptr);
	SrtListInsert(&num1, list);
	SrtListInsert(&num4, list);
	SrtListInsert(&num5, list);
	SrtListInsert(&num2, list);
	SrtListInsert(&num3, list);
	
	begin = SrtListBegin(list);
	end = SrtListNext(SrtListNext(SrtListNext(begin)));	

	SrtListForEach(begin, end, &increase, action_ptr);

	printf("\n*****" BBLU "TestSrtListForEach" KNRM "*****\n");
	TEST("TestSrtListForEach", 1, *((int *)(SrtListGetData(begin))) == 201);
	begin = SrtListNext(begin);
	TEST("TestSrtListForEach", 2, *((int *)(SrtListGetData(begin))) == 202);
	begin = SrtListNext(begin);
	TEST("TestSrtListForEach", 3, *((int *)(SrtListGetData(begin))) == 103);
	begin = SrtListNext(begin);	
	TEST("TestSrtListForEach", 4, *((int *)(SrtListGetData(begin))) == num4);
	begin = SrtListNext(begin);	
	TEST("TestSrtListForEach", 5, *((int *)(SrtListGetData(begin))) == num5);
	printf("***** ********* *****\n");

	SrtListDestroy(list);
    action_ptr = NULL;
    list = NULL;
}

static int IsBefore(const void *new_data, const void *src_data, void *param)
{
	(void)param;
	
	if (*((int*)new_data) <= (*((int*)src_data)))
	{
		return 1;
	}

	return 0;
}

static int IncreaseByParameter(void *data, void *param)
{
	if (102 < *((int *)(data)))
	{
		return 1;
	}

	*((int *)(data)) += *((int*)param); 

	return 0;
}

static int FindValue(const void *data, const void *param)
{
	if (*((int*)param) == *((int*)data))
	{
		return 1;
	}

	return 0;
}

