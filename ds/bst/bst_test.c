 
#include "bst.h"
#include <stdio.h>

#define PRINT_TEST(test, name, num) \
printf("Function: %-17sTest #%d  %s\n", \
(name), (num), (test ? "\033[0;32mPassed\033[0m" : "\033[0;31mFailed\033[0m")) 
#define TEST(name, int) ((int)>0) ? (printf("%-30s" KGRN " pass" KNRM "\n", name)) \
									: (printf("%-30s" KRED " fail" KNRM "\n", name))

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

struct bst_node
{
	bst_node_t *parent;
	bst_node_t *left;
	bst_node_t *right;
	void *data;
};

struct bst
{
	comparison_func comparison_func;
	void *comparison_param;
	bst_node_t stub;
};

int my_cmp (const void *new_data, 
							   const void *src_data, 
							   const void *comp_param);

void TestBSTRemove();
void TestBSTInsert();
void TestBSTFind();
void TestBSTForEach();
void TestBSTDestroy();
static int my_action(void *data, void *for_each_param);

int main()
{

	TestBSTInsert();
	TestBSTRemove();
	TestBSTFind();
	TestBSTForEach();
	TestBSTDestroy();
	
	return 0;
}

static int my_action(void *data, void *for_each_param)
{
	if (*(int*)data > *(int*)for_each_param)
	{
		return 1;
	}

	*(int*)data *= 2;

	return 0;
}

void TestBSTDestroy()
{
	int arr[10] = {5, 1, 2, 8, 3, 7, 4, 6, 9, 0};
	int i = 0;
	int action_param = 8;
		
	bst_t *tree = BSTCreate(my_cmp, NULL);
	bst_iter_t my_iter[10] = {0};
	for (i = 0 ; i < 10 ; i++)
	{
		my_iter[i] = BSTInsert(tree, &arr[i]);
	}
	BSTDestroy(tree);


}

void TestBSTForEach()
{
	int arr[10] = {5, 1, 2, 8, 3, 7, 4, 6, 9, 0};
	int i = 0;
	int action_param = 8;
		
	bst_t *tree = BSTCreate(my_cmp, NULL);
	bst_iter_t my_iter[10] = {0};
	for (i = 0 ; i < 10 ; i++)
	{
		my_iter[i] = BSTInsert(tree, &arr[i]);
	}
	TEST("ForEach", BSTForEach(BSTBegin(tree), BSTEnd(tree), my_action, &action_param) == 1);
	action_param = 20;
	TEST("ForEach", BSTForEach(BSTBegin(tree), BSTEnd(tree), my_action, &action_param) == 0);
	TEST("Find and ForEach", BSTFind(tree, &arr[2]) == my_iter[2]);
	BSTDestroy(tree);
}

void TestBSTFind()
{
	int arr[10] = {5, 1, 2, 8, 3, 7, 4, 6, 9, 0};
	int i = 0;
		
	bst_t *tree = BSTCreate(my_cmp, NULL);
	bst_iter_t my_iter[10] = {0};
	for (i = 0 ; i < 10 ; i++)
	{
		my_iter[i] = BSTInsert(tree, &arr[i]);
	}
	TEST("Find", BSTFind(tree, &arr[0]) == my_iter[0]);
	TEST("Find", BSTFind(tree, &i) == BSTEnd(tree));
		BSTDestroy(tree);
}

void TestBSTRemove()
{
	int arr[10] = {5, 1, 2, 8, 3, 7, 4, 6, 9, 0};
	int i = 0;
		
	bst_t *tree = BSTCreate(my_cmp, NULL);
	bst_iter_t my_iter[10] = {0};
	TEST("Remove Size", BSTSize(tree) == 0);
	for (i = 0 ; i < 10 ; i++)
	{
		my_iter[i] = BSTInsert(tree, &arr[i]);
	}
	
	BSTRemove(my_iter[1]);
	TEST("Remove", *(int*)BSTGetData(BSTNext(BSTBegin(tree))) == arr[2]);
	BSTRemove(my_iter[5]);
	TEST("Remove", *(int*)BSTGetData(BSTPrev(BSTPrev(BSTPrev(BSTEnd(tree))))) == arr[7]);
	BSTRemove(my_iter[9]);
	TEST("Remove", *(int*)BSTGetData(BSTBegin(tree)) == arr[2]);
	BSTRemove(my_iter[3]);
	TEST("Remove", *(int*)BSTGetData(BSTPrev(BSTPrev(BSTPrev(BSTEnd(tree))))) == arr[0]);
	BSTRemove(my_iter[0]);
	TEST("Remove", *(int*)BSTGetData(BSTNext(BSTBegin(tree))) == arr[4]);
	TEST("Remove Size", BSTSize(tree) == 5);
		BSTDestroy(tree);
}

void TestBSTInsert()
{
	int arr[10] = {5, 1, 2, 8, 3, 7, 4, 6, 9, 0};
	int i = 0;
		
	bst_t *tree = BSTCreate(my_cmp, NULL);
	bst_iter_t my_iter[10] = {0};

	for (i = 0 ; i < 10 ; i++)
	{
		my_iter[i] = BSTInsert(tree, &arr[i]);
	}
	
	TEST("First Element", *(int*)BSTGetData(my_iter[9]) == 
							*(int*)BSTGetData(BSTBegin(tree)));
	TEST("Third Element", *(int*)BSTGetData(my_iter[2]) == 
							*(int*)BSTGetData(BSTNext(BSTNext(BSTBegin(tree)))));		
	TEST("Last Element", *(int*)BSTGetData(my_iter[8]) ==
						 *(int*)BSTGetData(BSTPrev(BSTEnd(tree))));
 	BSTDestroy(tree);
}


int my_cmp (const void *new_data, 
							   const void *src_data, 
							   const void *comp_param)
{
	if (*(int *)new_data > *(int*)src_data)
	{
		return 1;
	}
	else if (*(int *)new_data < *(int*)src_data)
	{
		return -1;
	}
	return 0;

}
