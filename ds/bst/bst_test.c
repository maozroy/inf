 
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

void TestBSTCreate();
void TestBSTInsert();

int main()
{
TestBSTCreate();
TestBSTInsert();
}

void TestBSTCreate()
{
	int x = 10;
	bst_t *my_tree = NULL;
	bst_iter_t *my_iter = NULL;

	my_tree = BSTCreate(my_cmp, NULL);
	/*my_iter = BSTInsert(my_tree, &x);*/
	
	
}


void TestBSTInsert()
{
	int x = 10;
	int y = 5;
	int z = 2;

	bst_t *tree = BSTCreate(my_cmp, NULL);
	bst_node_t *node1 = BSTInsert(tree, &x);


	TEST("TestBSTInsert", (*(int*)(tree -> stub.left-> data) == 10));
	printf("%d \n",*(int*)(tree -> stub.left-> data));

	BSTInsert(tree, &y);
	printf("%d \n",*(int*)(tree -> stub.left->left-> data));

	BSTInsert(tree, &z);
	printf("%d \n",*(int*)(tree -> stub.left->left->left-> data));

}

int my_cmp (const void *new_data, 
							   const void *src_data, 
							   const void *comp_param)
{
	return *(int *)new_data - *(int*)src_data;

}
