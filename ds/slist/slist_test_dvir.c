/********************************************
 *              OL-79 - ds                  *
 *             SList Tests  				*
 *                                          *
 *    										*
 *               04/11/19                   *
 ********************************************/

#include <stdio.h>      /* For printf  */ 
#include <stdlib.h>     /* for malloc  */
#include "slist.h"

#define PRINT_TEST(test, name, num) \
printf("Function: %-22sTest #%d  %s\n",\
(name), (num), (test ? "\033[0;32mPassed\033[0m" : "\033[0;31mFailed\033[0m"))\
 


void SlistCreateNodeTest();
void SListFreeAllTest();
void SListInsertsTest();
void SListCountTest();
void SListFindTest();
void SListRemovesTest();
void SListFlipTest();
void SListForeachTest();
void SListHasLoopTest();
void SListFindIntersectionTest();

int main ()
{	
	printf("\n");
	SlistCreateNodeTest();
	SListFreeAllTest();
	SListInsertsTest();
	SListCountTest();
	SListFindTest();
	SListRemovesTest();
	SListFlipTest();
	SListForeachTest();		
	SListHasLoopTest();
	SListFindIntersectionTest();
	
	return 0;
} 

void SlistCreateNodeTest()
{
	int test_n[4] = {3,42,89,6};	
	sl_node_t *test_p4 = SListCreateNode(&test_n[3], NULL);
	sl_node_t *test_p3 = SListCreateNode(&test_n[2], test_p4);
	sl_node_t *test_p2 = SListCreateNode(&test_n[1], test_p3);
	sl_node_t *test_p1 = SListCreateNode(&test_n[0], test_p2);
	
	int test1 = ( 3 == *((int *)test_p1->data) );
	int test2 = ( 6 == *((int *)test_p2->next->next->data) );
	
	PRINT_TEST(test1, "SlistCreateNode", 1);
	PRINT_TEST(test2, "SlistCreateNode", 2);
	printf("\n");

	SListFreeAll(test_p1);
}

void SListFreeAllTest()
{
	printf("Function: SlistFreeAll          Test #1  \033[01;33mCheck valgrind");
	printf(" for any memory leaks\033[0m\n\n");
}

void SListInsertsTest()
{
	int test_n[4] = {3,42,89,6};	
	sl_node_t *test_p5 = SListCreateNode(&test_n[2], NULL);
	sl_node_t *test_p4 = SListCreateNode(&test_n[3], NULL);
	sl_node_t *test_p3 = SListCreateNode(&test_n[2], NULL);
	sl_node_t *test_p2 = SListCreateNode(&test_n[1], test_p3);
	sl_node_t *test_p1 = SListCreateNode(&test_n[0], test_p2);
	
	int test1 = ( 6 == *((int *)(SListInsert(test_p4, test_p2)->data)) );
	int test2 = ( 89 == *((int *)test_p1->next->next->next->data) );
	
	PRINT_TEST(test1, "SlistInsert", 1);
	PRINT_TEST(test2, "SlistInsert", 2);
	printf("\n");
	
	test1 = ( 89 == *((int *)(SListInsertAfter(test_p5, test_p2)->data)) );
	test2 = ( *((int *)test_p1->next->next->next->next->data) == 
			  *((int *)test_p1->next->next->data) );
	
	PRINT_TEST(test1, "SlistInsertAfter", 1);
	PRINT_TEST(test2, "SlistInsertAfter", 2);
	printf("\n");
	
	SListFreeAll(test_p1);
}

void SListCountTest()
{
	int test_n[4] = {3,42,89,6};	
	sl_node_t *test_p4 = SListCreateNode(&test_n[3], NULL);
	sl_node_t *test_p3 = SListCreateNode(&test_n[2], test_p4);
	sl_node_t *test_p2 = SListCreateNode(&test_n[1], test_p3);
	sl_node_t *test_p1 = SListCreateNode(&test_n[0], test_p2);
	
	int test1 = (4 == SListCount(test_p1));
	int test2 = (2 == SListCount(test_p3));
	
	PRINT_TEST(test1, "SlistCount", 1);
	PRINT_TEST(test2, "SlistCount", 2);
	printf("\n");
	
	SListFreeAll(test_p1);
} 

static int IsDivN(const sl_node_t *node, void *param)
{
	return !( *((int *)node->data) % *((int *)param));
}

void SListFindTest()
{
	int test_n[4] = {3,42,89,6};	
	sl_node_t *test_p4 = SListCreateNode(&test_n[3], NULL);
	sl_node_t *test_p3 = SListCreateNode(&test_n[2], test_p4);
	sl_node_t *test_p2 = SListCreateNode(&test_n[1], test_p3);
	sl_node_t *test_p1 = SListCreateNode(&test_n[0], test_p2);
	
	find_ptr div_ptr = IsDivN;	
	int param1 = 7;
	int param2 = 9;
	
	int test1 = (42 == *(int *)(SListFind(test_p1, (void *)&param1, div_ptr)->data));
	int test2 = ( NULL == SListFind(test_p1, (void *)&param2, div_ptr ));
	
	PRINT_TEST(test1, "SlistFind", 1);
	PRINT_TEST(test2, "SlistFind", 2);
	printf("\n");
	
	SListFreeAll(test_p1);
}

void SListRemovesTest()
{
	sl_node_t *array[10];
	int data[10] = {0,1,2,3,4,5,6,7,8,9};
	int i = 9;
	int test1 = 0;
	int test2 = 0;
	
	array[9] = SListCreateNode(&data[i], NULL);
	for (--i ; i >= 0 ; --i)
	{
		array[i] = SListCreateNode(&data[i], array[i+1]);
	}
	
	test1 = (4 == *((int *)SListRemove(array[3])->data));
	printf("%d",*(int*)array[4]->data);
	test2 = ( 3 == (*((int *)array[4]->data)) );
	free(array[4]);
	
	PRINT_TEST(test1, "SlistRemove", 1);
	PRINT_TEST(test2, "SlistRemove", 2);
	printf("\n");
	
	test1 = (8 == *((int *)SListRemoveAfter(array[8])->data));
	free(array[9]);
	SListRemoveAfter(array[3]);
	free(array[5]);
	test2 = ( 8 == (*((int *)array[3]->next->next->next->data)) );
	
	PRINT_TEST(test1, "SlistRemoveAfter", 1);
	PRINT_TEST(test2, "SlistRemoveAfter", 2);
	printf("\n");
	
	SListFreeAll(array[0]);
}

void SListFlipTest()
{
	sl_node_t *array[5];
	int data[10] = {0,1,2,3,4};
	int i = 4;
	int test1 = 0;
	int test2 = 0;
	
	array[4] = SListCreateNode(&data[i], NULL);
	for (--i ; i >= 0 ; --i)
	{
		array[i] = SListCreateNode(&data[i], array[i+1]);
	}
	
	test1 = ( 2 == *((int *)(SListFlip(array[1])->next->next->data)) );
	test2 = ( NULL == ((array[0])->next->next) );
	
	PRINT_TEST(test1, "SlistFlip", 1);
	PRINT_TEST(test2, "SlistFlip", 2);
	printf("\n");
	
	array[0]->next = NULL;
	SListFreeAll(array[0]);
	SListFreeAll(array[4]);
}

static int AddN(sl_node_t *node, void *param)
{
	*((int *)node->data) = (*((int *)node->data) + *((int *)param));
	
	return 0;
}

void SListForeachTest()
{
	int test_n[4] = {3,42,89,6};	
	sl_node_t *test_p4 = SListCreateNode(&test_n[3], NULL);
	sl_node_t *test_p3 = SListCreateNode(&test_n[2], test_p4);
	sl_node_t *test_p2 = SListCreateNode(&test_n[1], test_p3);
	sl_node_t *test_p1 = SListCreateNode(&test_n[0], test_p2);
	
	find_ptr div_ptr = IsDivN;	
	for_each_ptr add_ptr = AddN;	
	int param1 = 2;
	int param2 = 11;

	int test1 = (0 == SListForEach(test_p1, (void *)&param1, add_ptr));
	int test2 = ( 44 == *((int *)SListFind(test_p1, (void *)&param2, div_ptr )->data));
	
	PRINT_TEST(test1, "SlistForEach", 1);
	PRINT_TEST(test2, "SlistForEach", 2);
	printf("\n");
	
	SListFreeAll(test_p1);
}

void SListHasLoopTest()
{
	sl_node_t *array[10];
	int data[10] = {0,1,2,3,4,5,6,7,8,9};
	int i = 9;
	int test1 = 0;
	int test2 = 0;
	
	array[9] = SListCreateNode(&data[i], NULL);
	for (--i ; i >= 0 ; --i)
	{
		array[i] = SListCreateNode(&data[i], array[i+1]);
	}
	
	
	test1 = !SListHasLoop(array[0]);
	array[9]->next = array[2];
	test2 = SListHasLoop(array[0]);
	
	PRINT_TEST(test1, "SlistHasLoop", 1);
	PRINT_TEST(test2, "SlistHasLoop", 2);
	printf("\n");
	
	array[9]->next = NULL;
	SListFreeAll(array[0]);
}

void SListFindIntersectionTest()
{
	sl_node_t *array[10];
	int data[10] = {0,1,2,3,4,5,6,7,8,9};
	int i = 9;
	int test1 = 0;
	int test2 = 0;
	int test3 = 0;
	
	array[9] = SListCreateNode(&data[i], NULL);
	for (--i ; i >= 0 ; --i)
	{
		array[i] = SListCreateNode(&data[i], array[i+1]);
	}
	
	test1 = (NULL == SListFindIntersection(array[1], array[5]));
	test2 = (array[3] == SListFindIntersection(array[0],SListFlip(array[3])));
	array[3]->next = array[0];
	array[0]->next = NULL;
	test3 = (NULL == SListFindIntersection(array[0],array[9]));
	
	PRINT_TEST(test1, "SlistFindIntersection", 1);
	PRINT_TEST(test2, "SlistFindIntersection", 2);
	PRINT_TEST(test3, "SlistFindIntersection", 3);
	printf("\n");
	
	array[2]->next = NULL;
	SListFreeAll(array[1]);
	SListFreeAll(array[9]);
}

/****************************************************

printf("%d\n", (*((int *)array[0]->data)) );
	printf("%d\n", (*((int *)array[1]->data)) );
	printf("%d\n", (*((int *)array[2]->data)) );
	printf("%d\n", (*((int *)array[3]->data)) );
	printf("%d\n", (*((int *)array[4]->data)) );
	printf("%d\n", (*((int *)array[5]->data)) );
	printf("%d\n", (*((int *)array[6]->data)) );
	printf("%d\n", (*((int *)array[7]->data)) );
	printf("%d\n", (*((int *)array[8]->data)) );
	printf("%d\n", (*((int *)array[9]->data)) );










Testing scenarios

create list 7

flip 4 +
create 2 after 4

connect new head with after flip
has loops
remove 1 before flip

free no valgrind!!!!




create small loop - test for loop alone???




*/





