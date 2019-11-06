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

#define NUM_FOR_TESTING 5

#define PRINTTESTRESULTS(description,num, res) \
(printf("%-40s Test %d %s\n",description, num, res == 1 ? KGRN "passed"\
 KNRM : KRED "failed" KNRM))\
/*
typedef int (*find_ptr)(const sl_node_t *node, void *param);
typedef int (*for_each_ptr)(sl_node_t *node, void *param);
	
*/

void TestSListInsertAfter();
void TestSListCreateNode();
void TestSListInsert();
void TestSListCount();
void TestSListRemoveAfter();
void TestSListFlip();
void TestSListHasLoop1();
void TestSListHasLoop2();
void TestSListFindIntersection();
void TestSListRemove();
void TestSListForEach();
void TestSListFind();
static int PrintNodes(sl_node_t *node, void *param);
static int FindNum(const sl_node_t *node, void *param);

static int FindNum(const sl_node_t *node, void *param)
{
	if (node->data == (int *)param)
	{
		return 1;
	}
	
	return 0;
}

static int PrintNodes(sl_node_t *node, void *param)
{
	char *message = (char *)param;
	printf("%s %d\n", message, *((int *)node->data));
	return 0;
}

void TestSListFind()
{
	int n = NUM_FOR_TESTING;
	int x = NUM_FOR_TESTING;
	int y = NUM_FOR_TESTING * 2;
	int z = NUM_FOR_TESTING * 3;
	int result = 0;

	sl_node_t *node_p3 = NULL;
	sl_node_t *node_p2 = NULL;
	sl_node_t *node_p1 = NULL;
	const sl_node_t *result_node = NULL;
	find_ptr ptr = FindNum;
	
	node_p3 = SListCreateNode(&z, NULL);
	node_p2 = SListCreateNode(&y, node_p3);
	node_p1 = SListCreateNode(&x, node_p2);
	
	result = *((int *)(SListFind(node_p1, &y, ptr)->data));
 	PRINTTESTRESULTS("SListFind", 1, (result == 10));
 	
 	result = *((int *)(SListFind(node_p1, &z, ptr)->data));
 	PRINTTESTRESULTS("SListFind", 2, (result == 15));
 	
 	result_node = SListFind(node_p1, &n, ptr);
 	PRINTTESTRESULTS("SListFind", 3, (result_node == NULL));
 	
 	SListFreeAll(node_p1);
}

void TestSListForEach()
{
	int x = NUM_FOR_TESTING;
	int y = NUM_FOR_TESTING * 2;
	int z = NUM_FOR_TESTING * 3;
	sl_node_t * node_p3 = NULL;
	sl_node_t * node_p2 = NULL;
	sl_node_t * node_p1 = NULL;
	for_each_ptr func_ptr = PrintNodes;
	
	printf("\nTesting SlistForEach - values should be 5, 10, 15\n");
	
	node_p3 = SListCreateNode(&z, NULL);
	node_p2 = SListCreateNode(&y, node_p3);
	node_p1 = SListCreateNode(&x, node_p2);
	
	SListForEach(node_p1, "My Value is: ", func_ptr);
	
 	SListFreeAll(node_p1);
}

void TestSListRemove()
{
	int x = NUM_FOR_TESTING;
	int *p_x = &x;
	sl_node_t *node_p4 = NULL;
	sl_node_t *node_p3 = NULL;
	sl_node_t *node_p2 = NULL;
	sl_node_t *node_p1 = NULL;
	
	node_p4 = SListCreateNode(p_x, NULL);
	node_p3 = SListCreateNode(p_x, node_p4);
	node_p2 = SListCreateNode(p_x, node_p3);
	node_p1 = SListCreateNode(p_x, node_p2);
	
	
	node_p3 = SListRemove(node_p3);
 	PRINTTESTRESULTS("SListRemove", 1, (node_p2->next == node_p3));
 	
 	SListFreeAll(node_p1);
 	SListFreeAll(node_p4);
 	
}

void TestSListFindIntersection()
{
	int x = NUM_FOR_TESTING;
	sl_node_t *node1_p3 = SListCreateNode(&x, NULL);
	sl_node_t *node1_p2 = SListCreateNode(&x, node1_p3);
	sl_node_t *node1_p1 = SListCreateNode(&x, node1_p2);
	
	sl_node_t *node2_p3 = SListCreateNode(&x, NULL);
	sl_node_t *node2_p2 = SListCreateNode(&x, node2_p3);
	sl_node_t *node2_p1 = SListCreateNode(&x, node2_p2);
	PRINTTESTRESULTS("SListFindIntersection", 1,
	 				(SListFindIntersection(node1_p1, node2_p1) == NULL));
	
	node2_p3->next = node1_p3;
	PRINTTESTRESULTS("SListFindIntersection", 2,
	 				(SListFindIntersection(node1_p1, node2_p1) == node1_p3));
	
	node2_p3->next = NULL;		
 	SListFreeAll(node1_p1);
 	SListFreeAll(node2_p1);
}

void TestSListHasLoop1()
{
	int x = NUM_FOR_TESTING;
	int *p_x = &x;
	sl_node_t *node_p3 = SListCreateNode(p_x, NULL);
	sl_node_t *node_p2 = SListCreateNode(p_x, node_p3);
	sl_node_t *node_p1 = SListCreateNode(p_x, node_p2);
 	PRINTTESTRESULTS("SListHasLoop", 1, (SListHasLoop(node_p1) == 0));
 	SListFreeAll(node_p1);
}

void TestSListHasLoop2()
{
	int x = NUM_FOR_TESTING;
	int *p_x = &x;
	sl_node_t *node_p3 = SListCreateNode(p_x, NULL);
	sl_node_t *node_p2 = SListCreateNode(p_x, node_p3);
	sl_node_t *node_p1 = SListCreateNode(p_x, node_p2);
	node_p3->next = node_p1;
 	PRINTTESTRESULTS("SListHasLoop", 2, (SListHasLoop(node_p1) == 1));
 	node_p3->next = NULL;
 	SListFreeAll(node_p1);
}

void TestSListFlip()
{
	int x = NUM_FOR_TESTING;
	int *p_x = &x;
	sl_node_t *node_p4 = SListCreateNode(p_x, NULL);
	sl_node_t *node_p3 = SListCreateNode(p_x, node_p4);
	sl_node_t *node_p2 = SListCreateNode(p_x, node_p3);
	sl_node_t *node_p1 = SListCreateNode(p_x, node_p2);
 	PRINTTESTRESULTS("SListFlip", 1, (node_p4->next == NULL));
	node_p4 = SListFlip(node_p1);
 	PRINTTESTRESULTS("SListFlip", 2, (node_p4->next == node_p3));
 	PRINTTESTRESULTS("SListFlip", 3, (node_p1->next == NULL));	
 	SListFreeAll(node_p4);
}

void TestSListRemoveAfter()
{
	int x = NUM_FOR_TESTING;
	int *p_x = &x;
	sl_node_t *node_p4 = SListCreateNode(p_x, NULL);
	sl_node_t *node_p3 = SListCreateNode(p_x, node_p4);
	sl_node_t *node_p2 = SListCreateNode(p_x, node_p3);
	sl_node_t *node_p1 = SListCreateNode(p_x, node_p2);
	SListRemoveAfter(node_p2);
 	PRINTTESTRESULTS("SListRemoveAfter", 1, (node_p2->next == node_p4));
	SListRemoveAfter(node_p1);
 	PRINTTESTRESULTS("SListRemoveAfter", 2, (node_p1->next == node_p4));
 	SListFreeAll(node_p1);
  	SListFreeAll(node_p3);
 	SListFreeAll(node_p2);
 	
}

void TestSListCount()
{
	int x = NUM_FOR_TESTING;
	int *p_x = &x;
	sl_node_t *node_p1 = NULL;
	sl_node_t *node_p2 = SListCreateNode(p_x, NULL);
 	PRINTTESTRESULTS("SListCount", 1, (SListCount(node_p2) == 1));
	node_p1 = SListCreateNode(p_x, node_p2);
 	PRINTTESTRESULTS("SListCount", 2, (SListCount(node_p1) == 2));
	SListFreeAll(node_p1);
}

void TestSListInsert()
{
	int x = NUM_FOR_TESTING;
	int *p_x = &x;
	int y = NUM_FOR_TESTING * 4;
	int *p_y = &y;
	sl_node_t *node_p1 = SListCreateNode(p_x, NULL);
	sl_node_t *node_p2 = SListCreateNode(p_x, NULL);
	sl_node_t *node_p3 = NULL;
	
	SListInsertAfter(node_p2, node_p1);
 	PRINTTESTRESULTS("SListInsert", 1, (node_p1->next == node_p2));
	node_p3 = SListCreateNode(p_y, NULL);
	node_p3 = SListInsert(node_p3, node_p2);
 	PRINTTESTRESULTS("SListInsert", 2, (node_p1->next == node_p3));
 	PRINTTESTRESULTS("SListInsert", 3, ((*((int *)node_p3->data)) == 20));
	SListFreeAll(node_p1);
}

void TestSListInsertAfter()
{
	int x = NUM_FOR_TESTING;
	int *p_x = &x;
	sl_node_t *node_p1 = SListCreateNode(p_x, NULL);
	sl_node_t *node_p2 = SListCreateNode(p_x, NULL);
	SListInsertAfter(node_p2, node_p1);
 	PRINTTESTRESULTS("SListInsertAfter", 1, (node_p1->next == node_p2));
 	SListFreeAll(node_p1);
}

void TestSListCreateNode()
{
	int x = 5;
	int *p_x = &x;
	sl_node_t *node_p1 = NULL;
	sl_node_t *node_p2 = SListCreateNode(p_x, NULL);
  	PRINTTESTRESULTS("SListCreateNode", 1, (node_p2 != NULL));
  	x = 10;
 	node_p1 = SListCreateNode(p_x, node_p2);
 	PRINTTESTRESULTS("SListCreateNode", 2, (node_p1 != NULL));
 	PRINTTESTRESULTS("SListCreateNode", 3, ((*((int *)node_p1->data)) == 10));
 	PRINTTESTRESULTS("SListCreateNode", 4, (node_p1->next == node_p2));
 	SListFreeAll(node_p1);
}

int main()
{	
		TestSListRemove();
/**/
 	TestSListFind();
 	TestSListForEach();
 	

 	TestSListFindIntersection();
	TestSListHasLoop1();
	TestSListHasLoop2();
	TestSListFlip();
	TestSListRemoveAfter();
	TestSListCount();
	TestSListInsert();
	TestSListCreateNode();
	TestSListInsertAfter();
	return 0;
}

