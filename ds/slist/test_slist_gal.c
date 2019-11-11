#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "slist.h"

#define RUN_TEST(TEST, NAME) (TEST) ?\
printf ("Test %-20s\t" KGRN "PASS" KNRM "\n", NAME) :\
printf("Test %-30s\t" KRED "FAIL" KNRM "\n", NAME)

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

void TestFlip();
void TestCreateAndInsert();
void TestSListRemove();
void TestFreeAll();
void TestHasLoop();
void TestIntersection();
void TestForEach();
void TestFindInNodes();
int IncreaseByParameter(sl_node_t *node, void *param);
int FindValue(const sl_node_t *node, void *param);

int main()
{
	TestFlip();
	TestCreateAndInsert();
	TestSListRemove();
	TestFreeAll();
	TestHasLoop();
	TestIntersection();
	TestForEach();
	TestFindInNodes();
	
	return 0;
}

void TestFlip()
{
	int a = 1, b = 2, c = 3, d = 4, e = 5;
	int val = 0;
	
	sl_node_t *node1 = NULL;
	sl_node_t *node2 = NULL;
	sl_node_t *node3 = NULL;
	sl_node_t *node4 = NULL;
	sl_node_t *node5 = NULL;	
	
	node5 = SListCreateNode(&e, NULL);
	node4 = SListCreateNode(&d, node5);
	node3 = SListCreateNode(&c, node4);
	node2 = SListCreateNode(&b, node3);
	node1 = SListCreateNode(&a, node2);	
	
	val = *(int*)node1->data;
	RUN_TEST((val == a), "Flip");
	val = *(int*)(node1->next)->data;
	RUN_TEST((val == b), "Flip");	
	
	SListFlip(node1);
	
	val = *(int*)node5->data;
	RUN_TEST((val == e), "Flip");
	val = *(int*)(node5->next)->data;
	RUN_TEST((val == d), "Flip");
	
	SListFreeAll(node5);
}

void TestCreateAndInsert()
{
	int a = 1, b = 2, c = 3, d = 4;
	int val = 0;
	
	sl_node_t *node1 = NULL;
	sl_node_t *node2 = NULL;
	sl_node_t *node3 = NULL;
	sl_node_t *node4 = NULL;

	node4 = SListCreateNode(&d, NULL);
	node3 = SListCreateNode(&c, NULL);
	node2 = SListCreateNode(&b, NULL);
	node1 = SListCreateNode(&a, node3);
	
	SListInsert(node2, node3);
	SListInsertAfter(node4, node2);
	
	val = *(int*)node1->data;
	RUN_TEST((val == a), "Create And Insert");
	
	val = *(int*)(((node1->next)->next)->next)->data;
	RUN_TEST((val == d), "Create And Insert");
	
	SListFreeAll(node1);	
}

void TestSListRemove()
{
	int a = 1, b = 2, c = 3, d = 4, e = 5;
	size_t counter = 0;
	
	sl_node_t *node1 = NULL;
	sl_node_t *node2 = NULL;
	sl_node_t *node3 = NULL;
	sl_node_t *node4 = NULL;
	sl_node_t *node5 = NULL;	
	
	node5 = SListCreateNode(&e, NULL);
	node4 = SListCreateNode(&d, node5);
	node3 = SListCreateNode(&c, node4);
	node2 = SListCreateNode(&b, node3);
	node1 = SListCreateNode(&a, node2);	
	
	counter = 5;
	RUN_TEST((SListCount(node1) == counter), "Count Before Delete");
	/*
	printf("%d \n", *(int*)node1->data);
	printf("%d \n", *(int*)(node1->next)->data);
	printf("%d \n", *(int*)((node1->next)->next)->data);
	printf("%d \n", *(int*)(((node1->next)->next)->next)->data);
	printf("%d \n", *(int*)((((node1->next)->next)->next)->next)->data);
	*/
	SListRemoveAfter(node2);
	free(node3);
	counter = 4;
	RUN_TEST((SListCount(node1) == counter), "Count After Delete");
	/*
	printf("%d \n", *(int*)node1->data);
	printf("%d \n", *(int*)(node1->next)->data);
	printf("%d \n", *(int*)((node1->next)->next)->data);
	printf("%d \n", *(int*)(((node1->next)->next)->next)->data);
	*/
	SListRemove(node4);
	counter = 3;
	RUN_TEST((SListCount(node1) == counter), "Count After Delete");
	
	free(node5);
	SListFreeAll(node1);
}

void TestFreeAll()
{
	int a = 1, b = 2, c = 3, d = 4, e = 5;
	
	sl_node_t *node1 = NULL;
	sl_node_t *node2 = NULL;
	sl_node_t *node3 = NULL;
	sl_node_t *node4 = NULL;
	sl_node_t *node5 = NULL;	
	
	node5 = SListCreateNode(&e, NULL);
	node4 = SListCreateNode(&d, node5);
	node3 = SListCreateNode(&c, node4);
	node2 = SListCreateNode(&b, node3);
	node1 = SListCreateNode(&a, node2);		
	
	SListFreeAll(node1);
}

void TestHasLoop()
{
	int a = 1, b = 2, c = 3, d = 4, e = 5;
	int is_have_loop = 0;
	
	sl_node_t *node1 = NULL;
	sl_node_t *node2 = NULL;
	sl_node_t *node3 = NULL;
	sl_node_t *node4 = NULL;
	sl_node_t *node5 = NULL;	
	
	node5 = SListCreateNode(&e, NULL);
	node4 = SListCreateNode(&d, node5);
	node3 = SListCreateNode(&c, node4);
	node2 = SListCreateNode(&b, node3);
	node1 = SListCreateNode(&a, node2);
	
	node5->next = node1; /* connect all to 1 loop */
	
	is_have_loop = SListHasLoop(node1);
	
	RUN_TEST((is_have_loop == 1), "Hes Loop");
		
	node5->next = NULL; /* dis-connect loop */
	
	is_have_loop = SListHasLoop(node1);
	RUN_TEST((is_have_loop == 0), "Hes Loop");
	
	SListFreeAll(node1);	

	/*	
	printf("%d \n", *(int*)node1->data);
	printf("%d \n", *(int*)(node1->next)->data);
	printf("%d \n", *(int*)((node1->next)->next)->data);
	printf("%d \n", *(int*)(((node1->next)->next)->next)->data);
	printf("%d \n", *(int*)((((node1->next)->next)->next)->next)->data);
	printf("%d \n", *(int*)(((((node1->next)->next)->next)->next)->next)->data);	
	*/
}

void TestIntersection()
{
	int a = 1, b = 2, c = 3, d = 4, e = 5;
	
	sl_node_t *node1 = NULL;
	sl_node_t *node2 = NULL;
	sl_node_t *node3 = NULL;
	sl_node_t *node4 = NULL;
	sl_node_t *node5 = NULL;
	sl_node_t *node_check = NULL;	
	
	node5 = SListCreateNode(&e, NULL);
	node4 = SListCreateNode(&d, node5);
	node3 = SListCreateNode(&c, NULL);
	node2 = SListCreateNode(&b, node3);
	node1 = SListCreateNode(&a, node2);
	
	node5->next = node4;
	node4->next = node3;
	
	node_check = SListFindIntersection(node1, node5);
	
	RUN_TEST((node_check == node3), "Intersection");
	
	SListFreeAll(node1);
	free(node4);
	free(node5);
	
	/*
	printf("%d \n", *(int*)node1->data);
	printf("%d \n", *(int*)(node1->next)->data);
	printf("%d \n", *(int*)((node1->next)->next)->data); */	
}

void TestForEach()
{
	for_each_ptr for_each = IncreaseByParameter;
	int a = 1, b = 2, c = 3, d = 4, e = 5;
	int param = 10;	
	int check_data = 0;	
		
	sl_node_t *node1 = NULL;
	sl_node_t *node2 = NULL;
	sl_node_t *node3 = NULL;
	sl_node_t *node4 = NULL;
	sl_node_t *node5 = NULL;	
	
	node5 = SListCreateNode(&e, NULL);
	node4 = SListCreateNode(&d, node5);
	node3 = SListCreateNode(&c, node4);
	node2 = SListCreateNode(&b, node3);
	node1 = SListCreateNode(&a, node2);	
	
	SListForEach(node1, &param, for_each);
	
	check_data = *(int*)node1->data;		
	RUN_TEST((check_data == 11), "Intersection");
	
	SListFreeAll(node1);
	/*
	printf("%d \n", *(int*)node1->data);
	printf("%d \n", *(int*)(node1->next)->data);
	printf("%d \n", *(int*)((node1->next)->next)->data);
	printf("%d \n", *(int*)(((node1->next)->next)->next)->data);
	printf("%d \n", *(int*)((((node1->next)->next)->next)->next)->data);*/	
}

void TestFindInNodes()
{	
	int a = 1, b = 2, c = 3, d = 4, e = 5;
	int param = 3;
	find_ptr find = FindValue;

	sl_node_t *node1 = NULL;
	sl_node_t *node2 = NULL;
	sl_node_t *node3 = NULL;
	sl_node_t *node4 = NULL;
	sl_node_t *node5 = NULL;	
	
	node5 = SListCreateNode(&e, NULL);
	node4 = SListCreateNode(&d, node5);
	node3 = SListCreateNode(&c, node4);
	node2 = SListCreateNode(&b, node3);
	node1 = SListCreateNode(&a, node2);		
	
	SListFind(node1, &param, find);
	SListFreeAll(node1);
}

int IncreaseByParameter(sl_node_t *node, void *param)
{	
	if (*(int*)node->data > 3)
	{
		return 1;
	}
	
	*(int*)node->data += *(int*)param;
	
	return 0;	
}

int FindValue(const sl_node_t *node, void *param)
{
	if (*(int*)param == *(int*)node->data)
	{
		return 1;		
	}
	
	return 0;
}



