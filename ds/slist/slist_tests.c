#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include "slist.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define PRINTTESTRESULTS(func,num, res) \
(printf("%-55s: Test %d %s\n" KNRM,func, num, (res) == 1 ?\
KGRN "\t\tpassed" : KRED "\t\tfailed"))

static void PrintSList(sl_node_t *head);
int FindNum(const sl_node_t *node, void *param);
int ForEach(sl_node_t *node, void *param);

void TestsCreateInsertCountDestroy();
void TestsCreateRemoveCountDestroy();
void TestsCreateFlipDestroy();
void TestsCreateFindDestroy();
void TestsCreateForEachDestroy();
void TestsHasLoop();
void TestsFindIntersection();
	
int main()
{
	TestsCreateInsertCountDestroy();
	TestsCreateRemoveCountDestroy();
	TestsCreateFlipDestroy();	
	TestsCreateFindDestroy();
	TestsCreateForEachDestroy();
	TestsHasLoop();
	TestsFindIntersection();	
	
	return 0;
}

static void PrintSList(sl_node_t *head)
{	
	while (NULL != head -> next)
	{
		printf("%d, ", *(int *)(head -> data));
		head = (head -> next);
	}
	
	printf("%d\n", *(int *)(head -> data));
}

int FindNum(const sl_node_t *node, void *param)
{
	if (*(int *)param == *(int *)(node -> data))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int ForEach(sl_node_t *node, void *param)
{
	*(int *)node -> data += *(int *)param;
	
	return 1;
}

void TestsCreateInsertCountDestroy()
{
	int a1 = 1;
	int a2 = 2;
	int a3 = 3;
	int a4 = 4;
	
	sl_node_t *last_node = SListCreateNode(&a4, NULL);
	sl_node_t *head = SListCreateNode(&a1, last_node);
	sl_node_t *insert_after_first_node = SListCreateNode(&a2, NULL);
	sl_node_t *insert_before_last_node = SListCreateNode(&a3, NULL);
	
	PRINTTESTRESULTS("TestsCreateInsertCountDestroy_SListCount",1, 
	2 == SListCount(head));
	
	PRINTTESTRESULTS("TestsCreateInsertCountDestroy_SListInsert",2,
	(head -> next)  == 
	SListInsert(insert_before_last_node, last_node));
	
	PRINTTESTRESULTS("TestsCreateInsertCountDestroy_SListCount",3, 
	3 == SListCount(head));
	
	PRINTTESTRESULTS("TestsCreateInsertCountDestroy_SListInsertAfter",4,
	a2 == *(int *)(SListInsertAfter(insert_after_first_node, head)) -> data);
	
	PRINTTESTRESULTS("TestsCreateInsertCountDestroy_SListCount",5, 
	4 == SListCount(head));
	
	PRINTTESTRESULTS("TestsCreateInsertCountDestroy_headData",6,
	a1 == *(int *)(head -> data));
	PRINTTESTRESULTS("TestsCreateInsertCountDestroy_secondData",7,
	a2 == *(int *)((head -> next) -> data));
	PRINTTESTRESULTS("TestsCreateInsertCountDestroy_thirdData",8,
	a3 == *(int *)(((head -> next) -> next) -> data));
	PRINTTESTRESULTS("TestsCreateInsertCountDestroy_lastData",9,
	a4 == *(int *)((((head -> next) -> next) -> next) -> data));
	
	SListFreeAll(head);
	
	printf("\n\n");
}

void TestsCreateRemoveCountDestroy()
{
	int a1 = 1;
	int a2 = 2;
	int a3 = 3;
	int a4 = 4;
	
	sl_node_t *temp_node = NULL;
	sl_node_t *remove_after_node = NULL; 
	sl_node_t *last_node = SListCreateNode(&a4, NULL);
	sl_node_t *head = SListCreateNode(&a1, last_node);
	sl_node_t *insert_after_first_node = SListCreateNode(&a2, NULL);
	sl_node_t *insert_before_last_node = SListCreateNode(&a3, NULL);
	insert_before_last_node = SListInsert(insert_before_last_node, last_node);
	insert_after_first_node = SListInsertAfter(insert_after_first_node, head);
	
	PrintSList(head);
	
	temp_node = SListRemove(insert_before_last_node);
	PRINTTESTRESULTS("TestsCreateRemoveCountDestroy_SListRemove",1, 
	a3 == *(int *)(temp_node -> data));
	free(temp_node);
	
	PRINTTESTRESULTS("TestsCreateRemoveCountDestroy_SListCount",2, 
	3 == SListCount(head));
	
	remove_after_node = head -> next;
	temp_node = SListRemoveAfter(head);
	PRINTTESTRESULTS("TestsCreateRemoveCountDestroy_SListRemoveAfter",3, 
	a1 == *(int *)(temp_node -> data));
	free(remove_after_node);
	
	PRINTTESTRESULTS("TestsCreateRemoveCountDestroy_SListCount",4, 
	2 == SListCount(head));
	
	PRINTTESTRESULTS("TestsCreateInsertCountDestroy_headData",5,
	a1 == *(int *)(head -> data));
	PRINTTESTRESULTS("TestsCreateInsertCountDestroy_secondData",6,
	a4 == *(int *)((head -> next) -> data));
	
	remove_after_node = head -> next;
	SListRemoveAfter(head);
	PRINTTESTRESULTS("TestsCreateRemoveCountDestroy_SListCount",7, 
	1 == SListCount(head));
	free(remove_after_node);

	SListFreeAll(head);
	
	printf("\n\n");
}

void TestsCreateFlipDestroy()
{
	int a1 = 1;
	int a2 = 2;
	int a3 = 3;
	int a4 = 4;
	
	sl_node_t *temp_node = NULL;
	sl_node_t *last_node = SListCreateNode(&a4, NULL);
	sl_node_t *head = SListCreateNode(&a1, last_node);
	sl_node_t *insert_after_first_node = SListCreateNode(&a2, NULL);
	sl_node_t *insert_before_last_node = SListCreateNode(&a3, NULL);
	insert_before_last_node = SListInsert(insert_before_last_node, last_node);
	insert_after_first_node = SListInsertAfter(insert_after_first_node, head);
	
	PRINTTESTRESULTS("TestsCreateFlipDestroy_headData",1,
	a1 == *(int *)(head -> data));
	PRINTTESTRESULTS("TestsCreateFlipDestroy_secondData",2,
	a2 == *(int *)((head -> next) -> data));
	PRINTTESTRESULTS("TestsCreateFlipDestroy_thirdData",3,
	a3 == *(int *)(((head -> next) -> next) -> data));
	PRINTTESTRESULTS("TestsCreateFlipDestroy_thirdData",4,
	a4 == *(int *)((((head -> next) -> next) -> next) -> data));

	temp_node = SListFlip(head);
	printf("FLip\n");
	
	PRINTTESTRESULTS("TestsCreateFlipDestroy_headData",5,
	a4 == *(int *)(temp_node -> data));
	PRINTTESTRESULTS("TestsCreateFlipDestroy_secondData",6,
	a3 == *(int *)((temp_node -> next) -> data));
	PRINTTESTRESULTS("TestsCreateFlipDestroy_thirdData",7,
	a2 == *(int *)(((temp_node -> next) -> next) -> data));
	PRINTTESTRESULTS("TestsCreateFlipDestroy_thirdData",8,
	a1 == *(int *)((((temp_node -> next) -> next) -> next) -> data));
	
	temp_node = SListFlip(temp_node);
	printf("FLip\n");
	
	PRINTTESTRESULTS("TestsCreateFlipDestroy_headData",9,
	a1 == *(int *)(head -> data));
	PRINTTESTRESULTS("TestsCreateFlipDestroy_secondData",10,
	a2 == *(int *)((head -> next) -> data));
	PRINTTESTRESULTS("TestsCreateFlipDestroy_thirdData",11,
	a3 == *(int *)(((head -> next) -> next) -> data));
	PRINTTESTRESULTS("TestsCreateFlipDestroy_thirdData",12,
	a4 == *(int *)((((head -> next) -> next) -> next) -> data));

	SListFreeAll(head);
	
	printf("\n\n");
}

void TestsCreateFindDestroy()
{
	int a1 = 1;
	int a2 = 2;
	int a3 = 3;
	int a4 = 4;
	int x = 10;
	find_ptr ptr = FindNum;
		
	sl_node_t *last_node = SListCreateNode(&a4, NULL);
	sl_node_t *head = SListCreateNode(&a1, last_node);
	sl_node_t *insert_after_first_node = SListCreateNode(&a2, NULL);
	sl_node_t *insert_before_last_node = SListCreateNode(&a3, NULL);
	insert_before_last_node = SListInsert(insert_before_last_node, last_node);
	insert_after_first_node = SListInsertAfter(insert_after_first_node, head);
	
	PRINTTESTRESULTS("TestsCreateFindDestroy_Find",1,
	head == SListFind(head, &a1, ptr));
	PRINTTESTRESULTS("TestsCreateFindDestroy_Find",2,
	(head -> next) -> next == SListFind(head, &a3, ptr));
	PRINTTESTRESULTS("TestsCreateFindDestroy_Find",3,
	NULL == SListFind(head, &x, ptr));
	
	SListFreeAll(head);
	
	printf("\n\n");
}

void TestsCreateForEachDestroy()
{
	int a1 = 1;
	int a2 = 2;
	int a3 = 3;
	int a4 = 4;
	int x = 10;
	int a1_after_change = a1 + x;
	int a2_after_change = a2 + x;	
	int a3_after_change = a3 + x;
	int a4_after_change = a4 + x;
	for_each_ptr ptr = ForEach;

	sl_node_t *last_node = SListCreateNode(&a4, NULL);
	sl_node_t *head = SListCreateNode(&a1, last_node);
	sl_node_t *insert_after_first_node = SListCreateNode(&a2, NULL);
	sl_node_t *insert_before_last_node = SListCreateNode(&a3, NULL);
	insert_before_last_node = SListInsert(insert_before_last_node, last_node);
	insert_after_first_node = SListInsertAfter(insert_after_first_node, head);
	
	PRINTTESTRESULTS("TestsCreateForEachDestroy_forEach",1,
	0 == SListForEach(head, &x, ptr));
	
	PRINTTESTRESULTS("TestsCreateForEachDestroy_headData",2,
	a1_after_change == *(int *)(head -> data));
	PRINTTESTRESULTS("TestsCreateForEachDestroy_secondData",3,
	a2_after_change == *(int *)((head -> next) -> data));
	PRINTTESTRESULTS("TestsCreateForEachDestroy_thirdData",4,
	a3_after_change == *(int *)(((head -> next) -> next) -> data));
	PRINTTESTRESULTS("TestsCreateForEachDestroy_thirdData",5,
	a4_after_change == *(int *)((((head -> next) -> next) -> next) -> data));
	
	SListFreeAll(head);
	
	printf("\n\n");
}

void TestsHasLoop()
{
	int a1 = 1;
	int a2 = 2;
	int a3 = 3;
	int a4 = 4;
	int a5 = 5;
	int a6 = 6;
	int a7 = 7;
	int a8 = 8;
	
	sl_node_t *temp_node = NULL;
	sl_node_t *eight_node = SListCreateNode(&a8, NULL);
	sl_node_t *seven_node = SListCreateNode(&a7, eight_node);
	sl_node_t *six_node = SListCreateNode(&a6, seven_node);
	sl_node_t *five_node = SListCreateNode(&a5, six_node);
	sl_node_t *four_node = SListCreateNode(&a4, five_node);
	sl_node_t *three_node = SListCreateNode(&a3, four_node);
	sl_node_t *two_node = SListCreateNode(&a2, three_node);
	sl_node_t *head = SListCreateNode(&a1, two_node);
	
	
	PRINTTESTRESULTS("TestsHasLoop_SListCount",1, 
	8 == SListCount(head));
	PRINTTESTRESULTS("TestsHasLoop_SListHasLoop",2, 
	0 == SListHasLoop(head));
	
	
	temp_node = eight_node;
	seven_node -> next = three_node;
	free(temp_node);
	
	PRINTTESTRESULTS("TestsHasLoop_SListHasLoop",3,
	1 == SListHasLoop(head));
	
	seven_node -> next = NULL;
	
	SListFreeAll(head);
	
	printf("\n\n");
}

void TestsFindIntersection()
{
	int a1 = 1;
	int a2 = 2;
	int a3 = 3;
	int a4 = 4;
	int a5 = 5;
	int a6 = 6;
	int a7 = 7;
	int a8 = 8;
	
	sl_node_t *temp_node = NULL;
	sl_node_t *eight_node = SListCreateNode(&a8, NULL);
	sl_node_t *seven_node = SListCreateNode(&a7, eight_node);
	sl_node_t *six_node = SListCreateNode(&a6, seven_node);
	sl_node_t *five_node = SListCreateNode(&a5, six_node);
	sl_node_t *four_node = SListCreateNode(&a4, five_node);
	sl_node_t *three_node = SListCreateNode(&a3, four_node);
	sl_node_t *two_node = SListCreateNode(&a2, three_node);
	sl_node_t *head = SListCreateNode(&a1, two_node);
	
	PRINTTESTRESULTS("TestsFindIntersection_SListFindIntersection",1,
	NULL == SListFindIntersection(head, seven_node));
	
	temp_node = SListFlip(four_node);
	printf("Flip");
	
	PRINTTESTRESULTS("TestsFindIntersection_SListFindIntersection",2, 
	four_node == SListFindIntersection (head, seven_node));
	
	SListFlip(head);
	
	PRINTTESTRESULTS("TestsFindIntersection_SListFindIntersection",3, 
	NULL == SListFindIntersection (head, seven_node));
	
	PRINTTESTRESULTS("TestsFindIntersection_SListCount",4, 
	8 == SListCount(temp_node));
		
	SListFreeAll(temp_node);
}
