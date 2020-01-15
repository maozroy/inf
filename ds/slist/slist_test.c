#include "slist.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RUN_TEST(TEST,NAME) \
	(TEST) ? \
	printf("Test"KBLU"  %-50s"KGRN" PASS\n"KNRM, (NAME)) : \
	printf("Test"KBLU"  %-50s"KRED" FAIL\n"KNRM, (NAME));
void TestSListCreateNode();
void TestSListInsert();
void TestSListInsertAfter();
void TestSListRemove();
void TestSListCount();
void TestSListFreeAll();
void TestSListRemoveAfter();
void TestSListFlip();
void TestSListHasLoop();
void TestSListForEach();
void TestSListFind();
void TestSListFindIntersection();
int EnterTwoInAll(sl_node_t *node, void *param);
int FindInNode(sl_node_t *node, void *param);

int main()
{
	TestSListCreateNode();
	TestSListInsert();
	TestSListInsertAfter();
	TestSListRemove();
	TestSListCount();
	TestSListFreeAll();
	TestSListRemoveAfter();
	TestSListFlip();
/*	TestSListHasLoop();
	TestSListForEach();
	TestSListFind();
	TestSListFindIntersection();*/
	return 0;
	
}
/*
void TestSListCreateNode()
{
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	sl_node_t *ptr_a = NULL;
	sl_node_t *ptr_b = NULL;
	sl_node_t *ptr_c = NULL;
	sl_node_t *ptr_d = NULL;
	printf("\n\t\tCreate Test \n");
	ptr_d = SListCreateNode(&d, NULL);
	ptr_c = SListCreateNode(&c, ptr_d);
	ptr_b = SListCreateNode(&b, ptr_c);
	ptr_a = SListCreateNode(&a, ptr_b);

	RUN_TEST((ptr_a->next) == ptr_b, "Link to next node");
	RUN_TEST((ptr_d->next) == NULL, "Link to next node (NULL)");
	RUN_TEST(*(int*)(ptr_b->data) == b, "Value assigment");
	RUN_TEST((ptr_d->data) == &d, "Pointer assigment");
	SListFreeAll(ptr_a);
}

void TestSListInsert()
{
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	sl_node_t *ptr_a = NULL;
	sl_node_t *ptr_b = NULL;
	sl_node_t *ptr_c = NULL;
	sl_node_t *ptr_d = NULL;
	printf("\n\t\tInsert Test \n");
	ptr_d = SListCreateNode(&d, NULL);
	ptr_c = SListCreateNode(&c, NULL);
	ptr_b = SListCreateNode(&b, ptr_c);
	ptr_a = SListCreateNode(&a, ptr_b);
	SListInsert(ptr_d, ptr_b);
	RUN_TEST(*(int*)(ptr_a->next->data) == 4, "pos points to next");
	RUN_TEST(*(int*)(ptr_d->next->data) == 3, "new points to old->next");
	RUN_TEST(*(int*)(ptr_b->next->data) == 2, "pos->next points to b");
	SListFreeAll(ptr_a);

}

void TestSListInsertAfter()
{
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	sl_node_t *ptr_a = NULL;
	sl_node_t *ptr_b = NULL;
	sl_node_t *ptr_c = NULL;
	sl_node_t *ptr_d = NULL;
	printf("\n\t\tInsertAfter Test \n");
	ptr_d = SListCreateNode(&d, NULL);
	ptr_c = SListCreateNode(&c, NULL);
	ptr_b = SListCreateNode(&b, ptr_c);
	ptr_a = SListCreateNode(&a, ptr_b);
	SListInsertAfter(ptr_d, ptr_b);
	RUN_TEST(*(int*)(ptr_a->next->data) == 2, "pos-1 before points to new");
	RUN_TEST(*(int*)(ptr_d->next->data) == 3, "new points to pos->next");
	RUN_TEST(*(int*)(ptr_b->next->data) == 4, "new->next points to pos");
	SListFreeAll(ptr_a);
}

void TestSListRemove()
{
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	sl_node_t *ptr_a = NULL;
	sl_node_t *ptr_b = NULL;
	sl_node_t *ptr_c = NULL;
	sl_node_t *ptr_d = NULL;
	printf("\n\t\tRemove Test \n");
	ptr_d = SListCreateNode(&d, NULL);
	ptr_c = SListCreateNode(&c, ptr_d);
	ptr_b = SListCreateNode(&b, ptr_c);
	ptr_a = SListCreateNode(&a, ptr_b);
	
	SListRemove(ptr_a);
	RUN_TEST((ptr_a->next) == ptr_c, "node->next points to next->next");
	SListFreeAll(ptr_a);
	SListFreeAll(ptr_b);

	
}

void TestSListRemoveAfter()
{
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	sl_node_t *ptr_a = NULL;
	sl_node_t *ptr_b = NULL;
	sl_node_t *ptr_c = NULL;
	sl_node_t *ptr_d = NULL;
	printf("\n\t\tRemoveAfter Test \n");
	ptr_d = SListCreateNode(&d, NULL);
	ptr_c = SListCreateNode(&c, ptr_d);
	ptr_b = SListCreateNode(&b, ptr_c);
	ptr_a = SListCreateNode(&a, ptr_b);
	
	SListRemoveAfter(ptr_a);
	RUN_TEST((ptr_a->next) == ptr_c, "node->next is node->next->next");
	SListRemoveAfter(ptr_a);
	RUN_TEST((ptr_a->next) == ptr_d, "second remove gets next->next->next");
	SListFreeAll(ptr_a);
	SListFreeAll(ptr_b);
	SListFreeAll(ptr_c);
}


void TestSListCount()
{
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	sl_node_t *ptr_a = NULL;
	sl_node_t *ptr_b = NULL;
	sl_node_t *ptr_c = NULL;
	sl_node_t *ptr_d = NULL;
	printf("\n\t\tCount Test \n");
	ptr_d = SListCreateNode(&d, NULL);
	ptr_c = SListCreateNode(&c, NULL);
	ptr_b = SListCreateNode(&b, ptr_c);
	ptr_a = SListCreateNode(&a, ptr_b);
	
	RUN_TEST(SListCount(ptr_a) == 3, "Count Check");
	RUN_TEST(SListCount(ptr_d) == 1, "Count Check");
	SListFreeAll(ptr_a);
	SListFreeAll(ptr_d);
}

void TestSListHasLoop()
{
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	sl_node_t *ptr_a = NULL;
	sl_node_t *ptr_b = NULL;
	sl_node_t *ptr_c = NULL;
	sl_node_t *ptr_d = NULL;
	printf("\n\t\tHasLoop? Test \n");
	ptr_d = SListCreateNode(&d, NULL);
	ptr_c = SListCreateNode(&c, ptr_d);
	ptr_b = SListCreateNode(&b, ptr_c);
	ptr_a = SListCreateNode(&a, ptr_b);
	
	
	RUN_TEST(SListHasLoop(ptr_d) == 0, "Loop Check");
	ptr_d->next = ptr_a;
	RUN_TEST(SListHasLoop(ptr_a) == 1, "Loop Check");


}

int EnterTwoInAll(sl_node_t *node, void *param)
{
	node->data = (int*)param;
	
	return 0;
}

int FindInNode(sl_node_t *node, void *param)
{
	if (*(int*)node->data == *(int*)param)
	{
		return 1;
	}
	
	return 0;
}

void TestSListFind()
{
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	int counter = 4;
	sl_node_t *ptr_a = NULL;
	sl_node_t *ptr_b = NULL;
	sl_node_t *ptr_c = NULL;
	sl_node_t *ptr_d = NULL;
	printf("\n\t\tFind Test \n");
	ptr_d = SListCreateNode(&d, NULL);
	ptr_c = SListCreateNode(&c, ptr_d);
	ptr_b = SListCreateNode(&b, ptr_c);
	ptr_a = SListCreateNode(&a, ptr_b);
	
	RUN_TEST(SListFind(ptr_a, &counter, (find_ptr)FindInNode) == ptr_d, "Find Check");
	counter = 0;
	RUN_TEST(SListFind(ptr_a, &counter, (find_ptr)FindInNode) == NULL, "Find Check");
	

}

void TestSListForEach()
{
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	int counter = 2;
	sl_node_t *ptr_a = NULL;
	sl_node_t *ptr_b = NULL;
	sl_node_t *ptr_c = NULL;
	sl_node_t *ptr_d = NULL;
	printf("\n\t\tForEach Test \n");
	ptr_d = SListCreateNode(&d, NULL);
	ptr_c = SListCreateNode(&c, ptr_d);
	ptr_b = SListCreateNode(&b, ptr_c);
	ptr_a = SListCreateNode(&a, ptr_b);
	
	SListForEach(ptr_a, &counter, EnterTwoInAll);
	RUN_TEST(*(int*)ptr_d->data == 2, "ForEach Check");
	RUN_TEST(*(int*)ptr_c->data == 2, "ForEach Check");
	

}


void TestSListFreeAll()
{
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	sl_node_t *ptr_a = NULL;
	sl_node_t *ptr_b = NULL;
	sl_node_t *ptr_c = NULL;
	sl_node_t *ptr_d = NULL;
	printf("\n\t\tFreeAll Test \n");
	ptr_d = SListCreateNode(&d, NULL);
	ptr_c = SListCreateNode(&c, NULL);
	ptr_b = SListCreateNode(&b, ptr_c);
	ptr_a = SListCreateNode(&a, ptr_b);
	SListFreeAll(ptr_a);
	SListFreeAll(ptr_d);
}

void TestSListFlip()
{
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	sl_node_t *ptr_a = NULL;
	sl_node_t *ptr_b = NULL;
	sl_node_t *ptr_c = NULL;
	sl_node_t *ptr_d = NULL;
	printf("\n\t\tFlip Test \n");
	ptr_d = SListCreateNode(&d, NULL);
	ptr_c = SListCreateNode(&c, ptr_d);
	ptr_b = SListCreateNode(&b, ptr_c);
	ptr_a = SListCreateNode(&a, ptr_b);
	
	RUN_TEST(SListFlip(ptr_a) == ptr_d, "Flip return new head");
		RUN_TEST((ptr_a->next) == NULL, "Flip old head points to null");
	RUN_TEST((ptr_d->next) == ptr_c, "Flip new head points to one before");
	SListFlip(ptr_d);
	RUN_TEST(SListFindIntersection(ptr_a, ptr_b) == NULL, "Intersection+flip test");

	SListFreeAll(ptr_d);
	
}

void TestSListFindIntersection()
{
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	int e = 5;
	int f = 6;
	int g = 7;
	int h = 8;
	sl_node_t *ptr_a = NULL;
	sl_node_t *ptr_b = NULL;
	sl_node_t *ptr_c = NULL;
	sl_node_t *ptr_d = NULL;
	sl_node_t *ptr_e = NULL;
	sl_node_t *ptr_f = NULL;
	sl_node_t *ptr_g = NULL;
	sl_node_t *ptr_h = NULL;
	printf("\n\t\tIntersection Test \n");
	ptr_h = SListCreateNode(&h, NULL);
	ptr_g = SListCreateNode(&g, ptr_h);
	ptr_f = SListCreateNode(&f, ptr_g);
	ptr_e = SListCreateNode(&e, ptr_f);
	
	ptr_d = SListCreateNode(&d, ptr_e);
	ptr_c = SListCreateNode(&c, ptr_d);
	ptr_b = SListCreateNode(&b, ptr_c);
	ptr_a = SListCreateNode(&a, ptr_b);

	SListFlip(ptr_f);
	RUN_TEST(SListFindIntersection(ptr_h, ptr_a) == ptr_f, "super test");
	RUN_TEST(SListFindIntersection(ptr_a, ptr_e) == NULL, "Intersection test");
	RUN_TEST(SListFindIntersection(ptr_a, ptr_b) == NULL, "Intersection test");
	RUN_TEST(SListFindIntersection(ptr_a, ptr_a) == NULL, "Intersection test");
	SListFreeAll(ptr_h);
	SListFreeAll(ptr_a);
	
}
*/
