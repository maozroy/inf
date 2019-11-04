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
	printf("Test"KBLU" %s"KGRN" PASS\n"KNRM, (NAME)) : \
	printf("Test"KBLU" %s"KRED" FAIL\n"KNRM, (NAME));
void TestSListCreateNode();
void TestSListInsert();
void TestSListInsertAfter();
void TestSListRemove();
void TestSListCount();


int main()
{
	TestSListCreateNode();
	TestSListInsert();
	TestSListInsertAfter();
	TestSListRemove();
	TestSListCount();
	return 0;
	
}

void TestSListCreateNode()
{
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	node_t *ptr_a = NULL;
	node_t *ptr_b = NULL;
	node_t *ptr_c = NULL;
	node_t *ptr_d = NULL;
	
	ptr_d = SListCreateNode(&d, NULL);
	ptr_c = SListCreateNode(&c, ptr_d);
	ptr_b = SListCreateNode(&b, ptr_c);
	ptr_a = SListCreateNode(&a, ptr_b);

	RUN_TEST((ptr_b->next) == ptr_c, "Link to next node");
	RUN_TEST((ptr_d->next) == NULL, "Link to next node (NULL)");

	RUN_TEST(*(int*)(ptr_b->data) == b, "Value assigment");
	RUN_TEST((ptr_d->data) == &d, "Pointer assigment");
}

void TestSListInsert()
{
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	node_t *ptr_a = NULL;
	node_t *ptr_b = NULL;
	node_t *ptr_c = NULL;
	node_t *ptr_d = NULL;
	
	ptr_d = SListCreateNode(&d, NULL);
	ptr_c = SListCreateNode(&c, NULL);
	ptr_b = SListCreateNode(&b, ptr_c);
	ptr_a = SListCreateNode(&a, ptr_b);
	printf("%d\n",*(int*)ptr_d->data);
	printf("%d\n",*(int*)ptr_b->data);
	SListInsert(ptr_d, ptr_b);
	RUN_TEST(*(int*)(ptr_a->next->data) == 4, "Insert Check");
	RUN_TEST(*(int*)(ptr_d->next->data) == 3, "Insert Check");
	RUN_TEST(*(int*)(ptr_b->next->data) == 2, "Insert Check");
}

void TestSListInsertAfter()
{
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	node_t *ptr_a = NULL;
	node_t *ptr_b = NULL;
	node_t *ptr_c = NULL;
	node_t *ptr_d = NULL;
	
	ptr_d = SListCreateNode(&d, NULL);
	ptr_c = SListCreateNode(&c, NULL);
	ptr_b = SListCreateNode(&b, ptr_c);
	ptr_a = SListCreateNode(&a, ptr_b);
	SListInsertAfter(ptr_d, ptr_b);
	RUN_TEST(*(int*)(ptr_a->next->data) == 2, "InsertAfter Check");
	RUN_TEST(*(int*)(ptr_d->next->data) == 3, "InsertAfter Check");
	RUN_TEST(*(int*)(ptr_b->next->data) == 4, "InsertAfter Check");
}

void TestSListRemove()
{
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	node_t *ptr_a = NULL;
	node_t *ptr_b = NULL;
	node_t *ptr_c = NULL;
	node_t *ptr_d = NULL;
	
	ptr_d = SListCreateNode(&d, NULL);
	ptr_c = SListCreateNode(&c, NULL);
	ptr_b = SListCreateNode(&b, ptr_c);
	ptr_a = SListCreateNode(&a, ptr_b);
	
	SListRemove(ptr_b);
	RUN_TEST(*(int*)(ptr_a->next->data) == 3, "Remove Check");
	RUN_TEST(ptr_c->data == NULL, "Remove Check");
	SListRemove(ptr_d);
	RUN_TEST(ptr_c->next->data == NULL, "Remove Check");
}


void TestSListCount()
{
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	int counter = 0;
	node_t *ptr_a = NULL;
	node_t *ptr_b = NULL;
	node_t *ptr_c = NULL;
	node_t *ptr_d = NULL;
	
	ptr_d = SListCreateNode(&d, NULL);
	ptr_c = SListCreateNode(&c, NULL);
	ptr_b = SListCreateNode(&b, ptr_c);
	ptr_a = SListCreateNode(&a, ptr_b);
	
	RUN_TEST(SListCount(ptr_a) == 3, "Count Check");
	RUN_TEST(SListCount(ptr_d) == 1, "Count Check");
}








